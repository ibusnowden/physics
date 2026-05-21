#include "physics/projectile_lab.hpp"

#include <raylib.h>

#include <algorithm>
#include <string>

namespace {

constexpr int screen_width = 1100;
constexpr int screen_height = 720;
constexpr float pixels_per_meter = 18.0f;

Vector2 to_screen(physics::Vec2 meters) {
    return {
        80.0f + static_cast<float>(meters.x) * pixels_per_meter,
        static_cast<float>(screen_height - 80) - static_cast<float>(meters.y) * pixels_per_meter,
    };
}

void draw_path(const physics::ProjectileLab& lab) {
    const auto& samples = lab.samples();
    for (std::size_t i = 1; i < samples.size(); ++i) {
        DrawLineV(to_screen(samples[i - 1].position_m), to_screen(samples[i].position_m), Fade(SKYBLUE, 0.75f));
    }
}

} // namespace

int main() {
    InitWindow(screen_width, screen_height, "Physics Lab - Projectile Motion");
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

        BeginDrawing();
        ClearBackground({245, 246, 248, 255});

        DrawLine(60, screen_height - 80, screen_width - 40, screen_height - 80, DARKGRAY);
        DrawLine(80, 40, 80, screen_height - 60, DARKGRAY);

        draw_path(lab);
        DrawCircleV(to_screen(lab.particle().position_m), 10.0f, MAROON);

        const auto& sample = lab.latest();
        DrawText("Projectile Motion Lab", 28, 24, 26, BLACK);
        DrawText("Space: pause/resume   R: reset   D: drag   1: explicit Euler   2: semi-implicit Euler", 28, 58, 18, DARKGRAY);

        const std::string integrator = std::string("Integrator: ") + physics::name_of(config.integrator);
        const std::string drag = "Drag c: " + std::to_string(config.drag_coefficient_kg_per_m).substr(0, 5) + " kg/m";
        const std::string time = "Time: " + std::to_string(sample.time_s).substr(0, 5) + " s";
        const std::string energy = "Energy: " + std::to_string(sample.total_energy_j).substr(0, 8) + " J";

        DrawText(integrator.c_str(), 28, 96, 20, BLACK);
        DrawText(drag.c_str(), 28, 124, 20, BLACK);
        DrawText(time.c_str(), 28, 152, 20, BLACK);
        DrawText(energy.c_str(), 28, 180, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
