#pragma once

#include <string>

#include "../Input/InputHandler.h"
#include "../Render/RenderNode.h"

//#include "LuaBridge.h"
#include "flecs.h"
#include "../FileSystem/GEFile.h"

class ScriptSystem
{
public:
	ScriptSystem(InputHandler* pInputHandler, std::string strScriptsRoot);
	~ScriptSystem();
	ScriptSystem(const ScriptSystem&) = delete;

	ScriptSystem& operator=(const ScriptSystem&) = delete;


private:
	std::string m_strScriptsRoot;
	InputHandler* m_pInputHandler;
};

