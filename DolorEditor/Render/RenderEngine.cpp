#include "RenderEngine.h"

RenderEngine::RenderEngine(
	ResourceManager* pResourceManager, 
	InputHandler* pInputManager, 
	FileSystem* pFileSystem) :
	m_pRoot(nullptr),
	m_pRenderWindow(nullptr),
	m_pSceneManager(nullptr),
	m_pD3D11Plugin(nullptr),
	m_pCamera(nullptr),
	m_pWorkspace(nullptr),
	m_pRT(nullptr),
	m_bQuit(false),
	m_pResourceManager(pResourceManager),
	m_pInputManager(pInputManager),
	m_pFileSystem(pFileSystem)

{
	m_pRT = new RenderThread(this);

	m_pRT->RC_Init();
	m_pRT->RC_SetupDefaultCamera();
	m_pRT->RC_SetupDefaultCompositor();
	m_pRT->RC_LoadDefaultResources();
	m_pRT->RC_SetupDefaultLight();

	m_pRT->Start();
}

RenderEngine::~RenderEngine()
{
	SAFE_OGRE_DELETE(m_pRoot);
}

bool RenderEngine::SetOgreConfig()
{
#ifdef _DEBUG
	constexpr bool bAlwaysShowConfigWindow = true;
	if (bAlwaysShowConfigWindow || !m_pRoot->restoreConfig())
#else
	if (!m_pRoot->restoreConfig())
#endif
	{
		if (!m_pRoot->showConfigDialog())
		{
			return false;
		}
	}

	return true;
}

void RenderEngine::Update()
{
	Ogre::WindowEventUtilities::messagePump();

	for (RenderNode* pRenderNode : m_RenderNodes)
	{
		if (pRenderNode->GetStatic())
			continue;

		Ogre::Vector3 vPosition = pRenderNode->GetPosition();
		pRenderNode->GetSceneNode()->setPosition(vPosition);

		Ogre::Quaternion orientation = pRenderNode->GetOrientation();
		pRenderNode->GetSceneNode()->setOrientation(orientation);

		if (pRenderNode->IsCameraEnabled())
		{
			m_pCamera->setPosition(pRenderNode->GetCameraPosition());
			m_pCamera->lookAt(vPosition);
		}
	}
	
	if (m_pRenderWindow->isVisible())
	{
		if (m_pRenderWindow->isClosed())
		{
			m_bQuit = true;
			return;
		}
		else
		{
			m_bQuit |= !m_pRoot->renderOneFrame();
		}
	}

	SDL_PumpEvents();
	{
		SDL_Event event;
		while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_TEXTINPUT) > 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
		}
	}

	if (m_pInputManager->GetLMouseStatus())
	{
		RaycastToMouse();
	}
		
	StartGuiUpdate();

	DisplayMenuBar();
	DisplayAllScripts();
	DisplaySelectionParameters();

	EndGuiUpdate();

	m_pRenderWindow->windowMovedOrResized();
}

void RenderEngine::RaycastToMouse()
{
	Ogre::Ray ray = m_pCamera->getCameraToViewportRay(float(m_pInputManager->GetMousePos().x) / m_pRenderWindow->getWidth(), float(m_pInputManager->GetMousePos().y) / m_pRenderWindow->getHeight());
	Ogre::RaySceneQuery* query = m_pSceneManager->createRayQuery(ray);
	query->setSortByDistance(true);

	bool mMovableFound = false;
	Ogre::RaySceneQueryResult& result = query->execute();
	if (!result.empty())
	{
		m_bSelectionChanged = m_pCurSelection != result[0].movable->getParentSceneNode();
		m_pCurSelection = result[0].movable->getParentSceneNode();
	}

	m_pSceneManager->destroyQuery(query);
}

void RenderEngine::StartGuiUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_SDL_Window);
	ImGui::NewFrame();
}

void RenderEngine::EndGuiUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_SDL_Window);	
}

void RenderEngine::DisplayAllScripts()
{
	ImGui::Begin("Scripts");
	
	for (const auto& entry : std::filesystem::directory_iterator(m_pFileSystem->GetScriptsRoot()))
	{
		if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".lua")
		{
			std::string btnName = entry.path().filename().string();
			if (ImGui::Button(btnName.c_str()))
			{
				std::string command = "start " + entry.path().string();
				std::system(command.c_str());
			}
		}
	}
	ImGui::End();
}

