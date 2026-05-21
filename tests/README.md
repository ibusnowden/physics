# Tests

Regression tests live here.

Run:

```sh
make test
```

Current checks:

- RK4 projectile motion matches the analytic no-drag solution.
- Quadratic drag removes mechanical energy.
- Small-angle pendulum follows the linear cosine solution.
