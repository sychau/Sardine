#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#include "raylib.h"
#include "raymath.h"
#include "Fish.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(void)
{
	const int fps = 120;
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	const float containerSize = 50.0f;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, "Sardine simulator");
	SetTargetFPS(fps);
	DisableCursor();

	// Define camera
	Camera3D camera{ 0 };
	camera.position = Vector3{ containerSize * 1.0f, containerSize * 1.0f, containerSize * 1.1f }; // Camera position
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };         // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };             // Camera up vector (rotation towards target)
	camera.fovy = 65.0f;                                 // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;              // Camera projection type

	// Load model
	Model sardine = LoadModel("Assets/Sardine/sardine.obj");
	Texture2D texture = LoadTexture("Assets/Sardine/sardine.png");

	// Load shader for model
	// NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
	Shader shader = LoadShader(0, TextFormat("Assets/shaders/glsl%i/grayscale.fs", GLSL_VERSION));
	sardine.materials[0].shader = shader;                     // Set shader effect to 3d model
	sardine.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Bind texture to model


	// Create lights
	std::vector<Fish> fishes;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	bool seperationToggle = false;
	bool alignmentToggle = false;
	bool cohesionToggle = false;
	const Vector3 cubePosition{ 0.0f, 0.0f, 0.0f };
	while (!WindowShouldClose())
	{
		// Set simulation config
		
		const float marginX = 5.0f;
		const float marginY = 5.0f;
		const float marginZ = 5.0f;
		Fish::setRaylibConfig(RaylibConfig{ containerSize, containerSize, containerSize, marginX, marginY, marginZ, GetFrameTime() });

		const float seperationRadius = containerSize * 0.08f;
		const float seprationFactor = 0.5f;
		const float alignmentRadius = containerSize * 0.09f;
		const float alignmentFactor = 0.05f;
		const float cohesionRadius = containerSize * 0.07f;
		const float cohesionFactor = 0.3f;
		const float turnFactor = 5.0f;
		const float maxSpeed = 50.0f;
		const float minSpeed = 20.0f;
		Fish::setSimulationConfig(
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

		UpdateCamera(&camera, CAMERA_FREE);

		// Create fish
		if (IsKeyDown('B'))
		{
			const float speedScale = containerSize * 0.2f;
			float randomFloat1 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float randomFloat2 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float randomFloat3 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			Vector3 initVelocity = Vector3Scale(Vector3Normalize(Vector3{ randomFloat1, randomFloat2, randomFloat3 }), speedScale);
			Fish fish{ Vector3{0.0f, 0.0f, 0.0f}, initVelocity };
			fishes.push_back(fish);
		};

		// Utils
		if (IsKeyPressed('Z')) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
		if (IsKeyPressed('K')) fishes.clear();
		if (IsKeyReleased(KEY_F1)) seperationToggle = !seperationToggle;
		if (IsKeyReleased(KEY_F2)) alignmentToggle = !alignmentToggle;
		if (IsKeyReleased(KEY_F3)) cohesionToggle = !cohesionToggle;

		// Draw fishes
		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode3D(camera);
		DrawCubeWires(cubePosition, containerSize, containerSize, containerSize, RAYWHITE);
		for (Fish& fish : fishes)
		{
			Vector3 normalVel = Vector3Normalize(fish.getVelocity());
			Quaternion q = QuaternionFromVector3ToVector3(Vector3{ 0, 0, -1.0f }, normalVel);
			Vector3 rotationAxis;
			float rotationAngleRad;
			QuaternionToAxisAngle(q, &rotationAxis, &rotationAngleRad);

			float rotationAngleDeg = rotationAngleRad * 180.0f / PI;
			const Vector3 modelScale { containerSize * 0.35f, containerSize * 0.35f, containerSize * 0.35f };
			DrawModelEx(sardine, fish.getPosition(), rotationAxis, rotationAngleDeg, modelScale, WHITE);
			fish.update(fishes);
		}

		// Draw 3D UI
		if (seperationToggle && !fishes.empty())
		{
			Color red = RED;
			red.a = 128.0f;
			DrawSphere(fishes[0].getPosition(), seperationRadius, red);
		}
		if (alignmentToggle && !fishes.empty())
		{
			Color green = GREEN;
			green.a = 128.0f;
			DrawSphere(fishes[0].getPosition(), alignmentRadius, green);
		}
		if (cohesionToggle && !fishes.empty())
		{
			Color blue = BLUE;
			blue.a = 128.0f;
			DrawSphere(fishes[0].getPosition(), cohesionRadius, blue);
		}
		EndMode3D();

		// Draw 2D UI
		DrawText(TextFormat("%d", GetFPS()), screenWidth - 30, screenHeight - 30, 20, GREEN);
		DrawText(TextFormat("x: %.2f y:%.2f z:%.2f", camera.position.x, camera.position.y, camera.position.z), 10, 10, 20, DARKGRAY);
		DrawText("WASD: move", 10, 40, 20, RAYWHITE);
		DrawText("B: spawn fish", 10, 60, 20, RAYWHITE);
		DrawText("K: remove fishes", 10, 80, 20, RAYWHITE);
		DrawText("Z: reset camera", 10, 100, 20, RAYWHITE);
		DrawText("F1: toggle seperation radius", 10, 120, 20, RAYWHITE);
		DrawText("F2: toggle alignment radius", 10, 140, 20, RAYWHITE);
		DrawText("F3: toggle cohesion radius", 10, 160, 20, RAYWHITE);
		EndDrawing();
	}
	UnloadModel(sardine);
	UnloadTexture(texture);
	UnloadShader(shader);
	CloseWindow();
	return 0;
}