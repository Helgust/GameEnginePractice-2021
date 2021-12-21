#pragma once

#include <string>
#include <unordered_map>
#include <bitset>

#include "Ogre.h"

#include "SDL.h"

#include "../UI/imgui.h"
#include "../UI/imgui_impl_sdl.h"
#include "../UI/imgui_impl_opengl3.h"

class InputHandler
{
public:
	InputHandler(const std::string& strResourceRoot);
	~InputHandler();

	void Update(SDL_Window* window);

	bool Get(SDL_Scancode key) { return m_keyState.find(key) != m_keyState.end() ? m_keyState[key] : false; }
	bool GetCommand(std::string command) const;
	bool GetMouse(int key) { return m_mouseState.find(key) != m_mouseState.end() ? m_mouseState[key] : false; }

	bool GetQuit() { return m_bIsQuit; }

private:
	void LoadConfiguration();

	bool IsKeyDown(int vk_key);

	
	void MapInputEvent(int nSymbol, size_t nCommand);
	void MapSymbol(std::string strSymbol, int nSymbol);
	void MapCommandSymbol(std::string strCommand, size_t nCommand, std::string strDefaultSymbol);
	void Remap();

	void ReadMappedButtonInput();
	void ReadMouseInput();

	bool m_bIsQuit;
	std::string m_strMapFilePath;

	std::map<std::string, std::string> m_command2key;
	std::map<std::string, SDL_Scancode> m_key2code;
	std::map<SDL_Scancode, bool> m_keyState;

	std::map<int, bool> m_mouseState;

	bool m_bLMouseButtonDown;
	bool m_bRMouseButtonDown;
	bool m_bF8Down;

	POINT m_pMousePoint;
	Ogre::Vector2 m_pCurMousePos;
	Ogre::Vector2 m_pPrevMousePos;
	float m_pMouseSensitivity;
};

