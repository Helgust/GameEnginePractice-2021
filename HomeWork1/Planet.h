#pragma once
#include "Ogre.h"
class Planet
{
private:
	float m_fRadius;
	int m_nMass;
	Ogre::Vector3 m_v3Position;
	Ogre::Vector3 m_v3Velocity;
	Ogre::Vector3 m_v3Acceleration;
	

public:
	Planet(Ogre::Vector3 new_pos,float radius,size_t mass);
	void Update(float deltaTime);
	Ogre::Vector3 GetPosition();
	Ogre::Vector3 GetVelocity();
	int GetMass();
	float GetRadius();
	Ogre::Vector3 GetAcceleration();

	void SetVelocity(Ogre::Vector3 new_vel);
	void SetAcceleration(Ogre::Vector3 new_acc);
};

