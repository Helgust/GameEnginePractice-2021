#pragma once

#include "Input.h"

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

	void Update();

	bool GetQuit() const { return m_bIsQuit; };
	const std::bitset<eIC_Max>& GetInputState() const;
	bool IsCommandActive(EInputCommand inputCommand) const;

	Ogre::Vector2 MousePos() const;
	Ogre::Vector2 DeltaMousePos() const;
	Ogre::Vector2 DeltaDownMousePos() const;
	float GetMouseSensitivity() const { return m_pMouseSensitivity; }

	void SetWinHandle(HWND window);

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

	void FillSymbolMap();
	void FillCommandMap();
	void FillCommandSymbolMap();

	std::string m_strMapFilePath;

	typedef std::unordered_map<std::string, size_t> TCommandMap;
	typedef std::unordered_map<std::string, int> TSymbolMap;
	typedef std::unordered_map<int, size_t> TInputEventMap;
	typedef std::unordered_map<std::string, std::string> TCommandSymbolMap;

	TCommandMap m_commandMap;
	TSymbolMap m_symbolMap;
	TInputEventMap m_inputEventMap;
	TCommandSymbolMap m_commandSymbolMap;

	std::bitset<eIC_Max> m_InputState;

	bool m_bLMouseButtonDown;
	bool m_bRMouseButtonDown;

	POINT m_pMousePoint;
	Ogre::Vector2 m_pCurMousePos;
	Ogre::Vector2 m_pPrevMousePos;
	float m_pMouseSensitivity;
};

