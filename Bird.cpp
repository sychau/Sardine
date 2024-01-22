#include "raylib.h"
#include "raymath.h"
#include "Bird.h"
#include <vector>
#include <iostream>

SimulationConfig Bird::m_simulationConfig;
RaylibConfig Bird::m_raylibConfig;

Bird::Bird(Vector3 pos, Vector3 vel, Color col) : m_position{ pos }, m_velocity{ vel }, m_color{ col } {};

Vector3 Bird::getPosition() const
{
	return m_position;
};

Vector3 Bird::getVelocity() const
{
	return m_velocity;
};

Color Bird::getColor() const
{
	return m_color;
};

void Bird::update(const std::vector<Bird>& birds)
{
	Vector3 closeVel{ 0.0, 0.0 };
	Vector3 neighborAvgVel{ 0.0, 0.0 };
	Vector3 neighborAvgPos{ 0.0, 0.0 };
	int alignmentNeighbors = 0;
	int cohesionNeighbors = 0;

	for (const Bird& other : birds)
	{
		if (&other == this) continue;
		Vector3 otherPosition = other.getPosition();
		Vector3 otherVelocity = other.getVelocity();
		float dist = Vector3Distance(m_position, otherPosition);

		if (dist < m_simulationConfig.seperationRadius)
		{
			closeVel.x += m_position.x - otherPosition.x;
			closeVel.y += m_position.y - otherPosition.y;
			closeVel.z += m_position.z - otherPosition.z;
		}
		if (dist < m_simulationConfig.alignmentRadius)
		{
			neighborAvgVel.x += otherVelocity.x;
			neighborAvgVel.y += otherVelocity.y;
			neighborAvgVel.z += otherVelocity.z;
			alignmentNeighbors += 1;
		}
		if (dist < m_simulationConfig.cohesionRadius)
		{
			neighborAvgPos.x += otherPosition.x;
			neighborAvgPos.y += otherPosition.y;
			neighborAvgPos.z += otherPosition.z;
			cohesionNeighbors += 1;
		}
	}

	// Seperation
	m_velocity.x += closeVel.x * m_simulationConfig.seperationFactor;
	m_velocity.y += closeVel.y * m_simulationConfig.seperationFactor;
	m_velocity.z += closeVel.z * m_simulationConfig.seperationFactor;

	// Alignment
	if (alignmentNeighbors > 0)
	{
		neighborAvgVel.x /= alignmentNeighbors;
		neighborAvgVel.y /= alignmentNeighbors;
		neighborAvgVel.z /= alignmentNeighbors;
		m_velocity.x += (neighborAvgVel.x - m_velocity.x) * m_simulationConfig.alignmentFactor;
		m_velocity.y += (neighborAvgVel.y - m_velocity.y) * m_simulationConfig.alignmentFactor;
		m_velocity.z += (neighborAvgVel.z - m_velocity.z) * m_simulationConfig.alignmentFactor;
	}
	
	// Cohesion
	if (cohesionNeighbors > 0)
	{
		neighborAvgPos.x /= cohesionNeighbors;
		neighborAvgPos.y /= cohesionNeighbors;
		neighborAvgPos.z /= cohesionNeighbors;
		m_velocity.x += (neighborAvgPos.x - m_position.x) * m_simulationConfig.cohesionFactor;
		m_velocity.y += (neighborAvgPos.y - m_position.y) * m_simulationConfig.cohesionFactor;
		m_velocity.z += (neighborAvgPos.z - m_position.z) * m_simulationConfig.cohesionFactor;
	}

	// Edge turning
	int marginX = m_raylibConfig.MarginX;
	int marginY = m_raylibConfig.MarginY;
	int marginZ = m_raylibConfig.MarginZ;
	float tf = m_simulationConfig.turnFactor;
	if (m_position.x > m_raylibConfig.containerWidth / 2 - marginX)
		m_velocity.x -= tf;
	
	if (m_position.x < -m_raylibConfig.containerWidth / 2 + marginX)
		m_velocity.x += tf;
	
	if (m_position.y > m_raylibConfig.containerHeight / 2 - marginY)
		m_velocity.y -= tf;
	
	if (m_position.y < -m_raylibConfig.containerHeight / 2 + marginY)
		m_velocity.y += tf;
	
	if (m_position.z > m_raylibConfig.containerDepth / 2 - marginZ)
		m_velocity.z -= tf;
	
	if (m_position.z < -m_raylibConfig.containerDepth / 2 + marginZ)
		m_velocity.z += tf;
	
	// Speed limit 
	float speed = Vector3Length(m_velocity);
	float maxSpeed = m_simulationConfig.maxSpeed;
	float minSpeed = m_simulationConfig.minSpeed;
	if (speed > maxSpeed)
	{
		m_velocity.x *= maxSpeed / speed;
		m_velocity.y *= maxSpeed / speed;
		m_velocity.z *= maxSpeed / speed;
	}
	if (speed < minSpeed)
	{
		m_velocity.x *= minSpeed / speed;
		m_velocity.y *= minSpeed / speed;
		m_velocity.z *= minSpeed / speed;
	}

	// Update position
	m_position.x += m_velocity.x * m_raylibConfig.deltaTime;
	m_position.y += m_velocity.y * m_raylibConfig.deltaTime;
	m_position.z += m_velocity.z * m_raylibConfig.deltaTime;
};

void Bird::setSimulationConfig(SimulationConfig simConfig)
{
	m_simulationConfig = simConfig;
};

void Bird::setRaylibConfig(RaylibConfig raylibConfig)
{
	m_raylibConfig = raylibConfig;
}
