# Physics Lab

A small C++ learning sandbox for physics experiments.

The goal is to write the physics ourselves and keep graphics/tooling thin. The
first lab is projectile motion, with explicit Euler and semi-implicit Euler so
we can see how numerical integration affects energy and trajectories. RK4 is
also available for higher-accuracy comparisons.

## Build

This repo starts with a dependency-free console target:

```sh
make console
./build/projectile_console
```

The console app also writes CSV files:

```text
build/projectile_ideal.csv
build/projectile_drag.csv
build/pendulum_nonlinear.csv
build/pendulum_small_angle.csv
```

To generate quick visual plots without raylib:

```sh
make plots
```

This writes:

```text
build/projectile_trajectory.svg
build/projectile_energy.svg
build/pendulum_angle.svg
build/pendulum_energy.svg
```

To run regression tests:

```sh
make test
```

There is also a raylib graphical lab:

```sh
make raylib
```

The raylib target expects raylib to be installed locally. On macOS, that is
usually:

```sh
brew install raylib
```

## Project Shape

```text
include/physics/math/        vector and math types
include/physics/mechanics/   mechanics models
include/physics/numerics/    integrators and numerical methods
src/physics/mechanics/       mechanics implementation
labs/projectile/             projectile apps, plots, and notes
labs/pendulum/               pendulum apps, plots, and notes
notes/curriculum.md          curriculum map
notes/mechanics/             mechanics study notes
tests/                       regression tests
build/                       generated binaries, CSVs, and plots
```

See [ROADMAP.md](ROADMAP.md) for the long-term curriculum and project sequence.

## First Experiments

1. Change projectile gravity and initial velocity.
2. Compare explicit Euler vs semi-implicit Euler.
3. Track total mechanical energy over time.
4. Reduce and increase the timestep to see numerical error.
5. Compare nonlinear pendulum motion with the small-angle approximation.
6. Compare RK4 projectile motion against the analytic no-drag solution.
