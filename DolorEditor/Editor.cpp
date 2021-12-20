#include "Editor.h"
#include <stdlib.h>

Editor::Editor()
{
	m_pFileSystem = new FileSystem();
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager, m_pInputHandler, m_pFileSystem);
	m_pScriptSystem = new ScriptSystem(m_pInputHandler, m_pFileSystem->GetScriptsRoot());
	m_pEntityManager = new EntityManager(m_pRenderEngine, m_pScriptSystem);
	m_pLoadingSystem = new LoadingSystem(m_pEntityManager, m_pFileSystem->GetSavesRoot());

	m_Timer.Start();


	m_pLoadingSystem->LoadFromXML("initialScene.xml");

	m_pFileSystem->CreateInitMapOfFiles();
}

Editor::~Editor()
{
	SAFE_DELETE(m_pFileSystem);
	SAFE_DELETE(m_pResourceManager);
	SAFE_DELETE(m_pInputHandler);
	SAFE_DELETE(m_pRenderEngine);
	SAFE_DELETE(m_pEntityManager);
	SAFE_DELETE(m_pLoadingSystem);
	SAFE_DELETE(m_pScriptSystem);
}

void Editor::Run()
{

	m_Timer.Reset();


	while (true) // was just "true"
	{
		m_pRenderEngine->GetRT()->RC_BeginFrame();


		//static bool isDone = false;

		if (m_pInputHandler)
		{
			m_pInputHandler->Update();
		}


		if (m_pInputHandler->IsCommandActive(eIC_MoveForward))
		{
			m_pRenderEngine->GetRT()->RC_MoveFB(m_Timer.DeltaTime(), true);
		}

		if (m_pInputHandler->IsCommandActive(eIC_MoveBack))
		{
			m_pRenderEngine->GetRT()->RC_MoveFB(m_Timer.DeltaTime(), false);
		}

		if (m_pInputHandler->IsCommandActive(eIC_MoveLeft))
		{
			m_pRenderEngine->GetRT()->RC_MoveLR(m_Timer.DeltaTime(), true);
		}

		if (m_pInputHandler->IsCommandActive(eIC_MoveRight))
		{
			m_pRenderEngine->GetRT()->RC_MoveLR(m_Timer.DeltaTime(), false);
		}

		if (m_pInputHandler->IsCommandActive(eIC_MoveUp))
		{
			m_pRenderEngine->GetRT()->RC_MoveUD(m_Timer.DeltaTime(), false);
		}

		if (m_pInputHandler->IsCommandActive(eIC_MoveDown))
		{
			m_pRenderEngine->GetRT()->RC_MoveUD(m_Timer.DeltaTime(), true);
		}

		if (m_pInputHandler->GetRMouseStatus()) 
		{
			int width, height;
			SDL_GetWindowSize(m_pRenderEngine->GetWindow(), &width, &height);
			float diffY = -m_pInputHandler->GetMouseDiffY() / height * m_Timer.DeltaTime()* m_pInputHandler->GetMouseSensitivity() * 500;
			float diffX = -m_pInputHandler->GetMouseDiffX() / width* m_Timer.DeltaTime() * m_pInputHandler->GetMouseSensitivity() * 500;
			m_pRenderEngine->GetRT()->RC_SetRotation(diffX,diffY);
		}

		/*if (m_pInputHandler->Get(SDLK_F8))
		{
			m_pEntityManager->GetEntityQueue();
		}*/

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

bool Editor::Update()
{
	//m_pEcs->progress();

	return true;
}
