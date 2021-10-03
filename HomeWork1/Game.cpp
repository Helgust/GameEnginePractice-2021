#include "Game.h"

float G = 5.0f;
float zg = 2.0f;
float zm = 2.0f;
int k = 100;
float antiG = 0.5f;
int MagnConst = 0;
Game::Game() :
	m_pRenderEngine(nullptr)
{

	
	m_pRenderEngine = new RenderEngine();

	

	for (int i = 0; i < 9; ++i)
	{
		m_Planets.push_back(Planet({Ogre::Real(100*i+200),0,0 },6, rand()%50+10));
		m_Planets[i].SetVelocity({ 0,0,-(sqrt(2000 * G / 200))});
	}
	m_Planets.push_back(Planet({ 0,0,0 }, 10, 2000));
	
	m_Timer.Start();
}

Game::~Game()
{
}

void Game::Run()
{
	m_Timer.Reset();

	while (true)
	{
		m_pRenderEngine->GetRT()->RC_BeginFrame();

		m_Timer.Tick();

		if (!Update())
			break;

		m_pRenderEngine->GetRT()->RC_EndFrame();
	}
}

bool Game::Update()
{
	static float t = 0;
	float deltaTime = m_Timer.DeltaTime();
	//t += m_Timer.DeltaTime();
	t += 0.00001f;
	//m_pRenderEngine->GetRT()->RC_OscillateCamera(10.0f + sin(t));

	for (int i = 0; i < m_Planets.size(); ++i)
	{
		float f = 0;
		Planet self = m_Planets[i];
		Ogre::Vector3 m_v3SumAcc = { 0,0,0 };
		
		for (int j = 0; j < m_Planets.size(); ++j)
		{
			
			if (i == j)
			{
				continue;
			}
			float dist = self.GetPosition().distance(m_Planets[j].GetPosition());
			float min_dist = self.GetRadius() + m_Planets[j].GetRadius();
			int m = m_Planets[j].GetMass();
			if (dist <= min_dist)
			{
				Ogre::Vector3 newV = (self.GetVelocity() * self.GetMass() + (m_Planets[j].GetVelocity() * m))/(self.GetMass()+m);
				m_Planets[i].SetVelocity(((self.GetVelocity()*k)+newV)/(k+1));
				m_Planets[j].SetVelocity(((self.GetVelocity() * k) + newV) / (k + 1));
				f -= antiG * Ogre::min(std::abs(min_dist - dist), (float)Ogre::min((float)m, (float)self.GetMass()) * 3);
			}
			else
			{
				f += Ogre::min(self.GetMass() * m * G / (pow(dist, zg)), G / 10);
				float mf = MagnConst * self.GetMass() / (pow(dist, zm));
				f += mf;
			}
			int m_relative = self.GetMass() / m;

			m_v3SumAcc = f * (m_Planets[j].GetPosition() - self.GetPosition()) / (dist * self.GetMass());
			m_Planets[i].SetAcceleration(m_v3SumAcc);
			m_v3SumAcc *= -m_relative;
			m_Planets[j].SetAcceleration(m_v3SumAcc);
		}
		
	}
	for (int i = 0; i < m_Planets.size(); ++i)
	{
		m_Planets[i].Update(t);
		m_pRenderEngine->GetRT()->RC_MovePlanet(i, m_Planets[i].GetPosition());
	}

	return true;
}
