#include "WindowManager.h"
#include <filesystem>
#include <string>
#include <vector>

WindowManager::WindowManager(RenderEngine* pRenderEngine, EntityManager* pEntityManager, FileSystem* pFileSystem, LoadingSystem* pLoadingSystem) :
	m_bIsClosed(false),
	m_bIsFrozen(false),
	m_pCurrentSelection(nullptr),
	m_pRenderEngine(pRenderEngine),
	m_pEntityManager(pEntityManager),
	m_pFileSystem(pFileSystem),
	m_pLoadingSystem(pLoadingSystem)
{
	InitWindow();
}

WindowManager::~WindowManager()
{

}

void WindowManager::ProcessSDLInput() 
{
	SDL_Event event;
	bool processed;
	SDL_PumpEvents();
	std::vector<SDL_Event> notProcessedEvents;
	while (SDL_PollEvent(&event))
	{
		processed = false;
		if (event.window.windowID == SDL_GetWindowID(m_SDLWindow))
		{
			//w->SetImguiContext();
			ImGui_ImplSDL2_ProcessEvent(&event);
			processed = true;
		}
		if (!processed) {
			notProcessedEvents.push_back(event);
		}
		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
			m_bIsClosed = true;
	}
	
	for (auto ev : notProcessedEvents) {
		SDL_PushEvent(&ev);
	}
}

void WindowManager::InitWindow() 
{
	const char* glsl_version = "#version 130";

	m_SDLWindow = m_pRenderEngine->GetWindow();
	m_GLContext = SDL_GL_CreateContext(m_SDLWindow);
	SDL_GL_MakeCurrent(m_SDLWindow, m_GLContext);
	SDL_GL_SetSwapInterval(1);

	InitImGui();
}

void WindowManager::InitImGui() 
{
	const char* glsl_version = "#version 130";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureKeyboard = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_SDLWindow, m_GLContext);
	bool f = ImGui_ImplOpenGL3_Init(glsl_version);
}

void WindowManager::RenderGui()
{
	StartGuiUpdate();

	DisplayMenuBar();
	DisplayAllScripts();
	DisplaySelectionParameters();

	EndGuiUpdate();
}

void WindowManager::StartGuiUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_SDLWindow);
	ImGui::NewFrame();
}

void WindowManager::EndGuiUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WindowManager::Update() 
{
	SDL_GL_MakeCurrent(m_SDLWindow, m_GLContext);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ProcessSDLInput();
	
	RenderGui();
	
	SDL_GL_SwapWindow(m_SDLWindow);
}

