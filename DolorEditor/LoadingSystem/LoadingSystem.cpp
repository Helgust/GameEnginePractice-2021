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
			currentCharacter.rotation = ParseRotation(e->Attribute("rotation"));

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
	const auto pathName = m_strSavesRootPath +fileName;
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
		Ogre::Vector3 vPos = entity.second.pRenderNode->GetPosition();
		std::string strPos = std::to_string(vPos.x) + "," + std::to_string(vPos.y) + "," + std::to_string(vPos.z);
		e->SetAttribute("position", strPos.c_str());
		Ogre::Quaternion vQ = entity.second.pRenderNode->GetOrientation();
		std::string strQ = std::to_string(vQ.w) + "," + std::to_string(vQ.x) + "," + std::to_string(vQ.y)+"," + std::to_string(vQ.z);
		e->SetAttribute("rotation", strQ.c_str());
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
			currentCharacter.rotation = ParseRotation(e->Attribute("rotation"));

			tempVec.push_back(currentCharacter);
		}

		std::ofstream outfile;
		outfile.open(pathName, std::ios::binary | std::ios::out);
		if (!outfile) {
			//Ogre::LogManager::getSingleton().logMessage("Cannot open file!");
			return 1;
		}

		int count = tempVec.size();
		outfile.write((char*)&count, sizeof(int)); // sizeof can take a type
		for (auto x : tempVec)
		{
			count = x.objName.size();
			outfile.write((char*)&count, sizeof(int));
			outfile.write(x.objName.c_str(), count);
			count = x.meshName.size();
			outfile.write((char*)&count, sizeof(int));
			outfile.write(x.meshName.c_str(), count);
			count = x.scriptName.size();
			outfile.write((char*)&count, sizeof(int));
			outfile.write(x.scriptName.c_str(), count);
			outfile.write((char*)&x.position, sizeof(Ogre::Vector3));
			outfile.write((char*)&x.rotation, sizeof(Ogre::Quaternion));
		}
		outfile.close();
		if (!outfile.good())
		{
			//Ogre::LogManager::getSingleton().logMessage("Error occurred at writing time!");
			return 1;
		}
		return 0;
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

Ogre::Quaternion LoadingSystem::ParseRotation(const char* strRotation)
{
	std::regex regex("[+-]?([0-9]*[.])?[0-9]+");
	std::cmatch match;

	Ogre::Quaternion vQuaternion;

	std::regex_search(strRotation, match, regex);
	vQuaternion.w = std::stof(match[0]);

	strRotation = match.suffix().first;
	std::regex_search(strRotation, match, regex);
	vQuaternion.x = std::stof(match[0]);

	strRotation = match.suffix().first;
	std::regex_search(strRotation, match, regex);
	vQuaternion.y = std::stof(match[0]);

	strRotation = match.suffix().first;
	std::regex_search(strRotation, match, regex);
	vQuaternion.z = std::stof(match[0]);

	return vQuaternion;
}

