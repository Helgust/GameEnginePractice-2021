#include "LoadingSystem.h"
#include "../EntityManager.h"
#include "OgreSceneManager.h"
#include "tinyxml.h"
#include <regex>
#include<fstream>

LoadingSystem::LoadingSystem(EntityManager* pEntityManager, const std::string& savesRoot, const std::string& levelsRoot) :
	m_pEntityManager(pEntityManager),
	m_strSavesRootPath(savesRoot),
	m_strLevelsRootPath(levelsRoot)
{

}

void LoadingSystem::LoadFromXML(const std::string fileName)
{
	const auto pathName = m_strSavesRootPath + fileName;

	TiXmlDocument doc(pathName.c_str());

	if (doc.LoadFile())
	{
		const auto elem = doc.FirstChildElement("scene");
		for (TiXmlElement* e = elem->FirstChildElement("character"); e != nullptr; e = e->NextSiblingElement("character"))
		{
			EntityInfo currentCharacter;
			currentCharacter.meshName = e->Attribute("meshName");
			currentCharacter.scriptName = e->Attribute("scriptName");
			currentCharacter.position = ParsePosition(e->Attribute("position"));

			m_pEntityManager->CreateEntity(currentCharacter);
		}
	}
}

int LoadingSystem::LoadFromBin(const std::string fileName)
{
	const auto pathName = m_strLevelsRootPath + fileName;
	std::vector<EntityInfo> entityVector;
	std::ifstream infile;
	infile.open( pathName, std::ios::binary | std::ios::in);
	if (!infile)
	{
		//cout << "Cannot open file!" << endl;
		return 1;
	}
	int n;
	infile.read((char*)&n, sizeof(int));
	std::cout << n;
	for (int i = 0; i < n; i++)
	{
		EntityInfo tempEI;
		int tempSize;
		std::string tempStr;
		infile.read((char*)&tempSize, sizeof(int));
		char* temp = new char[tempSize + 1];
		infile.read(temp, tempSize);
		temp[tempSize] = '\0';
		tempEI.objName = temp;
		delete[] temp;

		infile.read((char*)&tempSize, sizeof(int));
		temp = new char[tempSize + 1];
		infile.read(temp, tempSize);
		temp[tempSize] = '\0';
		tempEI.meshName = temp;
		delete[] temp;

		infile.read((char*)&tempSize, sizeof(int));
		temp = new char[tempSize + 1];
		infile.read(temp, tempSize);
		temp[tempSize] = '\0';
		tempEI.scriptName = temp;
		delete[] temp;
		
		Ogre::Vector3 tempPos;
		infile.read((char*)&tempPos, sizeof(Ogre::Vector3));
		tempEI.position = tempPos;
		Ogre::Quaternion tempQ;
		infile.read((char*)&tempQ, sizeof(Ogre::Quaternion));
		tempEI.rotation = tempQ;

		entityVector.push_back(tempEI);
	}
	infile.close();
	
	if (!infile.good()) {
		//cout << "Error occurred at reading time!" << endl;
		return 1;
	}
	for (auto x : entityVector)
	{
		m_pEntityManager->CreateEntity(x);
	}
	return 0;
}

void LoadingSystem::SaveToXML(const std::string fileName)
{
	const auto pathName = m_strSavesRootPath + fileName;
	TiXmlDocument doc(pathName.c_str());
	const auto elem = doc.FirstChildElement("scene");
	auto entityQueue = m_pEntityManager->GetEntityQueue();

	for (auto& entity : entityQueue)
	{
		for (TiXmlElement* e = elem->FirstChildElement("character"); e != NULL; e = e->NextSiblingElement("character"))
		{
			TiXmlElement* meshElement = e->FirstChildElement("meshName");
			meshElement->SetAttribute("meshName", entity.second.pScriptNode->GetMeshName().c_str());
		}
	}

	doc.SaveFile();
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