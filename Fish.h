#pragma once
#include "raylib.h"
#include <vector>

struct RaylibConfig {
	float containerHeight{};
	float containerWidth{};
	float containerDepth{};
	float MarginX{};
	float MarginY{};
	float MarginZ{};
	float deltaTime{};
};

struct SimulationConfig {
	float seperationRadius{};
	float seperationFactor{};
	float alignmentRadius{};
	float alignmentFactor{};
	float cohesionRadius{};
	float cohesionFactor{};
	float turnFactor{};
	float maxSpeed{};
	float minSpeed{};
};

class Fish
{
private:
	Vector3 m_position{};
	Vector3 m_velocity{};
	Color m_color{};
	static SimulationConfig m_simulationConfig;
	static RaylibConfig m_raylibConfig;

public:
	Fish(Vector3 pos, Vector3 vel);
	Vector3 getPosition() const;
	Vector3 getVelocity() const;
	Color getColor() const;
	void update(const std::vector<Fish>& birds);
	static void setSimulationConfig(SimulationConfig simConfig);
	static void setRaylibConfig(RaylibConfig raylibConfig);
};