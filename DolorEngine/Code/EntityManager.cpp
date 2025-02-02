#include "EntityManager.h"

EntityManager::EntityManager(RenderEngine* pRenderEngine, ScriptSystem* pScriptSystem, flecs::world* ecs) :
	m_pRenderEngine(pRenderEngine),
	m_pEcs(ecs),
	m_pScriptSystem(pScriptSystem)
{

}

EntityManager::~EntityManager()
{
	m_entityQueue.clear();
}

void EntityManager::CreateEntity(std::string strScriptName)
{
	flecs::entity newEntity = m_pEcs->entity();
	uint32_t nIndex = GetNewIndex();

	ScriptNode* pScriptNode = m_pScriptSystem->CreateScriptNode(strScriptName, newEntity);

	Ogre::String strMeshName = pScriptNode->GetMeshName();
	RenderNode* pRenderNode = new RenderNode(nIndex, strMeshName);

	newEntity.set(EntityIndex{ nIndex })
		.set(RenderNodeComponent{ pRenderNode })
		.set(ScriptNodeComponent{ pScriptNode });

	m_pRenderEngine->GetRT()->RC_CreateSceneNode(pRenderNode);

	Entity entity;
	entity.pRenderNode = pRenderNode;
	entity.pScriptNode = pScriptNode;

	entity.idx = nIndex;
	
	m_entityQueue[nIndex] = entity;
}


void EntityManager::CreateEntity(const EntityInfo &fromSave)
{
	flecs::entity newEntity = m_pEcs->entity();
	uint32_t nIndex = GetNewIndex();

	ScriptNode* pScriptNode = m_pScriptSystem->CreateScriptNode(fromSave.scriptName, newEntity);
	pScriptNode->SetPosition(fromSave.position);
	pScriptNode->SetNewOrientation(fromSave.rotation);

	Ogre::String strMeshName = fromSave.meshName;
	RenderNode* pRenderNode = new RenderNode(nIndex, strMeshName);

	newEntity.set(EntityIndex{ nIndex })
		.set(RenderNodeComponent{ pRenderNode })
		.set(ScriptNodeComponent{ pScriptNode });

	m_pRenderEngine->GetRT()->RC_CreateSceneNode(pRenderNode);

	Entity entity;
	entity.pRenderNode = pRenderNode;
	entity.pScriptNode = pScriptNode;
	//entity.position = fromSave.position;
	//entity.rotation = fromSave.rotation;
	entity.idx = nIndex;

	m_entityQueue[nIndex] = entity;
}

void EntityManager::ReloadScripts(FileSystem* m_pFileSystem)
{
	for (auto x : m_entityQueue)
	{
		std::string file_path = x.second.pScriptNode->GetPath();
		auto file_last_time = m_pFileSystem->m_mapFileReport[file_path];
		if (std::filesystem::last_write_time(file_path) != file_last_time)
		{
			//ScriptNode* temp = x.second.pScriptNode;
			//x.second.pScriptNode = m_pScriptSystem->CreateScriptNode(std::filesystem::path,);
			x.second.pScriptNode->ReloadScript();
			m_pFileSystem->m_mapFileReport[file_path] = std::filesystem::last_write_time(file_path);
		}
	}
}

uint32_t EntityManager::GetNewIndex() const
{
	return m_entityQueue.size();
}

std::unordered_map<uint32_t, Entity> EntityManager::GetEntityQueue() const
{
	return m_entityQueue;
}