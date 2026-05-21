#include "physics/projectile_lab.hpp"

namespace physics {

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
    clear_forces(particle_);
    add_gravity(particle_, {0.0, -config_.gravity_mps2});
    add_quadratic_drag(particle_, config_.drag_coefficient_kg_per_m);
    integrate(particle_, config_.timestep_s, config_.integrator);
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

} // namespace physics
