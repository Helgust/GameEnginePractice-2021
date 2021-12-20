#pragma once

#include "Render/RenderEngine.h"
#include "FileSystem/FileSystem.h"
#include "ScriptSystem/ScriptSystem.h"
#include "LoadingSystem/LoadingSystem.h"
#include "Input/InputHandler.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "WindowManager.h"
#include "GameTimer.h"
#include "flecs.h"


class Editor
{
public:
	Editor();
	~Editor();
	Editor(const Editor&) = delete;
	Editor& operator=(const Editor&) = delete;

	void Run();
	bool Update();

private:
	GameTimer m_Timer;
	flecs::world* m_pEcs;
	RenderEngine* m_pRenderEngine;
	FileSystem* m_pFileSystem;
	ResourceManager* m_pResourceManager;
	InputHandler* m_pInputHandler;
	ScriptSystem* m_pScriptSystem;
	EntityManager* m_pEntityManager;
	LoadingSystem* m_pLoadingSystem;
	WindowManager* m_pWindowManager;
};

