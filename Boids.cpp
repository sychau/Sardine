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

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, "Boids");
	SetTargetFPS(fps);
	DisableCursor();

	// Define camera
	Camera3D camera{ 0 };
	camera.position = Vector3{ 120.0f, 120.0f, 140.0f }; // Camera position
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };         // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };             // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                 // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;              // Camera projection type

	std::vector<Bird> birds;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	bool seperationToggle = false;
	bool alignmentToggle = false;
	bool cohesionToggle = false;
	const Vector3 cubePosition{ 0.0f, 0.0f, 0.0f };
	while (!WindowShouldClose())
	{
		// Set simulation config
		const float containerSize = 100.0f;
		const float marginX = 5.0f;
		const float marginY = 5.0f;
		const float marginZ = 5.0f;
		Bird::setRaylibConfig(RaylibConfig{ containerSize, containerSize, containerSize, marginX, marginY, marginZ, GetFrameTime() });

		const float seperationRadius = 8.0f;
		const float seprationFactor = 0.5f;
		const float alignmentRadius = 9.0f;
		const float alignmentFactor = 0.05f;
		const float cohesionRadius = 10.0f;
		const float cohesionFactor = 0.3f;
		const float turnFactor = 5.0f;
		const float maxSpeed = 50.0f;
		const float minSpeed = 20.0f;
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
		if (IsKeyDown('B'))
		{
			const float speedScale = 20.0f;
			float randomFloat1 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float randomFloat2 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float randomFloat3 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			Vector3 initVelocity = Vector3Scale(Vector3Normalize(Vector3{ randomFloat1, randomFloat2, randomFloat3 }), speedScale);
			unsigned char r = std::rand() % 256;
			unsigned char g = std::rand() % 256;
			unsigned char b = std::rand() % 256;
			Bird bird{ Vector3{0.0f, 0.0f, 0.0f}, initVelocity, Color{r, g, b, 255} };
			birds.push_back(bird);
		};

		UpdateCamera(&camera, CAMERA_FREE);

		// Utils
		if (IsKeyPressed('Z')) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
		if (IsKeyPressed('K')) birds.clear();
		if (IsKeyReleased(KEY_F1)) seperationToggle = !seperationToggle;
		if (IsKeyReleased(KEY_F2)) alignmentToggle = !alignmentToggle;
		if (IsKeyReleased(KEY_F3)) cohesionToggle = !cohesionToggle;

		// Draw birds
		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode3D(camera);
		DrawCubeWires(cubePosition, containerSize, containerSize, containerSize, RAYWHITE);
		for (Bird& bird : birds)
		{
			Vector3 normalVel = Vector3Normalize(bird.getVelocity());
			Vector3 startPos = Vector3Add(Vector3Scale(normalVel, 1.0f), bird.getPosition());
			Vector3 endPos = Vector3Add(Vector3Scale(normalVel, -2.0f), bird.getPosition());
			DrawCylinderEx(startPos, endPos, 0, 1.0f, 10, bird.getColor());
			bird.update(birds);
		}
		
		// Draw 3D UI
		if (seperationToggle && !birds.empty())
		{
			Color red = RED;
			red.a = 128.0f;
			DrawSphere(birds[0].getPosition(), seperationRadius, red);
		}
		if (alignmentToggle && !birds.empty())
		{
			Color green = GREEN;
			green.a = 128.0f;
			DrawSphere(birds[0].getPosition(), alignmentRadius, green);
		}
		if (cohesionToggle && !birds.empty())
		{
			Color blue = BLUE;
			blue.a = 128.0f;
			DrawSphere(birds[0].getPosition(), cohesionRadius, blue);
		}
		EndMode3D();

		// Draw 2D UI
		DrawText(TextFormat("%d", GetFPS()), screenWidth - 30, screenHeight - 30, 20, GREEN);
		DrawText(TextFormat("x: %.2f y:%.2f z:%.2f", camera.position.x, camera.position.y, camera.position.z), 10, 10, 20, DARKGRAY);
		DrawText("WASD: move", 10, 40, 20, RAYWHITE);
		DrawText("B: spawn bird", 10, 60, 20, RAYWHITE);
		DrawText("K: remove birds", 10, 80, 20, RAYWHITE);
		DrawText("Z: reset camera", 10, 100, 20, RAYWHITE);
		DrawText("F1: toggle seperation radius", 10, 120, 20, RAYWHITE);
		DrawText("F2: toggle alignment radius", 10, 140, 20, RAYWHITE);
		DrawText("F3: toggle cohesion radius", 10, 160, 20, RAYWHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}