#include "RenderEngine.h"

#include "ProjectDefines.h"

RenderEngine::RenderEngine() :
	m_pRoot(nullptr),
	m_pRenderWindow(nullptr),
	m_pSceneManager(nullptr),
	m_pD3D11Plugin(nullptr),
	m_pCamera(nullptr),
	m_pWorkspace(nullptr),
	m_pRT(nullptr),
	m_bQuit(false)
{
	m_pRT = new RenderThread(this);

	m_pRT->RC_Init();
	m_pRT->RC_SetupDefaultCamera();
	m_pRT->RC_SetupDefaultCompositor();
	m_pRT->RC_LoadDefaultResources();
	m_pRT->RC_LoadMesh("sphere");
	
	for (int i = 1; i < 10; i++)
	{
		m_pRT->RC_CreateSphere(i,6);
	}
	m_pRT->RC_CreateSphere(0, 10);
	m_pRT->RC_SetupDefaultLight();

	m_pRT->Start();
}

RenderEngine::~RenderEngine()
{
	SAFE_OGRE_DELETE(m_pRoot);
	SAFE_OGRE_DELETE(m_pRenderWindow);
	SAFE_OGRE_DELETE(m_pSceneManager);
	SAFE_OGRE_DELETE(m_pD3D11Plugin);
	SAFE_OGRE_DELETE(m_pCamera);
	SAFE_OGRE_DELETE(m_pWorkspace);
	SAFE_OGRE_DELETE(m_pRT);
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

	if (m_pRenderWindow->isVisible())
		m_bQuit |= !m_pRoot->renderOneFrame();
}

void RenderEngine::RT_Init()
{
	m_pRoot = OGRE_NEW Ogre::Root();
	m_pD3D11Plugin = OGRE_NEW Ogre::D3D11Plugin();

	m_pRoot->installPlugin(m_pD3D11Plugin);

	if (!SetOgreConfig())
	{
		m_bQuit = true;
		return;
	}

	m_pRoot->initialise(false);

	// Creating window
	Ogre::uint32 width = 1280;
	Ogre::uint32 height = 720;
	Ogre::String sTitleName = "Game Engine";

	m_pRenderWindow = Ogre::Root::getSingleton().createRenderWindow(sTitleName, width, height, false);

	// Scene manager
	m_pSceneManager = m_pRoot->createSceneManager(Ogre::SceneType::ST_GENERIC, 1);
}

void RenderEngine::RT_SetupDefaultCamera()
{
	m_pCamera = m_pSceneManager->createCamera("Main Camera");

	m_pCamera->setPosition(Ogre::Vector3(0, 2000, 1));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(0.2f);
	m_pCamera->setFarClipDistance(10000.0f);
	m_pCamera->setAutoAspectRatio(true);
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
	Ogre::ConfigFile cf;
	cf.load("resources2.cfg");

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();

		if (secName != "Hlms")
		{
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
			}
		}
	}

	LoadHlms(cf);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
}

void RenderEngine::LoadHlms(Ogre::ConfigFile& cf)
{
	// Load hlms (high level material system) files
	Ogre::String rootHlmsFolder = cf.getSetting("DoNotUseAsResource", "Hlms", "");

	if (rootHlmsFolder.empty())
		rootHlmsFolder = "./";
	else if (*(rootHlmsFolder.end() - 1) != '/')
		rootHlmsFolder += "/";

	//At this point rootHlmsFolder should be a valid path to the Hlms data folder

	Ogre::HlmsUnlit* hlmsUnlit = nullptr;
	Ogre::HlmsPbs* hlmsPbs = nullptr;

	//For retrieval of the paths to the different folders needed
	Ogre::String mainFolderPath;
	Ogre::StringVector libraryFoldersPaths;
	Ogre::StringVector::const_iterator libraryFolderPathIt;
	Ogre::StringVector::const_iterator libraryFolderPathEn;

	Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

	{
		//Create & Register HlmsUnlit
		//Get the path to all the subdirectories used by HlmsUnlit
		Ogre::HlmsUnlit::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
		Ogre::Archive* archiveUnlit = archiveManager.load(rootHlmsFolder + mainFolderPath,
			"FileSystem", true);
		Ogre::ArchiveVec archiveUnlitLibraryFolders;
		libraryFolderPathIt = libraryFoldersPaths.begin();
		libraryFolderPathEn = libraryFoldersPaths.end();
		while (libraryFolderPathIt != libraryFolderPathEn)
		{
			Ogre::Archive* archiveLibrary =
				archiveManager.load(rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true);
			archiveUnlitLibraryFolders.push_back(archiveLibrary);
			++libraryFolderPathIt;
		}

		//Create and register the unlit Hlms
		hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &archiveUnlitLibraryFolders);
		Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);
	}

	{
		//Create & Register HlmsPbs
		//Do the same for HlmsPbs:
		Ogre::HlmsPbs::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
		Ogre::Archive* archivePbs = archiveManager.load(rootHlmsFolder + mainFolderPath,
			"FileSystem", true);

		//Get the library archive(s)
		Ogre::ArchiveVec archivePbsLibraryFolders;
		libraryFolderPathIt = libraryFoldersPaths.begin();
		libraryFolderPathEn = libraryFoldersPaths.end();
		while (libraryFolderPathIt != libraryFolderPathEn)
		{
			Ogre::Archive* archiveLibrary =
				archiveManager.load(rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true);
			archivePbsLibraryFolders.push_back(archiveLibrary);
			++libraryFolderPathIt;
		}

		//Create and register
		hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &archivePbsLibraryFolders);
		Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
	}


	Ogre::RenderSystem* renderSystem = m_pRoot->getRenderSystem();
	bool supportsNoOverwriteOnTextureBuffers;
	renderSystem->getCustomAttribute("MapNoOverwriteOnDynamicBufferSRV",
		&supportsNoOverwriteOnTextureBuffers);

	if (!supportsNoOverwriteOnTextureBuffers)
	{
		hlmsPbs->setTextureBufferDefaultSize(512 * 1024);
		hlmsUnlit->setTextureBufferDefaultSize(512 * 1024);
	}
}

