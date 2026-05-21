# Projectile Lab

Mechanics lab for projectile motion under gravity, optional quadratic drag,
integrator comparison, and energy diagnostics.

## Run

```sh
make console
./build/projectile_console
```

## Plot

```sh
make plots
```

Outputs:

```text
build/projectile_ideal.csv
build/projectile_drag.csv
build/projectile_trajectory.svg
build/projectile_energy.svg
```

## GUI

```sh
make raylib
./build/projectile_raylib
```

Controls:

- `Space`: pause/resume
- `R`: reset
- `D`: toggle drag
- `1`: explicit Euler
- `2`: semi-implicit Euler

