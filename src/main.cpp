#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Circle {
    float radius;
    float mass;

    float x;
    float y;

    float dx;
    float dy;

    Color color{GRAY};
};

struct Pad {
    float x;
    float width;
    float height;
    float boost;
};

constexpr float ddy = 0.003f;

static void ResolveCollision(Circle &a, Circle &b) {
    float nx = b.x - a.x;
    float ny = b.y - a.y;
    float dist = std::sqrt(nx * nx + ny * ny);
    if (dist == 0.f) return;

    float overlap = (a.radius + b.radius) - dist;
    if (overlap <= 0.f) return;

    nx /= dist;
    ny /= dist;

    a.x -= nx * overlap * 0.5f;
    a.y -= ny * overlap * 0.5f;
    b.x += nx * overlap * 0.5f;
    b.y += ny * overlap * 0.5f;

    float relVelX = b.dx - a.dx;
    float relVelY = b.dy - a.dy;
    float sepVel = relVelX * nx + relVelY * ny;
    if (sepVel > 0.f) return;

    float e = 1.f;
    float j = -(1.f + e) * sepVel / (1.f / a.mass + 1.f / b.mass);

    float impulseX = j * nx;
    float impulseY = j * ny;

    a.dx -= impulseX / a.mass;
    a.dy -= impulseY / a.mass;
    b.dx += impulseX / b.mass;
    b.dy += impulseY / b.mass;
}

static void ApplyPadBoost(Circle &c, const Pad &pad, int screenHeight) {
    float padCenterX = pad.x + pad.width * 0.5f;
    float padCenterY = static_cast<float>(screenHeight);
    float rx = pad.width * 0.5f;
    float ry = pad.height;

    float dx = c.x - padCenterX;
    if (std::abs(dx) > rx + c.radius) return;

    float normalized = (dx * dx) / (rx * rx);
    if (normalized > 1.f) return;

    float yBoundary = padCenterY - ry * std::sqrt(std::max(0.f, 1.f - normalized));
    float circleBottom = c.y + c.radius;
    if (circleBottom < yBoundary) return;

    if (c.dy > 0.f) {
        c.dy = -std::max(pad.boost, std::abs(c.dy));
        c.y = yBoundary - c.radius;
    }
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    int screenWidth = 600;
    int screenHeight = 300;
    InitWindow(screenWidth, screenHeight, "We are Charlie Kirk!");

    Circle grey{
        .radius = 20.f,
        .mass = 1.f,
        .x = 150.f,
        .y = 130.f,
        .dx = 0.6f,
        .dy = 0.f,
    };

    Circle white{
        .radius = 20.f,
        .mass = 1.f,
        .x = 400.f,
        .y = 120.f,
        .dx = -0.4f,
        .dy = 0.f,
        .color = WHITE,
    };

    Circle blue{
        .radius = 15.f,
        .mass = 0.5625f,
        .x = 300.f,
        .y = 50.f,
        .dx = 0.2f,
        .dy = 0.f,
        .color = BLUE,
    };

    Pad yellowPad{
        .x = 10.f,
        .width = 40.f,
        .height = 10.f,
        .boost = 0.9f,
    };

    std::vector<Circle> circles{grey, white, blue};

    float dampening = 0.98f;

    while (!IsWindowReady()) {}

    while (!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        auto UpdateCircle = [&](Circle &c) {
            c.x += c.dx;
            c.y += c.dy;
            c.dy += ddy;

            if (c.x >= screenWidth - c.radius || c.x <= c.radius) {
                c.dx *= -1.f;
            }
            if (c.y >= screenHeight - c.radius) {
                c.dy *= -1.f * dampening;
                c.y = screenHeight - c.radius;
            }
            if (c.y <= c.radius) {
                c.dy *= -1.f;
                c.y = c.radius;
            }
        };

        for (auto &circle : circles) {
            UpdateCircle(circle);
        }
        for (size_t i = 0; i < circles.size(); ++i) {
            for (size_t j = i + 1; j < circles.size(); ++j) {
                ResolveCollision(circles[i], circles[j]);
            }
        }
        for (auto &circle : circles) {
            ApplyPadBoost(circle, yellowPad, screenHeight);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (const auto &circle : circles) {
            DrawCircle(circle.x, circle.y, circle.radius, circle.color);
        }

        float padCenterX = yellowPad.x + yellowPad.width * 0.5f;
        DrawEllipse(padCenterX, static_cast<float>(screenHeight), yellowPad.width * 0.5f, yellowPad.height, YELLOW);

        DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 20, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
