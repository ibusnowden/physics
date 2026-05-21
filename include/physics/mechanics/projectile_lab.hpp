#pragma once

#include "physics/numerics/integrator.hpp"

#include <vector>

namespace physics {

struct ProjectileConfig {
    double mass_kg = 1.0;
    double gravity_mps2 = 9.80665;
    double drag_coefficient_kg_per_m = 0.0;
    double timestep_s = 1.0 / 60.0;
    Vec2 start_position_m = {0.0, 0.0};
    Vec2 start_velocity_mps = {18.0, 18.0};
    Integrator integrator = Integrator::SemiImplicitEuler;
};

struct ProjectileSample {
    double time_s = 0.0;
    Vec2 position_m;
    Vec2 velocity_mps;
    double kinetic_energy_j = 0.0;
    double potential_energy_j = 0.0;
    double total_energy_j = 0.0;
};

ProjectileSample analytic_projectile_sample(const ProjectileConfig& config, double time_s);

class ProjectileLab {
public:
    explicit ProjectileLab(ProjectileConfig config);

    void reset();
    void step();

    const ProjectileConfig& config() const { return config_; }
    void set_config(ProjectileConfig config);

    const Particle& particle() const { return particle_; }
    const ProjectileSample& latest() const { return samples_.back(); }
    const std::vector<ProjectileSample>& samples() const { return samples_; }

private:
    ProjectileSample make_sample() const;

    ProjectileConfig config_;
    Particle particle_;
    double time_s_ = 0.0;
    std::vector<ProjectileSample> samples_;
};

} // namespace physics
