#pragma once
#include "base/ccConfig.h"
#include "imgui_lua.hpp"

extern int luaReg_imguiDrawList_imguiImDrawFlags(lua_State* lua_S);
extern int luaReg_imguiDrawList_imguiImDrawList(lua_State* lua_S);

inline int luaReg_imguiDrawList(lua_State* lua_S)
{
	luaReg_imguiDrawList_imguiImDrawFlags(lua_S);
	luaReg_imguiDrawList_imguiImDrawList(lua_S);
	return 0;
}
