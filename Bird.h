#pragma once
#include "raylib.h"
#include <vector>

struct RaylibConfig {
	int screenWidth{};
	int screenHeight{};
	int screenMarginX{};
	int screenMarginY{};
	float deltaTime{};
};

struct SimulationConfig {
	int seperationRadius{};
	float seperationFactor{};
	int alignmentRadius{};
	float alignmentFactor{};
	int cohesionRadius{};
	float cohesionFactor{};
	float turnFactor{};
	float maxSpeed{};
	float minSpeed{};
};

class Bird
{
private:
	Vector2 m_position{};
	Vector2 m_velocity{};
	static SimulationConfig m_simulationConfig;
	static RaylibConfig m_raylibConfig;

public:
	Bird(Vector2 pos, Vector2 vel);
	Vector2 getPosition() const;
	Vector2 getVelocity() const;
	void update(const std::vector<Bird>& birds);
	static void setSimulationConfig(SimulationConfig simConfig);
	static void setRaylibConfig(RaylibConfig raylibConfig);
};