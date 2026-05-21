#include "physics/mechanics/projectile_lab.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

void write_csv(const std::string& path, const physics::ProjectileLab& lab) {
    std::ofstream file(path);
    file << "time_s,x_m,y_m,vx_mps,vy_mps,kinetic_energy_j,potential_energy_j,total_energy_j\n";
    for (const auto& sample : lab.samples()) {
        file << std::fixed << std::setprecision(6)
             << sample.time_s << ","
             << sample.position_m.x << ","
             << sample.position_m.y << ","
             << sample.velocity_mps.x << ","
             << sample.velocity_mps.y << ","
             << sample.kinetic_energy_j << ","
             << sample.potential_energy_j << ","
             << sample.total_energy_j << "\n";
    }
}

void run_lab(const std::string& label, physics::ProjectileConfig config, const std::string& csv_path) {
    physics::ProjectileLab lab(config);
    const double initial_energy = lab.latest().total_energy_j;

    std::cout << "\nScenario: " << label << "\n";
    std::cout << "Integrator: " << physics::name_of(config.integrator) << "\n";
    std::cout << "Drag coefficient: " << config.drag_coefficient_kg_per_m << " kg/m\n";
    std::cout << "time_s,x_m,y_m,vx_mps,vy_mps,total_energy_j,energy_error_j\n";

    for (int i = 0; i < 240; ++i) {
        const auto& sample = lab.latest();
        const double energy_error = sample.total_energy_j - initial_energy;

        if (i % 10 == 0 || sample.position_m.y < 0.0) {
            std::cout << std::fixed << std::setprecision(4)
                      << sample.time_s << ","
                      << sample.position_m.x << ","
                      << sample.position_m.y << ","
                      << sample.velocity_mps.x << ","
                      << sample.velocity_mps.y << ","
                      << sample.total_energy_j << ","
                      << energy_error << "\n";
        }

        if (sample.position_m.y < 0.0 && sample.time_s > 0.0) {
            break;
        }

        lab.step();
    }

    write_csv(csv_path, lab);
    std::cout << "Wrote " << csv_path << "\n";
}

} // namespace

int main() {
    physics::ProjectileConfig ideal;
    ideal.integrator = physics::Integrator::SemiImplicitEuler;
    ideal.timestep_s = 1.0 / 60.0;

    physics::ProjectileConfig with_drag = ideal;
    with_drag.drag_coefficient_kg_per_m = 0.025;

    run_lab("ideal_projectile", ideal, "build/projectile_ideal.csv");
    run_lab("projectile_with_quadratic_drag", with_drag, "build/projectile_drag.csv");
    return 0;
}
