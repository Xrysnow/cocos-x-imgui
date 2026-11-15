#pragma once

struct lua_State;

// Register automatic and manual ImPlot bindings, preserving the Lua stack.
void luaReg_implotManual(lua_State* L);