void WindowManager::DisplayMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) { m_pLoadingSystem->SaveToXML(m_pLoadingSystem->GetCurrSaveFile()); }
			if (ImGui::MenuItem("Load", "")) {}
			if (ImGui::MenuItem("Export to Engine", "")) { int status = m_pLoadingSystem->ExportToEngine(); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void WindowManager::DisplaySelectionParameters()
{
	m_pCurrentSelection = m_pRenderEngine->GetCurrentSelection();

	if (m_pCurrentSelection)
	{
		ImGui::Begin("Parameters");
		ImGui::Text("Object Name:");
		ImGui::SameLine();
		ImGui::Text(m_pCurrentSelection->GetObjName().c_str());

		ImGui::Text("Object Id:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_pCurrentSelection->GetId()).c_str());
		

		static float posVec[3] = { 0.f, 0.f, 0.f };
		posVec[0] = float(m_pCurrentSelection->GetPosition().x);
		posVec[1] = float(m_pCurrentSelection->GetPosition().y);
		posVec[2] = float(m_pCurrentSelection->GetPosition().z);
		ImGui::InputFloat3("Position", posVec);
	
		static int rotVec[3] = { 0, 0, 0 };
		rotVec[0] = int(m_pCurrentSelection->GetOrientation().getPitch().valueDegrees());
		rotVec[1] = int(m_pCurrentSelection->GetOrientation().getYaw().valueDegrees());
		rotVec[2] = int(m_pCurrentSelection->GetOrientation().getRoll().valueDegrees());
		ImGui::InputInt3("Rotation", rotVec);

		m_pCurrentSelection->SetPosition(Ogre::Vector3(posVec[0], posVec[1], posVec[2]));
		//m_pRenderEngine->GetRT()->RC_LambdaAction([&] {m_pCurrentSelection->setPosition(posVec[0], posVec[1], posVec[2]); });
		
		std::vector<std::string> scriptList;
		int currScriptId = 0;
		int tempScriptId = 0;
		auto x = m_pEntityManager->GetEntityQueue()[m_pCurrentSelection->GetId()];
		std::string scriptIn = x.scriptName;
		for (const auto& entry : std::filesystem::directory_iterator(m_pFileSystem->GetScriptsRoot()))
		{
			if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".lua")
			{
				std::string tempStr = entry.path().filename().string();
				if (tempStr == scriptIn)
				{
					currScriptId = tempScriptId;
				}
				scriptList.push_back(tempStr);
				tempScriptId++;
			}
		}

	
		//ImGui::ShowDemoWindow();

		std::string scriptlabel = scriptList[currScriptId];
		if (ImGui::BeginCombo("Script", scriptlabel.c_str(), 0))
		{
			for (int n = 0; n < scriptList.size(); n++)
			{
				std::string selectedScriptName = scriptList[n];
				if (ImGui::Selectable(selectedScriptName.c_str()))
				{
					scriptlabel = scriptList[n];
					currScriptId = n;
					m_pEntityManager->SetNewScriptName(scriptlabel, m_pCurrentSelection->GetId());
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					
			}
			ImGui::EndCombo();
		}

		ImGui::End();
	
		Ogre::Quaternion q = m_pCurrentSelection->GetOrientation();
		int clampedX = std::clamp(rotVec[0], -180, 180);
		if (clampedX != int(m_pCurrentSelection->GetOrientation().getPitch().valueDegrees()))
		{
			Ogre::Radian radX = Ogre::Radian(Ogre::Degree(clampedX));
			Ogre::Radian offsetX = radX - m_pCurrentSelection->GetOrientation().getPitch();
			q.FromAngleAxis(offsetX, Ogre::Vector3::UNIT_X);
			q = q * m_pCurrentSelection->GetOrientation();
			m_pCurrentSelection->SetOrientation(q);
			return;
		}
	
		int clampedY = std::clamp(rotVec[1], -180, 180);
		if (clampedY != int(m_pCurrentSelection->GetOrientation().getYaw().valueDegrees()))
		{
			Ogre::Radian radY = Ogre::Radian(Ogre::Degree(clampedY));
			Ogre::Radian offsetY = radY - m_pCurrentSelection->GetOrientation().getYaw();
			q.FromAngleAxis(offsetY, Ogre::Vector3::UNIT_Y);
			q = q * m_pCurrentSelection->GetOrientation();
			m_pCurrentSelection->SetOrientation(q);
			return;
		}
	
		int clampedZ = std::clamp(rotVec[2], -180, 180);
		if (clampedZ != int(m_pCurrentSelection->GetOrientation().getRoll().valueDegrees()))
		{
			Ogre::Radian radZ = Ogre::Radian(Ogre::Degree(clampedZ));
			Ogre::Radian offsetZ = radZ - m_pCurrentSelection->GetOrientation().getRoll();
			q.FromAngleAxis(offsetZ, Ogre::Vector3::UNIT_Z);
			q = q * m_pCurrentSelection->GetOrientation();
			m_pCurrentSelection->SetOrientation(q);
			return;
		}
	}
}

void WindowManager::DisplayAllScripts()
{
	ImGui::Begin("Scripts");
	for (const auto& entry : std::filesystem::directory_iterator(m_pFileSystem->GetScriptsRoot()))
	{
		if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".lua")
		{
			std::string btnName = entry.path().filename().string();
			if (ImGui::Button(btnName.c_str()))
			{
				std::string command = "start notepad++ " + entry.path().string();
				std::system(command.c_str());
			}
		}
	}
	ImGui::End();
}

void WindowManager::DisplayFreezeBtn()
{
	ImGui::Begin("lol");
	ImGui::Checkbox("F", &m_bIsFrozen);
	ImGui::End();
}
