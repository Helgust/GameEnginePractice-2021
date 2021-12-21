#pragma once

#include "Ogre.h"
#include "OgreRoot.h"
#include "OgreWindow.h"
#include "OgreItem.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2.h"

#include "Compositor/OgreCompositorManager2.h"

#include "RenderSystems/Direct3D11/OgreD3D11Plugin.h"
#include "RenderSystems/GL3Plus/OgreGL3PlusPlugin.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "../UI/imgui.h"
#include "../UI/imgui_impl_sdl.h"
#include "../UI/imgui_impl_opengl3.h"

#include "../Input/InputHandler.h"

#ifdef _WINDOWS
#include "SDL_syswm.h"
#endif 

#include "RenderThread.h"
#include "RenderNode.h"
#include "../ResourceManager.h"
#include "../FileSystem/FileSystem.h"

class RenderEngine
{
	friend class RenderThread;

public:
	RenderEngine(ResourceManager* pResourceManager, InputHandler* pInputManager, FileSystem* m_pFileSystem);
	~RenderEngine();
	RenderEngine(const RenderEngine&) = delete;
	RenderEngine& operator=(const RenderEngine&) = delete;

	void Update();

	bool GetQuit() { return m_bQuit; };
	void SetQuit(bool bQuit) { m_bQuit = bQuit; };
	SDL_Window* GetWindows() { return m_SDL_Window; };

	RenderThread* GetRT() const { return m_pRT; }


	void RT_SDLClenup();


private:
	bool SetOgreConfig();

	void RT_Init();
	void RT_SetupDefaultCamera();
	void RT_SetupDefaultCompositor();
	void RT_LoadDefaultResources();
	void RT_SetupDefaultLight();
	void RT_CreateSceneNode(RenderNode* pRenderNode);
	void RT_InitSDL();

	void RaycastToMouse();

	void StartGuiUpdate();
	void EndGuiUpdate();
	void DisplaySelectionParameters();
	void DisplayAllScripts();
	void DisplayMenuBar();

	void ImportV1Mesh(Ogre::String strMeshName);


	Ogre::SceneNode* m_pCurSNSelection;
	RenderNode* m_pCurRNSelection;
	bool m_bSelectionChanged;
	bool m_bIsFreeze;

	Ogre::Root* m_pRoot;
	Ogre::Window* m_pRenderWindow;
	Ogre::SceneManager* m_pSceneManager;
	Ogre::Camera* m_pCamera;
	InputHandler* m_pInputManager;
	Ogre::CompositorWorkspace* m_pWorkspace;
	Ogre::D3D11Plugin* m_pD3D11Plugin;
	Ogre::GL3PlusPlugin* m_pGL3PlusPlugin;

	RenderThread* m_pRT;
	ResourceManager* m_pResourceManager;
	FileSystem* m_pFileSystem;

	std::vector<RenderNode*> m_RenderNodes;
	SDL_Window* m_SDL_Window;
	SDL_GLContext m_GL_Context;
	ImGuiContext* m_pImGuiContext;

	bool m_bQuit;
};

