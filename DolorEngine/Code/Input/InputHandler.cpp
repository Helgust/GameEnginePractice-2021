#include "InputHandler.h"
#include "../ProjectDefines.h"

#include "Ogre.h"

#include <algorithm>
#include <windows.h>

InputHandler::InputHandler(const std::string& strResourceRoot)
{
	m_pMouseSensitivity = 2;
	m_bLMouseButtonDown = false;
	m_bRMouseButtonDown = false;
	m_bIsQuit = false;
	m_strMapFilePath = strResourceRoot + "actionmapGame.ini";
	std::replace(m_strMapFilePath.begin(), m_strMapFilePath.end(), '\\', '/');

	MapSymbol("a", 'A');
	MapSymbol("d", 'D');
	MapSymbol("w", 'W');
	MapSymbol("s", 'S');

	MapSymbol("left", VK_LEFT);
	MapSymbol("right", VK_RIGHT);
	MapSymbol("up", VK_UP);
	MapSymbol("down", VK_DOWN);
	MapSymbol("lmb", VK_LBUTTON);
	MapSymbol("rmb", VK_RBUTTON);


	MapCommandSymbol("TurnLeft", eIC_TurnLeft, "a");
	MapCommandSymbol("TurnRight", eIC_TurnRight, "d");
	MapCommandSymbol("MoveForward", eIC_MoveForward, "w");
	MapCommandSymbol("MoveBack", eIC_MoveBack, "s");



	LoadConfiguration();

	Remap();
}

InputHandler::~InputHandler()
{
}

bool InputHandler::IsKeyDown(int vk_key)
{
	if (GetAsyncKeyState(vk_key) & 0x8000)
		return true;

	return false;
}

void InputHandler::MapSymbol(std::string strSymbol, int nSymbol)
{
	m_symbolMap[strSymbol] = nSymbol;
}

void InputHandler::MapInputEvent(int nSymbol, size_t nCommand)
{
	m_inputEventMap[nSymbol] = nCommand;
}

void InputHandler::MapCommandSymbol(std::string strCommand, size_t nCommand, std::string strDefaultSymbol)
{
	m_commandMap[strCommand] = nCommand;
	m_commandSymbolMap[strCommand] = strDefaultSymbol;
}

void InputHandler::LoadConfiguration()
{
	Ogre::ConfigFile cf;
	cf.load(m_strMapFilePath);

	Ogre::ConfigFile::SectionIterator secIter = cf.getSectionIterator();

	Ogre::String strInputType, strCommand, strSymbol;
	while (secIter.hasMoreElements())
	{
		strInputType = secIter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = secIter.getNext();

		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			strCommand = i->first;
			strSymbol = i->second;

			m_commandSymbolMap[strCommand] = strSymbol;
		}
	}
}

void InputHandler::Remap()
{
	for (auto& it : m_commandSymbolMap)
	{
		MapInputEvent(m_symbolMap[it.second], m_commandMap[it.first]);
	}
}

void InputHandler::ReadMappedButtonInput()
{
	for (auto& it : m_inputEventMap)
	{
		m_InputState.set(it.second, IsKeyDown(it.first));
	}
}

// We used int as return type just for demonstration. It should be done another way
void InputHandler::Update()
{
	ReadMappedButtonInput();
	ReadMouseInput();
}

void InputHandler::ReadMouseInput()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	m_pCurMousePos = Ogre::Vector2(x, y);
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{


		case SDL_MOUSEBUTTONDOWN:
			m_bLMouseButtonDown = event.button.button == SDL_BUTTON_LEFT;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				m_bLMouseButtonDown = false;
			}
			break;
		case SDL_QUIT:
			m_bIsQuit = true;
			break;
		default:
			break;
		}
		Ogre::LogManager::getSingleton().logMessage(std::to_string(m_bLMouseButtonDown));
	}
}

const std::bitset<eIC_Max>& InputHandler::GetInputState() const
{
	return m_InputState;
}

bool InputHandler::IsCommandActive(EInputCommand inputCommand) const
{
	//Ogre::LogManager::getSingleton().logMessage(std::to_string(inputCommand) + "_" + std::to_string(m_InputState.test(inputCommand)));
	return m_InputState.test(inputCommand);
}

Ogre::Vector2 InputHandler::MousePos() const
{
	return m_pCurMousePos;
}


