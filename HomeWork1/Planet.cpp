#include "Planet.h"

Planet::Planet(Ogre::Vector3 new_pos, float radius, size_t mass)
{
	m_fRadius = radius;
	m_v3Position = new_pos;
	m_nMass = mass;
	m_v3Velocity = { 0,0,0};
	m_v3Acceleration = {0,0,0};
}

void Planet::Update(float deltaTime)
{
	m_v3Velocity += m_v3Acceleration * deltaTime;
	m_v3Position += m_v3Velocity * deltaTime;
}

Ogre::Vector3 Planet::GetPosition()
{
	////return Ogre::Vector3({
	//	m_fRadius * cos(m_v3Position.y),
	//	0,
	//	m_fRadius* sin(m_v3Position.y) });
	return m_v3Position;
}
int Planet::GetMass()
{
	return m_nMass;
}

float Planet::GetRadius()
{
	return m_fRadius;
}

Ogre::Vector3 Planet::GetVelocity()
{
	return m_v3Velocity;
}


void Planet::SetAcceleration(Ogre::Vector3 new_acc)
{
	m_v3Acceleration = new_acc;
}
void Planet::SetVelocity(Ogre::Vector3 new_vel)
{
	m_v3Velocity = new_vel;
}

