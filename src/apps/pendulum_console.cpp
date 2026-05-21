#include "physics/pendulum_lab.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

void write_csv(const std::string& path, const physics::PendulumLab& lab) {
    std::ofstream file(path);
    file << "time_s,angle_rad,angular_velocity_radps,bob_x_m,bob_y_m,kinetic_energy_j,potential_energy_j,total_energy_j\n";
    for (const auto& sample : lab.samples()) {
        file << std::fixed << std::setprecision(6)
             << sample.time_s << ","
             << sample.angle_rad << ","
             << sample.angular_velocity_radps << ","
             << sample.bob_x_m << ","
             << sample.bob_y_m << ","
             << sample.kinetic_energy_j << ","
             << sample.potential_energy_j << ","
             << sample.total_energy_j << "\n";
    }
}

void run_lab(const std::string& label, physics::PendulumConfig config, const std::string& csv_path) {
    physics::PendulumLab lab(config);
    const double initial_energy = lab.latest().total_energy_j;
    const int steps = static_cast<int>(12.0 / config.timestep_s);
    const int print_every = static_cast<int>(1.0 / config.timestep_s);

    std::cout << "\nScenario: " << label << "\n";
    std::cout << "Equation: " << physics::name_of(config.equation) << "\n";
    std::cout << "Integrator: " << physics::name_of(config.integrator) << "\n";
    std::cout << "time_s,angle_rad,omega_radps,total_energy_j,energy_error_j\n";

    for (int i = 0; i <= steps; ++i) {
        const auto& sample = lab.latest();
        const double energy_error = sample.total_energy_j - initial_energy;

        if (i % print_every == 0) {
            std::cout << std::fixed << std::setprecision(4)
                      << sample.time_s << ","
                      << sample.angle_rad << ","
                      << sample.angular_velocity_radps << ","
                      << sample.total_energy_j << ","
                      << energy_error << "\n";
        }

        if (i < steps) {
            lab.step();
        }
    }

    write_csv(csv_path, lab);
    std::cout << "Wrote " << csv_path << "\n";
}

} // namespace

int main() {
    physics::PendulumConfig nonlinear;
    nonlinear.equation = physics::PendulumEquation::Nonlinear;
    nonlinear.integrator = physics::Integrator::SemiImplicitEuler;

    physics::PendulumConfig small_angle = nonlinear;
    small_angle.equation = physics::PendulumEquation::SmallAngle;

    run_lab("pendulum_nonlinear", nonlinear, "build/pendulum_nonlinear.csv");
    run_lab("pendulum_small_angle", small_angle, "build/pendulum_small_angle.csv");
    return 0;
}

