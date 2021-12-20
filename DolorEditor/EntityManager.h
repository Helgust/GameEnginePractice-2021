#pragma once
#include "ScriptSystem/ScriptSystem.h"
#include "LoadingSystem/LoadingSystem.h"
#include "FileSystem/FileSystem.h"

#include "string.h"
#include <filesystem>

class FileSystem;
class ScriptSystem;
class LoadingSystem;

struct EntityInfo
{
	std::string objName;
	std::string meshName;
	std::string scriptName;
	Ogre::Vector3 position;
	Ogre::Quaternion rotation;
};

struct Entity
{
	RenderNode* pRenderNode;
	std::string scriptName;
	Ogre::Vector3 position;
	Ogre::Quaternion rotation;
	int idx;
};


class EntityManager
{
public:
	EntityManager(RenderEngine* pRenderEngine, ScriptSystem* pScriptSystem);
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	//void CreateEntity(std::string strScriptName);
	void CreateEntity(const EntityInfo &fromSave);
	void SetNewScriptName(std::string newName, uint32_t id);
	void SetNameOfLevel(std::string newName);
	void ClearRenderNodes();
	//void ReloadScripts(FileSystem* m_pFileSystem);

	std::unordered_map<uint32_t, Entity> GetEntityQueue() const;
	std::string GetNameOfLevel() { return m_sLevelName; };
	
	void Update();

private:
	ScriptSystem* m_pScriptSystem;
	RenderEngine* m_pRenderEngine;
	LoadingSystem* m_pLoadingSystem;
	std::string m_sLevelName;
	std::unordered_map<uint32_t, Entity> m_entityQueue;

	uint32_t GetNewIndex() const;
};