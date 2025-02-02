#include "EntityManager.h"

EntityManager::EntityManager(RenderEngine* pRenderEngine, ScriptSystem* pScriptSystem) :
	m_pRenderEngine(pRenderEngine),
	m_pScriptSystem(pScriptSystem)
{
	m_sLevelName = "";
}

EntityManager::~EntityManager()
{
	m_entityQueue.clear();
}

//void EntityManager::CreateEntity(std::string strScriptName)
//{
//	flecs::entity newEntity = m_pEcs->entity();
//	uint32_t nIndex = GetNewIndex();
//
//	ScriptNode* pScriptNode = m_pScriptSystem->CreateScriptNode(strScriptName, newEntity);
//
//	Ogre::String strMeshName = pScriptNode->GetMeshName();
//	RenderNode* pRenderNode = new RenderNode(nIndex, strMeshName);
//
//	newEntity.set(EntityIndex{ nIndex })
//		.set(RenderNodeComponent{ pRenderNode }));
//
//	m_pRenderEngine->GetRT()->RC_CreateSceneNode(pRenderNode);
//
//	Entity entity;
//	entity.pRenderNode = pRenderNode;
//	entity.scriptName = pScriptNode;
//
//	entity.idx = nIndex;
//	
//	m_entityQueue[nIndex] = entity;
//}

void EntityManager::ClearRenderNodes()
{
	m_sLevelName = "";
	m_entityQueue.clear();
}

void EntityManager::CreateEntity(const EntityInfo &fromSave)
{
	//flecs::entity newEntity = m_pEcs->entity();
	uint32_t nIndex = GetNewIndex();

	Ogre::String strMeshName = fromSave.meshName;
	Ogre::String strObjName = fromSave.objName;
	RenderNode* pRenderNode = new RenderNode(nIndex, strMeshName, strObjName);

	pRenderNode->SetPosition(fromSave.position);
	pRenderNode->SetOrientation(fromSave.rotation);

	m_pRenderEngine->GetRT()->RC_CreateSceneNode(pRenderNode);

	Entity entity;
	entity.pRenderNode = pRenderNode;
	entity.scriptName = fromSave.scriptName;
	entity.position = fromSave.position;
	entity.rotation = fromSave.rotation;
	entity.idx = nIndex;


	m_entityQueue[nIndex] = entity;
}

//void EntityManager::ReloadScripts(FileSystem* m_pFileSystem)
//{
//	for (auto x : m_entityQueue)
//	{
//		std::string file_path = x.second.pScriptNode->GetPath();
//		auto file_last_time = m_pFileSystem->m_mapFileReport[file_path];
//		if (std::filesystem::last_write_time(file_path) != file_last_time)
//		{
//			//ScriptNode* temp = x.second.pScriptNode;
//			//x.second.pScriptNode = m_pScriptSystem->CreateScriptNode(std::filesystem::path,);
//			x.second.pScriptNode->ReloadScript();
//			m_pFileSystem->m_mapFileReport[file_path] = std::filesystem::last_write_time(file_path);
//		}
//	}
//}

uint32_t EntityManager::GetNewIndex() const
{
	return m_entityQueue.size();
}

std::unordered_map<uint32_t, Entity> EntityManager::GetEntityQueue() const
{
	return m_entityQueue;
}

void EntityManager::SetNewScriptName(std::string newName, uint32_t id)
{
	m_entityQueue[id].scriptName = newName;
}

void EntityManager::SetNameOfLevel(std::string newName)
{
	m_sLevelName = newName;
}