# Lab 001: Projectile Motion

## Question

How does a projectile move under constant gravity, and how much does the
computer's timestep change the answer?

## Model

State:

```text
position = (x, y)
velocity = (vx, vy)
acceleration = (0, -g)
```

Forces:

```text
F_gravity = mass * acceleration
```

Energy:

```text
kinetic = 0.5 * mass * speed^2
potential = mass * g * height
total = kinetic + potential
```

In ideal projectile motion, total mechanical energy should stay constant.
Numerical integration will introduce error, so energy becomes our first
diagnostic.

## Things To Try

1. Set `timestep_s` to `1.0 / 10.0`, `1.0 / 30.0`, and `1.0 / 120.0`.
2. Compare explicit Euler and semi-implicit Euler.
3. Change `start_velocity_mps`.
4. Add quadratic drag:

```text
F_drag = -c * speed * velocity
```

## New Diagnostic

With no drag, total mechanical energy should be almost constant. With drag,
total mechanical energy should decrease because drag removes energy from the
projectile's motion.

The console app writes:

```text
build/projectile_ideal.csv
build/projectile_drag.csv
```

Plot `x_m` vs `y_m` to compare trajectories. Plot `time_s` vs
`total_energy_j` to compare energy behavior.

The `make plots` command generates both as SVG files:

```text
build/projectile_trajectory.svg
build/projectile_energy.svg
```
