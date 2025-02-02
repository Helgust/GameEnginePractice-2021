#pragma once

#include "ScriptSystem/ScriptSystem.h"
#include "FileSystem/FileSystem.h"

#include "string.h"
#include <filesystem>

class FileSystem;
class ScriptSystem;

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
	ScriptNode* pScriptNode;
	Ogre::Vector3 position;
	Ogre::Quaternion rotation;
	int idx;
};


class EntityManager
{
public:
	EntityManager(RenderEngine* pRenderEngine, ScriptSystem* pScriptSystem, flecs::world* ecs);
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	void CreateEntity(std::string strScriptName);
	void CreateEntity(const EntityInfo &fromSave);
	void ReloadScripts(FileSystem* m_pFileSystem);

	std::unordered_map<uint32_t, Entity> GetEntityQueue() const;
	void Update();

private:
	ScriptSystem* m_pScriptSystem;
	RenderEngine* m_pRenderEngine;
	//LoadingSystem* m_pLoadingSystem;
	flecs::world* m_pEcs;

	std::unordered_map<uint32_t, Entity> m_entityQueue;

	uint32_t GetNewIndex() const;
};