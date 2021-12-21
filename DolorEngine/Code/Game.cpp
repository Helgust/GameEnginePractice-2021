#include "Game.h"
#include "ECS/ecsMesh.h"
#include "ECS/ecsSystems.h"
#include "ECS/ecsPhys.h"
#include "ECS/ecsControl.h"
#include <stdlib.h>

Game::Game()
{
	m_pEcs = new flecs::world();
	m_pFileSystem = new FileSystem();
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager, m_pInputHandler, m_pFileSystem);
	m_pScriptSystem = new ScriptSystem(m_pInputHandler, m_pFileSystem->GetScriptsRoot());
	m_pEntityManager = new EntityManager(m_pRenderEngine, m_pScriptSystem, m_pEcs);
	m_pLoadingSystem = new LoadingSystem(m_pEntityManager, m_pFileSystem->GetSavesRoot(), m_pFileSystem->GetLevelsRoot());

	m_Timer.Start();

	m_pEcs->entity("inputHandler")
		.set(InputHandlerPtr{ m_pInputHandler });
	m_pEcs->entity("scriptSystem")
		.set(ScriptSystemPtr{ m_pScriptSystem });

	//m_pLoadingSystem->LoadFromXML("initialScene.xml");
	m_pLoadingSystem->LoadFromBin("level0.dlr");

	register_ecs_mesh_systems(m_pEcs);
	register_ecs_control_systems(m_pEcs);
	register_ecs_phys_systems(m_pEcs);
	register_ecs_script_systems(m_pEcs);
	register_ecs_static_systems(m_pEcs);

	m_pFileSystem->CreateInitMapOfFiles();
}

Game::~Game()
{
	SAFE_DELETE(m_pEcs);
	SAFE_DELETE(m_pFileSystem);
	SAFE_DELETE(m_pResourceManager);
	SAFE_DELETE(m_pInputHandler);
	SAFE_DELETE(m_pRenderEngine);
	SAFE_DELETE(m_pScriptSystem);
	SAFE_DELETE(m_pEntityManager);
	SAFE_DELETE(m_pLoadingSystem);
}

void Game::Run()
{
	m_Timer.Reset();

	while (true) // was just "true"
	{
		m_pRenderEngine->GetRT()->RC_BeginFrame();

		//static bool isDone = false;

		if (m_pInputHandler)
		{
			m_pInputHandler->Update(m_pRenderEngine->GetWindows());
		}

		if (m_pInputHandler->Get(SDL_SCANCODE_F8))
		{
			m_pEntityManager->ReloadScripts(m_pFileSystem);
		}

		m_Timer.Tick();

		if (!Update())
		{
			break;
		}

		if (m_pInputHandler->GetQuit())
		{
			m_pRenderEngine->GetRT()->RC_SDLCleanup();
			break;
		}
		m_pRenderEngine->GetRT()->RC_EndFrame();
	}
}

bool Game::Update()
{
	m_pEcs->progress();

	return true;
}