void RenderEngine::RT_LoadOgreHead()
{
	//Load the v1 mesh. Notice the v1 namespace
	//Also notice the HBU_STATIC flag; since the HBU_WRITE_ONLY
	//bit would prohibit us from reading the data for importing.
	Ogre::v1::MeshPtr v1Mesh;
	Ogre::MeshPtr v2Mesh;

	v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
		"ogrehead.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);

	//Create a v2 mesh to import to, with a different name (arbitrary).
	v2Mesh = Ogre::MeshManager::getSingleton().createManual(
		"ogrehead.mesh Imported", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	bool halfPosition = true;
	bool halfUVs = true;
	bool useQtangents = true;

	//Import the v1 mesh to v2
	v2Mesh->importV1(v1Mesh.get(), halfPosition, halfUVs, useQtangents);

	//We don't need the v1 mesh. Free CPU memory, get it out of the GPU.
	//Leave it loaded if you want to use athene with v1 Entity.
	v1Mesh->unload();

	//Create an Item with the model we just imported.
	//Notice we use the name of the imported model. We could also use the overload
	//with the mesh pointer:
	Ogre::Item* item = m_pSceneManager->createItem("ogrehead.mesh Imported",
		Ogre::ResourceGroupManager::
		AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::SCENE_DYNAMIC);
	Ogre::SceneNode* sceneNode = m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		createChildSceneNode(Ogre::SCENE_DYNAMIC);
	sceneNode->attachObject(item);
	sceneNode->scale(0.1f, 0.1f, 0.1f);
}

void RenderEngine::RT_LoadMesh(const Ogre::String& _meshName)
{
	//Load the v1 mesh. Notice the v1 namespace
	//Also notice the HBU_STATIC flag; since the HBU_WRITE_ONLY
	//bit would prohibit us from reading the data for importing.
	Ogre::v1::MeshPtr v1Mesh;
	Ogre::MeshPtr v2Mesh;
	Ogre::String meshName = "sphere";


	v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
		meshName+".mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);

	//Create a v2 mesh to import to, with a different name (arbitrary).
	v2Mesh = Ogre::MeshManager::getSingleton().createManual(
		meshName + ".mesh Imported", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	bool halfPosition = true;
	bool halfUVs = true;
	bool useQtangents = true;

	//Import the v1 mesh to v2
	v2Mesh->importV1(v1Mesh.get(), halfPosition, halfUVs, useQtangents);

	//We don't need the v1 mesh. Free CPU memory, get it out of the GPU.
	//Leave it loaded if you want to use athene with v1 Entity.
	v1Mesh->unload();

	//Create an Item with the model we just imported.
	//Notice we use the name of the imported model. We could also use the overload
	//with the mesh pointer:
	Ogre::Item* item = m_pSceneManager->createItem(meshName+".mesh Imported",
		Ogre::ResourceGroupManager::
		AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::SCENE_DYNAMIC);
	Ogre::SceneNode* sceneNode = m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		createChildSceneNode(Ogre::SCENE_DYNAMIC);
	item->setVisible(false);
	sceneNode->setName("Sphere Node");
	sceneNode->attachObject(item);
	sceneNode->setVisible(false);
	//sceneNode->scale(0.1f, 0.1f, 0.1f);
}

void RenderEngine::RT_CreateSphere(const size_t& index,float radius)
{
	Ogre::Item* item = m_pSceneManager->createItem( "sphere.mesh Imported",
		Ogre::ResourceGroupManager::
		AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::SCENE_DYNAMIC);
	Ogre::SceneNode* sceneNode = ((Ogre::SceneNode*)m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		getChild(0))->createChildSceneNode(Ogre::SCENE_DYNAMIC);
	sceneNode->attachObject(item);
	float coef = (radius*0.3f)/10.0f;
	sceneNode->scale(coef, coef, coef);
}

void RenderEngine::RT_SetupDefaultLight()
{
	// Lightning
	Ogre::Light* light = m_pSceneManager->createLight();
	Ogre::SceneNode* lightNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(0, -1, 0).normalisedCopy());
}

void RenderEngine::RT_OscillateCamera(float time)
{
	m_pCamera->setPosition(Ogre::Vector3(0, time, 15));
}

void RenderEngine::RT_MovePlanet(const UINT32& index, const Ogre::Vector3& pos)
{
	Ogre::Node* sceneNode = m_pSceneManager->getRootSceneNode()->getChild(0)->getChild(index);
	sceneNode->setPosition(pos);
}

