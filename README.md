# Physics Lab

A small C++ learning sandbox for physics experiments.

The goal is to write the physics ourselves and keep graphics/tooling thin. The
first lab is projectile motion, with explicit Euler and semi-implicit Euler so
we can see how numerical integration affects energy and trajectories.

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

There is also a placeholder for a graphical lab:

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
include/physics/  public physics headers
src/physics/      physics implementation
src/apps/         runnable experiments
notes/            study notes and lab prompts
```

## First Experiments

1. Change projectile gravity and initial velocity.
2. Compare explicit Euler vs semi-implicit Euler.
3. Track total mechanical energy over time.
4. Reduce and increase the timestep to see numerical error.
5. Compare nonlinear pendulum motion with the small-angle approximation.
