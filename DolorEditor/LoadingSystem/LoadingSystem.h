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
	//void LoadFromJSON(const std::string fileName);
	void SaveToXML(const std::string fileName);
	int ExportToEngine();
	std::string GetCurrSaveFile() { return m_strCurSaveFile; };
private:
	EntityManager* m_pEntityManager;
	std::string m_strSavesRootPath;
	std::string m_strLevelsRootPath;
	std::string m_strCurSaveFile;
	Ogre::Vector3 ParsePosition(const char* strPosition);
	Ogre::Quaternion ParseRotation(const char* strRotation);
};

