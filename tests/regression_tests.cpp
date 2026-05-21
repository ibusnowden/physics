#include "physics/mechanics/pendulum_lab.hpp"
#include "physics/mechanics/projectile_lab.hpp"

#include <cmath>
#include <iostream>
#include <string>

namespace {

bool approx(double actual, double expected, double tolerance, const std::string& label) {
    const double error = std::abs(actual - expected);
    if (error <= tolerance) {
        return true;
    }

    std::cerr << "FAIL " << label << ": actual=" << actual
              << " expected=" << expected
              << " error=" << error
              << " tolerance=" << tolerance << "\n";
    return false;
}

bool test_projectile_rk4_matches_analytic_solution() {
    physics::ProjectileConfig config;
    config.integrator = physics::Integrator::RungeKutta4;
    config.drag_coefficient_kg_per_m = 0.0;
    config.timestep_s = 1.0 / 60.0;

    physics::ProjectileLab lab(config);
    for (int i = 0; i < 60; ++i) {
        lab.step();
    }

    const auto& sample = lab.latest();
    const auto analytic = physics::analytic_projectile_sample(config, sample.time_s);

    bool ok = true;
    ok &= approx(sample.position_m.x, analytic.position_m.x, 1e-12, "projectile rk4 x");
    ok &= approx(sample.position_m.y, analytic.position_m.y, 1e-12, "projectile rk4 y");
    ok &= approx(sample.velocity_mps.x, analytic.velocity_mps.x, 1e-12, "projectile rk4 vx");
    ok &= approx(sample.velocity_mps.y, analytic.velocity_mps.y, 1e-12, "projectile rk4 vy");
    return ok;
}

bool test_drag_removes_mechanical_energy() {
    physics::ProjectileConfig config;
    config.integrator = physics::Integrator::RungeKutta4;
    config.drag_coefficient_kg_per_m = 0.025;

    physics::ProjectileLab lab(config);
    const double initial_energy = lab.latest().total_energy_j;
    for (int i = 0; i < 60; ++i) {
        lab.step();
    }

    const double final_energy = lab.latest().total_energy_j;
    if (final_energy < initial_energy) {
        return true;
    }

    std::cerr << "FAIL drag energy: final=" << final_energy
              << " initial=" << initial_energy << "\n";
    return false;
}

bool test_small_angle_pendulum_matches_linear_solution() {
    physics::PendulumConfig config;
    config.equation = physics::PendulumEquation::SmallAngle;
    config.integrator = physics::Integrator::RungeKutta4;
    config.start_angle_rad = 0.1;
    config.start_angular_velocity_radps = 0.0;
    config.timestep_s = 1.0 / 240.0;

    physics::PendulumLab lab(config);
    for (int i = 0; i < 240; ++i) {
        lab.step();
    }

    const auto& sample = lab.latest();
    const double omega = std::sqrt(config.gravity_mps2 / config.length_m);
    const double expected_angle = config.start_angle_rad * std::cos(omega * sample.time_s);

    return approx(sample.angle_rad, expected_angle, 1e-9, "small-angle pendulum theta");
}

} // namespace

int main() {
    bool ok = true;
    ok &= test_projectile_rk4_matches_analytic_solution();
    ok &= test_drag_removes_mechanical_energy();
    ok &= test_small_angle_pendulum_matches_linear_solution();

    if (!ok) {
        return 1;
    }

    std::cout << "All regression tests passed.\n";
    return 0;
}

