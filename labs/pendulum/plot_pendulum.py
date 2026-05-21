#!/usr/bin/env python3

import csv
import html
from pathlib import Path


WIDTH = 1100
HEIGHT = 700
PAD_LEFT = 80
PAD_RIGHT = 40
PAD_TOP = 50
PAD_BOTTOM = 80


def read_samples(path):
    with path.open(newline="") as file:
        return [
            {
                "time_s": float(row["time_s"]),
                "angle_rad": float(row["angle_rad"]),
                "total_energy_j": float(row["total_energy_j"]),
            }
            for row in csv.DictReader(file)
        ]


def bounds(series, x_key, y_key):
    xs = [point[x_key] for samples in series.values() for point in samples]
    ys = [point[y_key] for samples in series.values() for point in samples]
    return min(xs), max(xs), min(ys), max(ys)


def scaler(x_min, x_max, y_min, y_max):
    if x_min == x_max:
        x_max = x_min + 1.0
    if y_min == y_max:
        y_max = y_min + 1.0

    plot_w = WIDTH - PAD_LEFT - PAD_RIGHT
    plot_h = HEIGHT - PAD_TOP - PAD_BOTTOM

    def scale(x, y):
        sx = PAD_LEFT + (x - x_min) / (x_max - x_min) * plot_w
        sy = HEIGHT - PAD_BOTTOM - (y - y_min) / (y_max - y_min) * plot_h
        return sx, sy

    return scale


def polyline(samples, x_key, y_key, scale):
    return " ".join(f"{scale(sample[x_key], sample[y_key])[0]:.2f},{scale(sample[x_key], sample[y_key])[1]:.2f}" for sample in samples)


def write_svg(path, title, series, x_key, y_key, x_label, y_label, include_zero):
    x_min, x_max, y_min, y_max = bounds(series, x_key, y_key)
    if include_zero:
        y_min = min(y_min, 0.0)
        y_max = max(y_max, 0.0)

    scale = scaler(x_min, x_max, y_min, y_max)

    colors = {
        "nonlinear": "#2563eb",
        "small_angle": "#dc2626",
    }

    axis_x1, axis_y1 = scale(x_min, y_min)
    axis_x2, _ = scale(x_max, y_min)
    _, axis_y2 = scale(x_min, y_max)

    parts = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{WIDTH}" height="{HEIGHT}" viewBox="0 0 {WIDTH} {HEIGHT}">',
        '<rect width="100%" height="100%" fill="#f8fafc"/>',
        f'<text x="{PAD_LEFT}" y="32" font-family="Arial, sans-serif" font-size="24" fill="#111827">{html.escape(title)}</text>',
        f'<line x1="{axis_x1:.2f}" y1="{axis_y1:.2f}" x2="{axis_x2:.2f}" y2="{axis_y1:.2f}" stroke="#374151" stroke-width="2"/>',
        f'<line x1="{axis_x1:.2f}" y1="{axis_y1:.2f}" x2="{axis_x1:.2f}" y2="{axis_y2:.2f}" stroke="#374151" stroke-width="2"/>',
        f'<text x="{WIDTH / 2:.2f}" y="{HEIGHT - 28}" text-anchor="middle" font-family="Arial, sans-serif" font-size="16" fill="#374151">{html.escape(x_label)}</text>',
        f'<text x="22" y="{HEIGHT / 2:.2f}" text-anchor="middle" transform="rotate(-90 22 {HEIGHT / 2:.2f})" font-family="Arial, sans-serif" font-size="16" fill="#374151">{html.escape(y_label)}</text>',
        f'<text x="{PAD_LEFT}" y="{HEIGHT - 54}" font-family="Arial, sans-serif" font-size="13" fill="#6b7280">{x_min:.2f}</text>',
        f'<text x="{WIDTH - PAD_RIGHT}" y="{HEIGHT - 54}" text-anchor="end" font-family="Arial, sans-serif" font-size="13" fill="#6b7280">{x_max:.2f}</text>',
        f'<text x="{PAD_LEFT - 12}" y="{axis_y1:.2f}" text-anchor="end" dominant-baseline="middle" font-family="Arial, sans-serif" font-size="13" fill="#6b7280">{y_min:.2f}</text>',
        f'<text x="{PAD_LEFT - 12}" y="{axis_y2:.2f}" text-anchor="end" dominant-baseline="middle" font-family="Arial, sans-serif" font-size="13" fill="#6b7280">{y_max:.2f}</text>',
    ]

    legend_y = 62
    for index, (name, samples) in enumerate(series.items()):
        color = colors.get(name, "#111827")
        parts.append(
            f'<polyline points="{polyline(samples, x_key, y_key, scale)}" fill="none" stroke="{color}" stroke-width="3" stroke-linejoin="round" stroke-linecap="round"/>'
        )
        parts.append(f'<rect x="{WIDTH - 230}" y="{legend_y + index * 28}" width="18" height="4" fill="{color}"/>')
        parts.append(
            f'<text x="{WIDTH - 202}" y="{legend_y + 5 + index * 28}" font-family="Arial, sans-serif" font-size="15" fill="#111827">{html.escape(name)}</text>'
        )

    parts.append("</svg>")
    path.write_text("\n".join(parts) + "\n")


def main():
    build = Path("build")
    nonlinear = read_samples(build / "pendulum_nonlinear.csv")
    small_angle = read_samples(build / "pendulum_small_angle.csv")
    series = {"nonlinear": nonlinear, "small_angle": small_angle}

    write_svg(build / "pendulum_angle.svg", "Pendulum Angle", series, "time_s", "angle_rad", "time (s)", "angle (rad)", True)
    write_svg(build / "pendulum_energy.svg", "Pendulum Energy", series, "time_s", "total_energy_j", "time (s)", "total energy (J)", True)
    print("Wrote build/pendulum_angle.svg")
    print("Wrote build/pendulum_energy.svg")


if __name__ == "__main__":
    main()

