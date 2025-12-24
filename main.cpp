#include <iostream>
#include <raylib.h>

struct Circle {
    float radius;
    float mass;

    float x;
    float y;

    float dx;
    float dy;
    
    
};

constexpr float ddy = 0.0001f;

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    std::cout << "Howdy World!" << std::endl;
    int screenWidth = 600;
    int screenHeight = 300;
    InitWindow(screenWidth, screenHeight, "We are Charlie Kirk!");


    float radius = 20.f;
    float x = 100.f;
    float y = 100.f;

    float dx = 0.1f;
    float dy = 0.f;
    float dampening = 0.95f;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        

        DrawCircle(x, y, radius, WHITE);
        // DrawCircle(x - screenWidth, y, radius, WHITE);
        // DrawCircle(x + screenWidth, y, radius, WHITE);
        
        x += dx;

        if (x >= screenWidth - radius) {
            dx *= -1.f;
            // x = screenWidth - radius;
        }
        if (x <= 0 + radius) {
            dx *= -1.f;
            // x = 0;
        }

        y += dy;
        dy += ddy;
        if (y >= screenHeight - radius) {
            dy *= -1.f * dampening;
            y = screenHeight - radius;
        }

        DrawEllipse(20, screenHeight - 20, 20, 20, BLUE);


        EndDrawing();
    }
    CloseWindow();
    return 0;
}
