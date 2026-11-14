#pragma once
#include "base/ccConfig.h"
#include "imgui_lua.hpp"

extern int luaReg_imguiColorTextEdit_imguiColorTextEdit(lua_State* lua_S);

inline int luaReg_imguiColorTextEdit(lua_State* lua_S)
{
	luaReg_imguiColorTextEdit_imguiColorTextEdit(lua_S);
	return 0;
}