void RenderEngine::DisplaySelectionParameters()
{
	if (m_pCurSelection)
	{
		ImGui::Begin("Parameters");
		ImGui::Text(m_pCurSelection->getName().c_str());

		static float posVec[3] = { 0.f, 0.f, 0.f };
		posVec[0] = float(m_pCurSelection->getPosition().x);
		posVec[1] = float(m_pCurSelection->getPosition().y);
		posVec[2] = float(m_pCurSelection->getPosition().z);
		ImGui::InputFloat3("Position", posVec);

		static int rotVec[3] = { 0, 0, 0 };
		rotVec[0] = int(m_pCurSelection->getOrientation().getPitch().valueDegrees());
		rotVec[1] = int(m_pCurSelection->getOrientation().getYaw().valueDegrees());
		rotVec[2] = int(m_pCurSelection->getOrientation().getRoll().valueDegrees());
		ImGui::InputInt3("Rotation", rotVec);
		ImGui::End();

		m_pCurSelection->setPosition(posVec[0], posVec[1], posVec[2]);

		Ogre::Quaternion q = m_pCurSelection->getOrientation();
		int clampedX = std::clamp(rotVec[0], -180, 180);
		if (clampedX != int(m_pCurSelection->getOrientation().getPitch().valueDegrees()))
		{
			Ogre::Radian radX = Ogre::Radian(Ogre::Degree(clampedX));
			Ogre::Radian offsetX = radX - m_pCurSelection->getOrientation().getPitch();
			q.FromAngleAxis(offsetX, Ogre::Vector3::UNIT_X);
			q = q * m_pCurSelection->getOrientation();
			m_pCurSelection->setOrientation(q);
			return;
		}

		int clampedY = std::clamp(rotVec[1], -180, 180);
		if (clampedY != int(m_pCurSelection->getOrientation().getYaw().valueDegrees()))
		{
			Ogre::Radian radY = Ogre::Radian(Ogre::Degree(clampedY));
			Ogre::Radian offsetY = radY - m_pCurSelection->getOrientation().getYaw();
			q.FromAngleAxis(offsetY, Ogre::Vector3::UNIT_Y);
			q = q * m_pCurSelection->getOrientation();
			m_pCurSelection->setOrientation(q);
			return;
		}

		int clampedZ = std::clamp(rotVec[2], -180, 180);
		if (clampedZ != int(m_pCurSelection->getOrientation().getRoll().valueDegrees()))
		{
			Ogre::Radian radZ = Ogre::Radian(Ogre::Degree(clampedZ));
			Ogre::Radian offsetZ = radZ - m_pCurSelection->getOrientation().getRoll();
			q.FromAngleAxis(offsetZ, Ogre::Vector3::UNIT_Z);
			q = q * m_pCurSelection->getOrientation();
			m_pCurSelection->setOrientation(q);
			return;
		}
	}
	else
	{

	}
}


void RenderEngine::DisplayMenuBar()
{
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "CTRL+S")) {}
			if (ImGui::MenuItem("Load", "")) {}
			if (ImGui::MenuItem("Export To Engine", "")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


}


void RenderEngine::RT_Init()
{
	if (!m_pRoot)
	{
		m_pRoot = OGRE_NEW Ogre::Root();
		//m_pD3D11Plugin = OGRE_NEW Ogre::D3D11Plugin();
		m_pGL3PlusPlugin = OGRE_NEW Ogre::GL3PlusPlugin();

		//m_pRoot->installPlugin(m_pD3D11Plugin);
		m_pRoot->installPlugin(m_pGL3PlusPlugin);

		if (!SetOgreConfig())
		{
			m_bQuit = true;
			return;
		}

		m_pRoot->initialise(false);

		RT_InitSDL();

		// Creating window
		Ogre::uint32 width = 1280;
		Ogre::uint32 height = 720;
		Ogre::String sTitleName = "Dolor Engine";

		Ogre::NameValuePairList params;
		params.insert(std::make_pair("title", sTitleName));
		params.insert(std::make_pair("gamma", "true"));
		params.insert(std::make_pair("FSAA", "false"));
		params.insert(std::make_pair("vsync", "false"));

		

		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWindowWMInfo(m_SDL_Window, &info);

		params["externalWindowHandle"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.info.win.window));
		params["externalGLContent"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.info.win.hdc));
		params["externalGLControl"] = Ogre::String("True");

		m_pRenderWindow = Ogre::Root::getSingleton().createRenderWindow(sTitleName, width, height, false, &params);

		// Scene manager
		m_pSceneManager = m_pRoot->createSceneManager(Ogre::SceneType::ST_GENERIC, 1);
	}
	
}

void RenderEngine::RT_InitSDL()
{
	SDL_Init(SDL_INIT_VIDEO);

	// GL 3.0 + GLSL 130
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_SDL_Window = SDL_CreateWindow("Dolor Engine ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

	m_GL_Context = SDL_GL_CreateContext(m_SDL_Window);
	SDL_GL_MakeCurrent(m_SDL_Window, m_GL_Context);
	SDL_GL_SetSwapInterval(0);

	IMGUI_CHECKVERSION();
	m_pImGuiContext = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_pImGuiContext);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.WantCaptureKeyboard = true;
	Ogre::LogManager::getSingleton().logMessage(std::to_string(io.WantCaptureKeyboard));

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_SDL_Window, m_GL_Context);
	bool f = ImGui_ImplOpenGL3_Init(SDL_GL_VERSION);
}

void RenderEngine::RT_MoveFB(float time, bool dir)
{
	if (dir) //positive dir
	{
		m_pCamera->setPosition(m_pCamera->getPosition() + m_nCameraSpeed*m_pCamera->getDirection() * time);
	}
	else
	{
		m_pCamera->setPosition(m_pCamera->getPosition() - m_nCameraSpeed*m_pCamera->getDirection() * time);
	}
}
	

