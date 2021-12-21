#include "InputHandler.h"
#include "Input.h"
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

	Ogre::ConfigFile cf;
	cf.load(m_strMapFilePath);

	Ogre::ConfigFile::SectionIterator secIter = cf.getSectionIterator();

	for (int i = 0; i < INPUTLIST.size(); i++) {
		if (!INPUTLIST[i].empty()) {
			m_key2code[INPUTLIST[i]] = SDL_Scancode(i);
		}
	}

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

			m_command2key[strCommand] = strSymbol;
		}
	}
}

InputHandler::~InputHandler()
{
}


// We used int as return type just for demonstration. It should be done another way
void InputHandler::Update(SDL_Window* window)
{
	SDL_Event ev;
	SDL_Scancode key;
	int width, height;

	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_KEYDOWN:
			key = ev.key.keysym.scancode;
			m_keyState[key] = true;
			break;

		case SDL_KEYUP:
			key = ev.key.keysym.scancode;
			m_keyState[key] = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_mouseState[ev.button.button] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			m_mouseState[ev.button.button] = false;
			break;
		case SDL_QUIT:
			m_bIsQuit = true;
			break;
		default:
			break;
		}
	}
}
bool InputHandler::GetCommand(std::string command) const
{
	std::string key;
	SDL_Scancode code;
	if (m_command2key.find(command) != m_command2key.end())
		key = m_command2key.at(command);
	else
		return false;
	if (m_key2code.find(key) != m_key2code.end())
		code = m_key2code.at(key);
	else
		return false;
	return m_keyState.find(code) != m_keyState.end() ? m_keyState.at(code) : false;
}
