#include "LoadingSystem.h"
#include "../EntityManager.h"
#include "OgreSceneManager.h"
#include "tinyxml.h"
#include <fstream>
#include <regex>


LoadingSystem::LoadingSystem(EntityManager* pEntityManager, const std::string& savesRoot, const std::string& levelsRoot) :
	m_pEntityManager(pEntityManager),
	m_strSavesRootPath(savesRoot),
	m_strLevelsRootPath(levelsRoot)
{

}

void LoadingSystem::LoadFromXML(const std::string fileName)
{
	const auto pathName = m_strSavesRootPath + fileName;
	m_strCurSaveFile = fileName;
	TiXmlDocument doc(pathName.c_str());

	if (doc.LoadFile())
	{
		const auto elem = doc.FirstChildElement("scene");
		m_pEntityManager->SetNameOfLevel(elem->Attribute("name"));
		for (TiXmlElement* e = elem->FirstChildElement("character"); e != nullptr; e = e->NextSiblingElement("character"))
		{

			EntityInfo currentCharacter;
			currentCharacter.objName = e->Attribute("name");
			currentCharacter.meshName = e->Attribute("meshName");
			currentCharacter.scriptName = e->Attribute("scriptName");
			currentCharacter.position = ParsePosition(e->Attribute("position"));

			m_pEntityManager->CreateEntity(currentCharacter);
		}
	}
}

//void LoadingSystem::LoadFromJSON(const std::string fileName)
//{
//	const auto pathName = m_strSavesRootPath + fileName;
//	m_pEntityManager->ClearRenderNodes();
//
//	std::ifstream i(pathName);
//	nlohmann::json j;
//	i >> j;
//	for (auto obj : j) 
//	{
//		EntityInfo currentCharacter;
//		currentCharacter.meshName = obj["meshName"];
//		currentCharacter.scriptName = obj["scriptName"];
//		currentCharacter.position = obj["position"];
//		
//
//		m_pEntityManager->CreateEntity(currentCharacter);
//	}
//}

void LoadingSystem::SaveToXML(const std::string fileName)
{
	const auto pathName = m_strSavesRootPath + "New"+fileName;
	TiXmlDocument doc(pathName.c_str());
	TiXmlElement* root = new TiXmlElement("scene");
	doc.LinkEndChild(root);

	root->SetAttribute("name", m_pEntityManager->GetNameOfLevel().c_str());
	auto entityQueue = m_pEntityManager->GetEntityQueue();

	for (auto& entity : entityQueue)
	{
		TiXmlElement* e = new TiXmlElement("character");
		root->LinkEndChild(e);
		e->SetAttribute("name", entity.second.pRenderNode->GetObjName().c_str());
		e->SetAttribute("scriptName", entity.second.scriptName.c_str());
		e->SetAttribute("meshName", entity.second.pRenderNode->GetMeshName().c_str());
		e->SetAttribute("position", Ogre::StringConverter::toString(entity.second.pRenderNode->GetPosition()).c_str());
	}
	
	doc.SaveFile();
}

int LoadingSystem::ExportToEngine()
{
	const auto pathName = m_strLevelsRootPath +  "level0" + ".dlr";
	
	const auto pathXmlFile = m_strSavesRootPath + m_strCurSaveFile;
	TiXmlDocument doc(pathXmlFile.c_str());
	std::vector<EntityInfo> tempVec;

	if (doc.LoadFile())
	{
		const auto elem = doc.FirstChildElement("scene");
		m_pEntityManager->SetNameOfLevel(elem->Attribute("name"));
		for (TiXmlElement* e = elem->FirstChildElement("character"); e != nullptr; e = e->NextSiblingElement("character"))
		{

			EntityInfo currentCharacter;
			currentCharacter.objName = e->Attribute("name");
			currentCharacter.meshName = e->Attribute("meshName");
			currentCharacter.scriptName = e->Attribute("scriptName");
			currentCharacter.position = ParsePosition(e->Attribute("position"));

			tempVec.push_back(currentCharacter);
		}

		std::ofstream wf(pathName, std::ios::out | std::ios::binary);
		if (!wf) {
			Ogre::LogManager::getSingleton().logMessage("Cannot open file!");
			return 1;
		}

		int count = tempVec.size();
		wf.write((char*)&count, sizeof(int));
		for (auto x : tempVec)
		{
			wf.write((char*)&x, sizeof(EntityInfo));
		}
		wf.close();
		if (!wf.good()) 
		{
			Ogre::LogManager::getSingleton().logMessage("Error occurred at writing time!");
			return 1;
		}
	}
}

LoadingSystem::~LoadingSystem()
{

};

Ogre::Vector3 LoadingSystem::ParsePosition(const char* strPosition)
{
	std::regex regex("[+-]?([0-9]*[.])?[0-9]+");
	std::cmatch match;

	Ogre::Vector3 vPosition;

	std::regex_search(strPosition, match, regex);
	vPosition.x = std::stof(match[0]);

	strPosition = match.suffix().first;
	std::regex_search(strPosition, match, regex);
	vPosition.y = std::stof(match[0]);

	strPosition = match.suffix().first;
	std::regex_search(strPosition, match, regex);
	vPosition.z = std::stof(match[0]);

	return vPosition;
}