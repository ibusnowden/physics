#include "physics/mechanics/projectile_lab.hpp"

#include <cmath>

namespace physics {

namespace {

Vec2 projectile_acceleration(const ProjectileConfig& config, Vec2 velocity_mps) {
    Vec2 acceleration = {0.0, -config.gravity_mps2};
    const double speed_mps = length(velocity_mps);

    if (speed_mps != 0.0 && config.drag_coefficient_kg_per_m != 0.0) {
        acceleration += velocity_mps * (-(config.drag_coefficient_kg_per_m / config.mass_kg) * speed_mps);
    }

    return acceleration;
}

void integrate_projectile_rk4(Particle& particle, const ProjectileConfig& config) {
    const double dt = config.timestep_s;
    const Vec2 p0 = particle.position_m;
    const Vec2 v0 = particle.velocity_mps;

    const Vec2 k1_p = v0;
    const Vec2 k1_v = projectile_acceleration(config, v0);

    const Vec2 k2_p = v0 + k1_v * (0.5 * dt);
    const Vec2 k2_v = projectile_acceleration(config, v0 + k1_v * (0.5 * dt));

    const Vec2 k3_p = v0 + k2_v * (0.5 * dt);
    const Vec2 k3_v = projectile_acceleration(config, v0 + k2_v * (0.5 * dt));

    const Vec2 k4_p = v0 + k3_v * dt;
    const Vec2 k4_v = projectile_acceleration(config, v0 + k3_v * dt);

    particle.position_m = p0 + (k1_p + k2_p * 2.0 + k3_p * 2.0 + k4_p) * (dt / 6.0);
    particle.velocity_mps = v0 + (k1_v + k2_v * 2.0 + k3_v * 2.0 + k4_v) * (dt / 6.0);
}

} // namespace

ProjectileLab::ProjectileLab(ProjectileConfig config) : config_(config) {
    reset();
}

void ProjectileLab::reset() {
    particle_ = {
        .mass_kg = config_.mass_kg,
        .position_m = config_.start_position_m,
        .velocity_mps = config_.start_velocity_mps,
        .force_n = {},
    };

    time_s_ = 0.0;
    samples_.clear();
    samples_.push_back(make_sample());
}

void ProjectileLab::step() {
    if (config_.integrator == Integrator::RungeKutta4) {
        integrate_projectile_rk4(particle_, config_);
    } else {
        clear_forces(particle_);
        add_gravity(particle_, {0.0, -config_.gravity_mps2});
        add_quadratic_drag(particle_, config_.drag_coefficient_kg_per_m);
        integrate(particle_, config_.timestep_s, config_.integrator);
    }

    time_s_ += config_.timestep_s;
    samples_.push_back(make_sample());
}

void ProjectileLab::set_config(ProjectileConfig config) {
    config_ = config;
    reset();
}

ProjectileSample ProjectileLab::make_sample() const {
    const double kinetic = kinetic_energy_j(particle_);
    const double potential = gravitational_potential_energy_j(particle_, config_.gravity_mps2);

    return {
        .time_s = time_s_,
        .position_m = particle_.position_m,
        .velocity_mps = particle_.velocity_mps,
        .kinetic_energy_j = kinetic,
        .potential_energy_j = potential,
        .total_energy_j = kinetic + potential,
    };
}

ProjectileSample analytic_projectile_sample(const ProjectileConfig& config, double time_s) {
    const Vec2 gravity = {0.0, -config.gravity_mps2};
    Particle particle = {
        .mass_kg = config.mass_kg,
        .position_m = config.start_position_m + config.start_velocity_mps * time_s + gravity * (0.5 * time_s * time_s),
        .velocity_mps = config.start_velocity_mps + gravity * time_s,
        .force_n = {},
    };

    const double kinetic = kinetic_energy_j(particle);
    const double potential = gravitational_potential_energy_j(particle, config.gravity_mps2);

    return {
        .time_s = time_s,
        .position_m = particle.position_m,
        .velocity_mps = particle.velocity_mps,
        .kinetic_energy_j = kinetic,
        .potential_energy_j = potential,
        .total_energy_j = kinetic + potential,
    };
}

} // namespace physics
