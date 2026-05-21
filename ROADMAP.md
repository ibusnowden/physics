# Physics Lab Roadmap

This project is a physics learning sandbox, not a production game engine or a
state-of-the-art research solver. The goal is to make each equation visible,
testable, and connected to a small experiment.

## 1. Mechanics

Current focus.

- Projectile motion
- Gravity and drag
- Energy diagnostics
- Pendulum
- Integrator comparison
- RK4
- Analytic projectile comparison
- Regression tests
- Springs
- Orbits
- Double pendulum
- Momentum diagnostics
- Collision experiments

## 2. Waves

- 1D vibrating string
- Mass-spring chain
- Damping and driving forces
- Resonance
- Fourier view

## 3. Fields / Electromagnetism

- Electric fields from point charges
- Field line visualization
- Charged particle motion in electric and magnetic fields
- Simple Maxwell visualizations

## 4. Thermal / Statistical Mechanics

- Gas particles in a box
- Maxwell-Boltzmann distribution
- Random walk
- Ising model
- Monte Carlo experiments

## 5. Quantum Toy Models

- 1D Schrodinger equation
- Wave packet in a potential well
- Tunneling
- Probability density animation

## Engineering Priorities

- Keep the physics core independent from rendering.
- Prefer visible units and reproducible experiments.
- Validate against analytic solutions when available.
- Track energy, momentum, and numerical error.
- Add dependencies only when they directly help learning or visualization.
