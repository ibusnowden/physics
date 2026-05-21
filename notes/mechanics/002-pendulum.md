# Lab 002: Simple Pendulum

## Question

When is the small-angle approximation good enough, and how does the numerical
integrator affect energy conservation?

## Model

State:

```text
theta = angle from vertical, radians
omega = angular velocity, radians / second
```

Nonlinear equation:

```text
theta'' = -(g / L) * sin(theta)
```

Small-angle approximation:

```text
theta'' = -(g / L) * theta
```

The approximation is only accurate when `theta` is small enough that
`sin(theta) approx theta`.

## Energy

Nonlinear:

```text
kinetic = 0.5 * mass * (L * omega)^2
potential = mass * g * L * (1 - cos(theta))
```

Small-angle approximation:

```text
kinetic = 0.5 * mass * (L * omega)^2
potential = 0.5 * mass * g * L * theta^2
```

## Things To Try

1. Set `start_angle_rad` to `0.1`, `0.5`, and `1.0`.
2. Compare nonlinear and small-angle motion.
3. Switch between explicit Euler and semi-implicit Euler.
4. Change `timestep_s` and watch the energy plot.

