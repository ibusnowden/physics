#pragma once

#include "physics/mechanics/particle.hpp"

namespace physics {

enum class Integrator {
    ExplicitEuler,
    SemiImplicitEuler,
    RungeKutta4,
};

inline const char* name_of(Integrator integrator) {
    switch (integrator) {
    case Integrator::ExplicitEuler:
        return "explicit_euler";
    case Integrator::SemiImplicitEuler:
        return "semi_implicit_euler";
    case Integrator::RungeKutta4:
        return "runge_kutta_4";
    }
    return "unknown";
}

inline void integrate(Particle& particle, double dt_s, Integrator integrator) {
    const Vec2 acceleration_mps2 = particle.force_n / particle.mass_kg;

    switch (integrator) {
    case Integrator::ExplicitEuler:
        particle.position_m += particle.velocity_mps * dt_s;
        particle.velocity_mps += acceleration_mps2 * dt_s;
        break;
    case Integrator::SemiImplicitEuler:
        particle.velocity_mps += acceleration_mps2 * dt_s;
        particle.position_m += particle.velocity_mps * dt_s;
        break;
    case Integrator::RungeKutta4:
        particle.position_m += particle.velocity_mps * dt_s + acceleration_mps2 * (0.5 * dt_s * dt_s);
        particle.velocity_mps += acceleration_mps2 * dt_s;
        break;
    }
}

} // namespace physics
