#include "physics/mechanics/particle.hpp"

namespace physics {

void clear_forces(Particle& particle) {
    particle.force_n = {};
}

void add_force(Particle& particle, Vec2 force_n) {
    particle.force_n += force_n;
}

void add_gravity(Particle& particle, Vec2 gravity_mps2) {
    add_force(particle, gravity_mps2 * particle.mass_kg);
}

void add_quadratic_drag(Particle& particle, double drag_coefficient_kg_per_m) {
    const double speed_mps = length(particle.velocity_mps);
    if (speed_mps == 0.0 || drag_coefficient_kg_per_m == 0.0) {
        return;
    }

    add_force(particle, particle.velocity_mps * (-drag_coefficient_kg_per_m * speed_mps));
}

double kinetic_energy_j(const Particle& particle) {
    return 0.5 * particle.mass_kg * dot(particle.velocity_mps, particle.velocity_mps);
}

double gravitational_potential_energy_j(const Particle& particle, double gravity_mps2) {
    return particle.mass_kg * gravity_mps2 * particle.position_m.y;
}

} // namespace physics
