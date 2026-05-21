#include "physics/pendulum_lab.hpp"

#include <cmath>

namespace physics {

const char* name_of(PendulumEquation equation) {
    switch (equation) {
    case PendulumEquation::Nonlinear:
        return "nonlinear";
    case PendulumEquation::SmallAngle:
        return "small_angle";
    }
    return "unknown";
}

PendulumLab::PendulumLab(PendulumConfig config) : config_(config) {
    reset();
}

void PendulumLab::reset() {
    angle_rad_ = config_.start_angle_rad;
    angular_velocity_radps_ = config_.start_angular_velocity_radps;
    time_s_ = 0.0;
    samples_.clear();
    samples_.push_back(make_sample());
}

void PendulumLab::step() {
    const double acceleration_radps2 = angular_acceleration_radps2();

    switch (config_.integrator) {
    case Integrator::ExplicitEuler:
        angle_rad_ += angular_velocity_radps_ * config_.timestep_s;
        angular_velocity_radps_ += acceleration_radps2 * config_.timestep_s;
        break;
    case Integrator::SemiImplicitEuler:
        angular_velocity_radps_ += acceleration_radps2 * config_.timestep_s;
        angle_rad_ += angular_velocity_radps_ * config_.timestep_s;
        break;
    }

    time_s_ += config_.timestep_s;
    samples_.push_back(make_sample());
}

void PendulumLab::set_config(PendulumConfig config) {
    config_ = config;
    reset();
}

double PendulumLab::angular_acceleration_radps2() const {
    const double displacement = config_.equation == PendulumEquation::SmallAngle ? angle_rad_ : std::sin(angle_rad_);
    return -(config_.gravity_mps2 / config_.length_m) * displacement;
}

PendulumSample PendulumLab::make_sample() const {
    const double speed_mps = config_.length_m * angular_velocity_radps_;
    const double kinetic = 0.5 * config_.mass_kg * speed_mps * speed_mps;
    const double potential = config_.equation == PendulumEquation::SmallAngle
                                 ? 0.5 * config_.mass_kg * config_.gravity_mps2 * config_.length_m * angle_rad_ * angle_rad_
                                 : config_.mass_kg * config_.gravity_mps2 * config_.length_m * (1.0 - std::cos(angle_rad_));

    return {
        .time_s = time_s_,
        .angle_rad = angle_rad_,
        .angular_velocity_radps = angular_velocity_radps_,
        .bob_x_m = config_.length_m * std::sin(angle_rad_),
        .bob_y_m = -config_.length_m * std::cos(angle_rad_),
        .kinetic_energy_j = kinetic,
        .potential_energy_j = potential,
        .total_energy_j = kinetic + potential,
    };
}

} // namespace physics

