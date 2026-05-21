#pragma once

#include "physics/vec2.hpp"

namespace physics {

struct Particle {
    double mass_kg = 1.0;
    Vec2 position_m;
    Vec2 velocity_mps;
    Vec2 force_n;
};

void clear_forces(Particle& particle);
void add_force(Particle& particle, Vec2 force_n);
void add_gravity(Particle& particle, Vec2 gravity_mps2);
void add_quadratic_drag(Particle& particle, double drag_coefficient_kg_per_m);

double kinetic_energy_j(const Particle& particle);
double gravitational_potential_energy_j(const Particle& particle, double gravity_mps2);

} // namespace physics
