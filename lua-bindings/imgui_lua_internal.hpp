#pragma once
#include "imgui_lua.hpp"

// Private helpers shared by the ImGui and ImPlot manual bindings.
ImVec2 _luaval_to_imvec2(lua_State* L, int lo);
ImVec4 _luaval_to_imvec4(lua_State* L, int lo);
ImU32 lua_tou32(lua_State* L, int lo);

#define lua_opt_value(_N, _IDX, _F, _DEF) _N >= _IDX ? _F(L, _IDX) : _DEF
#define lua_opt_number(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, luaL_checknumber, _DEF)
#define lua_opt_int(_N, _IDX, _DEF) (int)(lua_opt_value(_N, _IDX, luaL_checkinteger, _DEF))
#define lua_opt_u32(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, luaL_checknumber, _DEF)
#define lua_opt_string(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, luaL_checkstring, _DEF)
#define lua_opt_bool(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, lua_toboolean, _DEF)
#define lua_opt_imv2(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, _luaval_to_imvec2, _DEF)
#define lua_opt_imv4(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, _luaval_to_imvec4, _DEF)
#define lua_opt_string_null(_N, _IDX, _DEF) (_N >= _IDX) ? ( lua_isnil(L, _IDX) ? nullptr : luaL_checkstring(L, _IDX) ) : _DEF
#define lua_param_error(_i) luaL_error(L, "invalid parameter #%d: %s", (_i), luaL_typename(L, (_i)))
#define lua_checknative(_L, _N, _V)\
	if(!luaval_to_native(_L, (_N), (_V)))\
		return luaL_error(L, "invalid parameter #%d: %s", (_N), luaL_typename(_L, (_N)));


#define SHOW_WINDOW(_F) if (lua_gettop(L) == 0){\
	_F(); return 0; }\
bool open = lua_toboolean(L, 1);\
_F(&open);\
lua_pushboolean(L, open); return 1;
