#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "Bird.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(void)
{
	const int fps = 120;
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	const int marginX = 200;
	const int marginY = 100;
	const int seperationRadius = 80;
	const float seprationFactor = 0.2f;
	const int alignmentRadius = 100;
	const float alignmentFactor = 0.075f;
	const int cohesionRadius = 90;
	const float cohesionFactor = 0.1f;
	const float turnFactor = 20.0f;
	const float maxSpeed = 500.0f;
	const float minSpeed = 100.0f;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, "Boids");
	SetTargetFPS(fps);

	std::vector<Bird> birds;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	bool seperationToggle = false;
	bool alignmentToggle = false;
	bool cohesionToggle = false;
	while (!WindowShouldClose())
	{
		// Set simulation config
		Bird::setRaylibConfig(RaylibConfig{ screenWidth, screenHeight, marginX, marginY, GetFrameTime() });
		Bird::setSimulationConfig(
			SimulationConfig
			{
				seperationRadius,
				seprationFactor,
				alignmentRadius,
				alignmentFactor,
				cohesionRadius,
				cohesionFactor,
				turnFactor,
				maxSpeed,
				minSpeed
			}
		);

		// Create bird
		Vector2 mouse = GetMousePosition();
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			const float speedScale = 300.0;
			float randomFloat1 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float randomFloat2 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			Vector2 initVelocity = Vector2Scale(Vector2Normalize(Vector2{ randomFloat1, randomFloat2 }), speedScale);
			Bird b{ mouse, initVelocity };
			birds.push_back(b);
		};

		// Draw birds
		BeginDrawing();
		ClearBackground(RAYWHITE);
		for (Bird& b : birds)
		{
			Vector2 pos = b.getPosition();
			Vector2 vel = b.getVelocity();

			float angle = atan2(vel.y, vel.x);
			const float triangleSize = 10.0f; // control size
			const float triangleAngle = 7 * PI / 9; // control shape
			Vector2 base = Vector2Scale(Vector2{ cos(angle), sin(angle) }, triangleSize);
			Vector2 vertex1 = Vector2Add(pos, base);
			Vector2 vertex2 = Vector2Add(pos, Vector2Rotate(base, -triangleAngle));
			Vector2 vertex3 = Vector2Add(pos, Vector2Rotate(base, triangleAngle));
			DrawTriangle(vertex1, vertex2, vertex3, DARKGREEN);
			b.update(birds);
		}

		// Draw util
		DrawFPS(30, 30);
		if (seperationToggle && !birds.empty()) DrawCircleLinesV(birds[0].getPosition(), seperationRadius, RED);
		if (alignmentToggle && !birds.empty()) DrawCircleLinesV(birds[0].getPosition(), alignmentRadius, BLUE);
		if (cohesionToggle && !birds.empty()) DrawCircleLinesV(birds[0].getPosition(), cohesionRadius, BEIGE);

		if (IsKeyReleased(KEY_F1)) seperationToggle = !seperationToggle;
		if (IsKeyReleased(KEY_F2)) alignmentToggle = !alignmentToggle;
		if (IsKeyReleased(KEY_F3)) cohesionToggle = !cohesionToggle;
		EndDrawing();
	}

	CloseWindow();
	return 0;
}