#pragma once

#include "Render/RenderEngine.h"
#include "EntityManager.h"
#include "OgreSceneNode.h"

class WindowManager
{
public:
	WindowManager(RenderEngine* pRenderEngine, EntityManager* pEntityManager, FileSystem* pFileSystem, LoadingSystem* pLoadingSystem);
	~WindowManager();

	void Update();
	bool IsQuit() const { return m_bIsClosed; };
	bool IsFrozen() const { return m_bIsFrozen; };

	//void SetSelection(Ogre::SceneNode* newSelection) { m_pCurrentSelection = newSelection; };
private:

	bool m_bIsFrozen;
	bool m_bIsClosed;

	RenderEngine* m_pRenderEngine;
	EntityManager* m_pEntityManager;
	FileSystem* m_pFileSystem;
	LoadingSystem* m_pLoadingSystem;

	void ProcessSDLInput();
	void DeleteScene();

	void InitWindow();
	void InitImGui();

	void RenderGui();
	void StartGuiUpdate();
	void EndGuiUpdate();
	void DisplaySelectionParameters();
	void DisplayAllScripts();
	void DisplayFreezeBtn();
	void DisplayMenuBar();

	SDL_Window* m_SDLWindow;
	SDL_GLContext m_GLContext;
	RenderNode* m_pCurrentSelection;
};

