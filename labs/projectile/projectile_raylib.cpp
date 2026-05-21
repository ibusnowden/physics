#include "physics/mechanics/projectile_lab.hpp"

#include <raylib.h>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr int initial_screen_width = 1280;
constexpr int initial_screen_height = 820;

struct PlotBounds {
    double min_x = 0.0;
    double max_x = 1.0;
    double min_y = 0.0;
    double max_y = 1.0;
};

std::string fixed(double value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

std::vector<physics::ProjectileSample> predict_path(physics::ProjectileConfig config) {
    config.timestep_s = 1.0 / 120.0;
    physics::ProjectileLab preview(config);

    for (int i = 0; i < 1200; ++i) {
        if (preview.latest().position_m.y < 0.0 && preview.latest().time_s > 0.0) {
            break;
        }
        preview.step();
    }

    return preview.samples();
}

PlotBounds compute_bounds(const physics::ProjectileLab& lab, const std::vector<physics::ProjectileSample>& preview) {
    PlotBounds bounds;
    bounds.min_y = -1.0;

    auto include = [&bounds](physics::Vec2 point) {
        bounds.min_x = std::min(bounds.min_x, point.x);
        bounds.max_x = std::max(bounds.max_x, point.x);
        bounds.min_y = std::min(bounds.min_y, point.y);
        bounds.max_y = std::max(bounds.max_y, point.y);
    };

    for (const auto& sample : preview) {
        include(sample.position_m);
    }
    for (const auto& sample : lab.samples()) {
        include(sample.position_m);
    }

    const double width = std::max(1.0, bounds.max_x - bounds.min_x);
    const double height = std::max(1.0, bounds.max_y - bounds.min_y);
    bounds.min_x -= width * 0.04;
    bounds.max_x += width * 0.04;
    bounds.min_y -= height * 0.10;
    bounds.max_y += height * 0.18;
    return bounds;
}

Vector2 to_screen(physics::Vec2 meters, Rectangle plot, PlotBounds bounds) {
    const double world_w = bounds.max_x - bounds.min_x;
    const double world_h = bounds.max_y - bounds.min_y;

    return {
        plot.x + static_cast<float>((meters.x - bounds.min_x) / world_w) * plot.width,
        plot.y + plot.height - static_cast<float>((meters.y - bounds.min_y) / world_h) * plot.height,
    };
}

void draw_polyline(const std::vector<physics::ProjectileSample>& samples, Rectangle plot, PlotBounds bounds, Color color, float thickness) {
    for (std::size_t i = 1; i < samples.size(); ++i) {
        DrawLineEx(to_screen(samples[i - 1].position_m, plot, bounds), to_screen(samples[i].position_m, plot, bounds), thickness, color);
    }
}

void draw_grid(Rectangle plot, PlotBounds bounds) {
    DrawRectangleLinesEx(plot, 1.0f, Fade(DARKGRAY, 0.55f));

    constexpr int grid_x = 8;
    constexpr int grid_y = 5;
    for (int i = 0; i <= grid_x; ++i) {
        const float x = plot.x + plot.width * static_cast<float>(i) / static_cast<float>(grid_x);
        const double world_x = bounds.min_x + (bounds.max_x - bounds.min_x) * static_cast<double>(i) / static_cast<double>(grid_x);
        DrawLineV({x, plot.y}, {x, plot.y + plot.height}, Fade(LIGHTGRAY, 0.6f));
        DrawText(fixed(world_x, 0).c_str(), static_cast<int>(x - 10.0f), static_cast<int>(plot.y + plot.height + 10.0f), 14, GRAY);
    }

    for (int i = 0; i <= grid_y; ++i) {
        const float y = plot.y + plot.height * static_cast<float>(i) / static_cast<float>(grid_y);
        const double world_y = bounds.max_y - (bounds.max_y - bounds.min_y) * static_cast<double>(i) / static_cast<double>(grid_y);
        DrawLineV({plot.x, y}, {plot.x + plot.width, y}, Fade(LIGHTGRAY, 0.6f));
        DrawText(fixed(world_y, 1).c_str(), static_cast<int>(plot.x - 44.0f), static_cast<int>(y - 8.0f), 14, GRAY);
    }

    if (bounds.min_y < 0.0 && bounds.max_y > 0.0) {
        const Vector2 ground_left = to_screen({bounds.min_x, 0.0}, plot, bounds);
        const Vector2 ground_right = to_screen({bounds.max_x, 0.0}, plot, bounds);
        DrawLineEx(ground_left, ground_right, 2.0f, Fade(DARKGRAY, 0.75f));
    }

    DrawText("x (m)", static_cast<int>(plot.x + plot.width - 42.0f), static_cast<int>(plot.y + plot.height + 34.0f), 16, DARKGRAY);
    DrawText("y (m)", static_cast<int>(plot.x - 50.0f), static_cast<int>(plot.y - 26.0f), 16, DARKGRAY);
}

void draw_path(const physics::ProjectileLab& lab, const std::vector<physics::ProjectileSample>& preview, Rectangle plot, PlotBounds bounds) {
    const auto& samples = lab.samples();
    draw_polyline(preview, plot, bounds, Fade(SKYBLUE, 0.35f), 2.0f);
    draw_polyline(samples, plot, bounds, BLUE, 3.0f);
}

} // namespace

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(initial_screen_width, initial_screen_height, "Physics Lab - Projectile Motion");
    SetTargetFPS(60);

    physics::ProjectileConfig config;
    physics::ProjectileLab lab(config);
    bool paused = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }
        if (IsKeyPressed(KEY_R)) {
            lab.reset();
        }
        if (IsKeyPressed(KEY_ONE)) {
            config.integrator = physics::Integrator::ExplicitEuler;
            lab.set_config(config);
        }
        if (IsKeyPressed(KEY_TWO)) {
            config.integrator = physics::Integrator::SemiImplicitEuler;
            lab.set_config(config);
        }
        if (IsKeyPressed(KEY_D)) {
            config.drag_coefficient_kg_per_m = config.drag_coefficient_kg_per_m == 0.0 ? 0.025 : 0.0;
            lab.set_config(config);
        }
        if (!paused && lab.latest().position_m.y >= 0.0) {
            lab.step();
        }

        const int screen_width = GetScreenWidth();
        const int screen_height = GetScreenHeight();
        const Rectangle plot = {
            86.0f,
            210.0f,
            static_cast<float>(screen_width - 132),
            static_cast<float>(screen_height - 288),
        };
        const auto preview = predict_path(config);
        const PlotBounds bounds = compute_bounds(lab, preview);

        BeginDrawing();
        ClearBackground({248, 249, 251, 255});

        draw_grid(plot, bounds);
        draw_path(lab, preview, plot, bounds);

        const Vector2 projectile = to_screen(lab.particle().position_m, plot, bounds);
        DrawCircleV(projectile, 11.0f, ORANGE);
        DrawCircleLines(static_cast<int>(projectile.x), static_cast<int>(projectile.y), 11.0f, MAROON);

        const auto& sample = lab.latest();
        DrawText("Projectile Motion Lab", 32, 24, 24, BLACK);
        DrawText("Space pause/resume   R reset   D drag   1 explicit Euler   2 semi-implicit Euler", 32, 58, 16, DARKGRAY);

        const std::string status = paused ? "paused" : (sample.position_m.y < 0.0 ? "landed" : "running");
        const std::string integrator = std::string("integrator  ") + physics::name_of(config.integrator);
        const std::string drag = "drag c  " + fixed(config.drag_coefficient_kg_per_m, 3) + " kg/m";
        const std::string time = "time  " + fixed(sample.time_s, 3) + " s";
        const std::string position = "position  (" + fixed(sample.position_m.x, 2) + ", " + fixed(sample.position_m.y, 2) + ") m";
        const std::string energy = "energy  " + fixed(sample.total_energy_j, 2) + " J";

        DrawText(status.c_str(), 32, 94, 18, sample.position_m.y < 0.0 ? MAROON : DARKGREEN);
        DrawText(integrator.c_str(), 32, 124, 18, BLACK);
        DrawText(drag.c_str(), 32, 152, 18, BLACK);
        DrawText(time.c_str(), screen_width - 390, 94, 18, BLACK);
        DrawText(position.c_str(), screen_width - 390, 124, 18, BLACK);
        DrawText(energy.c_str(), screen_width - 390, 152, 18, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
