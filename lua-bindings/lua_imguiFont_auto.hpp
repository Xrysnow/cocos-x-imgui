#pragma once
#include "base/ccConfig.h"
#include "imgui_lua.hpp"

extern int luaReg_imguiFont_imguiImFontConfig(lua_State* lua_S);
extern int luaReg_imguiFont_imguiImFontGlyph(lua_State* lua_S);
extern int luaReg_imguiFont_imguiImFontAtlasFlags(lua_State* lua_S);
extern int luaReg_imguiFont_imguiImFontAtlas(lua_State* lua_S);
extern int luaReg_imguiFont_imguiImFontBaked(lua_State* lua_S);
extern int luaReg_imguiFont_imguiImFont(lua_State* lua_S);

inline int luaReg_imguiFont(lua_State* lua_S)
{
	luaReg_imguiFont_imguiImFontConfig(lua_S);
	luaReg_imguiFont_imguiImFontGlyph(lua_S);
	luaReg_imguiFont_imguiImFontAtlasFlags(lua_S);
	luaReg_imguiFont_imguiImFontAtlas(lua_S);
	luaReg_imguiFont_imguiImFontBaked(lua_S);
	luaReg_imguiFont_imguiImFont(lua_S);
	return 0;
}
