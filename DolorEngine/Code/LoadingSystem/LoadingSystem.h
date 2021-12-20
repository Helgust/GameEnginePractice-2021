#pragma once

#include <string>
#include "Ogre.h"

class EntityManager;

class LoadingSystem
{
public:
	LoadingSystem(EntityManager* pEntityManager, const std::string& savesRoot, const std::string& levelsRoot);

	~LoadingSystem();
	void LoadFromXML(const std::string fileName);
	int LoadFromBin(const std::string fileName);
	void SaveToXML(const std::string fileName);
private:
	EntityManager* m_pEntityManager;
	std::string m_strSavesRootPath;
	std::string m_strLevelsRootPath;

	Ogre::Vector3 ParsePosition(const char* strPosition);
};