void RenderEngine::RT_MoveLR(float time, bool dir)
{
	if(dir)
	{
		m_pCamera->setPosition(m_pCamera->getPosition() - m_nCameraSpeed*m_pCamera->getRight() * time);
	}
	else
	{
		m_pCamera->setPosition(m_pCamera->getPosition() + m_nCameraSpeed*m_pCamera->getRight() * time);
	}
	
}

void RenderEngine::RT_MoveUD(float time, bool dir)
{
	if (dir)
	{
		m_pCamera->setPosition(m_pCamera->getPosition() + m_nCameraSpeed*m_pCamera->getUp() * time);
	}
	else
	{
		m_pCamera->setPosition(m_pCamera->getPosition() - m_nCameraSpeed*m_pCamera->getUp() * time);
	}
}

void RenderEngine::RT_SetRotation(float diffX, float diffY)
{
	m_pCamera->rotate(Ogre::Quaternion(Ogre::Radian(diffY), m_pCamera->getRight()));
	m_pCamera->yaw(Ogre::Radian(diffX));
}

void RenderEngine::RT_SDLClenup()
{
	// Cleanup
	ImGui::DestroyContext();
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	

	SDL_GL_DeleteContext(m_GL_Context);
	SDL_DestroyWindow(m_SDL_Window);
	SDL_Quit();
}

void RenderEngine::RT_SetupDefaultCamera()
{
	if (!m_pCamera)
	{
		m_pCamera = m_pSceneManager->createCamera("Main Camera");

		m_pCamera->setPosition(Ogre::Vector3(0, 10, 35));
		m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
		m_pCamera->setNearClipDistance(0.2f);
		m_pCamera->setFarClipDistance(1000.0f);
		m_pCamera->setAutoAspectRatio(true);
	}
	
}

void RenderEngine::RT_SetupDefaultCompositor()
{
	Ogre::CompositorManager2* compositorManager = m_pRoot->getCompositorManager2();

	const Ogre::String workspaceName("WorkSpace");

	if (!compositorManager->hasWorkspaceDefinition(workspaceName))
	{
		compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue::Blue);
	}

	m_pWorkspace = compositorManager->addWorkspace(m_pSceneManager, m_pRenderWindow->getTexture(), m_pCamera, workspaceName, true);
}

void RenderEngine::RT_LoadDefaultResources()
{
	m_pResourceManager->LoadOgreResources("resources.cfg");
}

void RenderEngine::RT_CreateSceneNode(RenderNode* pRenderNode)
{
	//Create an Item with the model we just imported.
	//Notice we use the name of the imported model. We could also use the overload
	//with the mesh pointer:
	Ogre::String strImportedMeshName = pRenderNode->GetMeshName() + "v1";
	if (!Ogre::MeshManager::getSingleton().resourceExists(strImportedMeshName))
		ImportV1Mesh(pRenderNode->GetMeshName());

	Ogre::Item* item = m_pSceneManager->createItem(strImportedMeshName,
		Ogre::ResourceGroupManager::
		AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::SCENE_DYNAMIC);
	Ogre::SceneNode* pSceneNode = m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		createChildSceneNode(Ogre::SCENE_DYNAMIC);
	
	pSceneNode->attachObject(item);
	pSceneNode->setName(pRenderNode->GetObjName());
	pSceneNode->scale(0.1f, 0.1f, 0.1f); // TODO: move out to ecs
	
	pRenderNode->SetSceneNode(pSceneNode);

	m_RenderNodes.push_back(pRenderNode);

	/*Ogre::LogManager::getSingleton().logMessage("RN_" + std::to_string(pRenderNode->GetId()));
	Ogre::LogManager::getSingleton().logMessage("SN_" + std::to_string(pSceneNode->getId()));*/
}

void RenderEngine::ImportV1Mesh(Ogre::String strMeshName)
{
	//Load the v1 mesh. Notice the v1 namespace
	//Also notice the HBU_STATIC flag; since the HBU_WRITE_ONLY
	//bit would prohibit us from reading the data for importing.
	Ogre::String strImportedMeshName = strMeshName + "v1";

	Ogre::v1::MeshPtr v1Mesh;
	Ogre::MeshPtr v2Mesh;

	v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
		strMeshName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);

	//Create a v2 mesh to import to, with a different name (arbitrary).
	v2Mesh = Ogre::MeshManager::getSingleton().createManual(
		strImportedMeshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	bool halfPosition = true;
	bool halfUVs = true;
	bool useQtangents = true;

	//Import the v1 mesh to v2
	v2Mesh->importV1(v1Mesh.get(), halfPosition, halfUVs, useQtangents);

	//We don't need the v1 mesh. Free CPU memory, get it out of the GPU.
	//Leave it loaded if you want to use athene with v1 Entity.
	v1Mesh->unload();
}

void RenderEngine::RT_SetupDefaultLight()
{
	// Directional lightning
	Ogre::Light* light = m_pSceneManager->createLight();
	Ogre::SceneNode* lightNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
}

