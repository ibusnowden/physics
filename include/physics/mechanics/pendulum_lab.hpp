#pragma once

#include "physics/numerics/integrator.hpp"

#include <vector>

namespace physics {

enum class PendulumEquation {
    Nonlinear,
    SmallAngle,
};

const char* name_of(PendulumEquation equation);

struct PendulumConfig {
    double mass_kg = 1.0;
    double length_m = 2.0;
    double gravity_mps2 = 9.80665;
    double timestep_s = 1.0 / 120.0;
    double start_angle_rad = 1.0471975512;
    double start_angular_velocity_radps = 0.0;
    Integrator integrator = Integrator::SemiImplicitEuler;
    PendulumEquation equation = PendulumEquation::Nonlinear;
};

struct PendulumSample {
    double time_s = 0.0;
    double angle_rad = 0.0;
    double angular_velocity_radps = 0.0;
    double bob_x_m = 0.0;
    double bob_y_m = 0.0;
    double kinetic_energy_j = 0.0;
    double potential_energy_j = 0.0;
    double total_energy_j = 0.0;
};

class PendulumLab {
public:
    explicit PendulumLab(PendulumConfig config);

    void reset();
    void step();

    const PendulumConfig& config() const { return config_; }
    void set_config(PendulumConfig config);

    const PendulumSample& latest() const { return samples_.back(); }
    const std::vector<PendulumSample>& samples() const { return samples_; }

private:
    double angular_acceleration_radps2() const;
    PendulumSample make_sample() const;

    PendulumConfig config_;
    double angle_rad_ = 0.0;
    double angular_velocity_radps_ = 0.0;
    double time_s_ = 0.0;
    std::vector<PendulumSample> samples_;
};

} // namespace physics
