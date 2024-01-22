#include "raylib.h"
#include "raymath.h"
#include "Bird.h"
#include <vector>
#include <iostream>

SimulationConfig Bird::m_simulationConfig;
RaylibConfig Bird::m_raylibConfig;

Bird::Bird(Vector2 pos, Vector2 vel) : m_position{ pos }, m_velocity{ vel } {};

Vector2 Bird::getPosition() const
{
	return m_position;
};
Vector2 Bird::getVelocity() const
{
	return m_velocity;
};

void Bird::update(const std::vector<Bird>& birds)
{
	Vector2 closeVel{ 0.0, 0.0 };
	Vector2 neighborAvgVel{ 0.0, 0.0 };
	Vector2 neighborAvgPos{ 0.0, 0.0 };
	int alignmentNeighbors = 0;
	int cohesionNeighbors = 0;

	for (const Bird& other : birds)
	{
		if (&other == this) continue;
		Vector2 otherPosition = other.getPosition();
		Vector2 otherVelocity = other.getVelocity();
		float dist = Vector2Distance(m_position, otherPosition);

		if (dist < m_simulationConfig.seperationRadius)
		{
			closeVel.x += m_position.x - otherPosition.x;
			closeVel.y += m_position.y - otherPosition.y;
		}
		if (dist < m_simulationConfig.alignmentRadius)
		{
			neighborAvgVel.x += otherVelocity.x;
			neighborAvgVel.y += otherVelocity.y;
			alignmentNeighbors += 1;
		}
		if (dist < m_simulationConfig.cohesionRadius)
		{
			neighborAvgPos.x += otherPosition.x;
			neighborAvgPos.y += otherPosition.y;
			cohesionNeighbors += 1;
		}
	}

	// Seperation
	m_velocity.x += closeVel.x * m_simulationConfig.seperationFactor;
	m_velocity.y += closeVel.y * m_simulationConfig.seperationFactor;

	// Alignment
	if (alignmentNeighbors > 0)
	{
		neighborAvgVel.x /= alignmentNeighbors;
		neighborAvgVel.y /= alignmentNeighbors;
		m_velocity.x += (neighborAvgVel.x - m_velocity.x) * m_simulationConfig.alignmentFactor;
		m_velocity.y += (neighborAvgVel.y - m_velocity.y) * m_simulationConfig.alignmentFactor;
	}
	
	// Cohesion
	if (cohesionNeighbors > 0)
	{
		neighborAvgPos.x /= cohesionNeighbors;
		neighborAvgPos.y /= cohesionNeighbors;
		m_velocity.x += (neighborAvgPos.x - m_position.x) * m_simulationConfig.cohesionFactor;
		m_velocity.y += (neighborAvgPos.y - m_position.y) * m_simulationConfig.cohesionFactor;
	}

	// Edge turning
	int marginX = m_raylibConfig.screenMarginX;
	int marginY = m_raylibConfig.screenMarginY;
	float tf = m_simulationConfig.turnFactor;
	if (m_position.x < marginX)
	{
		m_velocity.x += tf;
	}
	if (m_position.x > m_raylibConfig.screenWidth - marginX)
	{
		m_velocity.x -= tf;
	}
	if (m_position.y < marginY)
	{
		m_velocity.y += tf;
	}
	if (m_position.y > m_raylibConfig.screenHeight - marginY)
	{
		m_velocity.y -= tf;
	}

	// Speed limit 
	float speed = Vector2Length(m_velocity);
	float maxSpeed = m_simulationConfig.maxSpeed;
	float minSpeed = m_simulationConfig.minSpeed;
	if (speed > maxSpeed)
	{
		m_velocity.x *= maxSpeed / speed;
		m_velocity.y *= maxSpeed / speed;
	}
	if (speed < minSpeed)
	{
		m_velocity.x *= minSpeed / speed;
		m_velocity.y *= minSpeed / speed;
	}

	// Update position
	m_position.x += m_velocity.x * m_raylibConfig.deltaTime;
	m_position.y += m_velocity.y * m_raylibConfig.deltaTime;
};

void Bird::setSimulationConfig(SimulationConfig simConfig)
{
	m_simulationConfig = simConfig;
};

void Bird::setRaylibConfig(RaylibConfig raylibConfig)
{
	m_raylibConfig = raylibConfig;
}
