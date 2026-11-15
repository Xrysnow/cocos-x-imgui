#include "imgui_lua.hpp"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "lua_imgui_auto.hpp"
#include "lua_imguiStyle_auto.hpp"
#include "lua_imguiIO_auto.hpp"
#include "lua_imguiFont_auto.hpp"
#include "lua_imguiViewport_auto.hpp"
#include "lua_imguiDrawList_auto.hpp"
#include "lua_ImGuiColorTextEdit_auto.hpp"
#include "implot_lua.hpp"
#include "imgui_lua_internal.hpp"
#include "../CCIMGUI.h"
#include "../CCImGuiLayer.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <type_traits>
#include <unordered_set>
#include <vector>

#pragma warning (disable: 4244)

using lua::luaval_to_native;
using lua::native_to_luaval;
using lua::LUA_TCDATA;

static_assert(sizeof(int64_t) == 8 && sizeof(ImGuiSelectionUserData) == sizeof(int64_t));
static_assert(std::numeric_limits<ImGuiSelectionUserData>::is_signed &&
	std::numeric_limits<ImGuiSelectionUserData>::digits == std::numeric_limits<int64_t>::digits);

static constexpr int64_t selection_max_safe_integer = 9007199254740991LL;

bool luaval_to_ImGuiSelectionToken(lua_State* L, int lo, ImGuiSelectionToken* outValue, const char* fName)
{
	LUA_CHECK_TO_NATIVE;
	const auto type = lua_type(L, lo);
	int64_t value = 0;
	if (type == LUA_TNUMBER)
	{
		const auto number = lua_tonumber(L, lo);
		if (!std::isfinite(number) || std::trunc(number) != number ||
			number < -selection_max_safe_integer || number > selection_max_safe_integer ||
			!luaval_to_native(L, lo, &value, fName))
			return false;
	}
	else if (type == LUA_TCDATA)
	{
		static constexpr auto read_int64 = R"__(
local ffi = require('ffi')
local signed = ffi.typeof('int64_t')
return function(value)
    if not ffi.istype(signed, value) then return nil end
    local copy = ffi.new('int64_t[1]')
    copy[0] = value
    return ffi.string(copy, 8)
end
)__";
		if (!lua::_try_get_internal_helper(L, "imgui.selection_userdata.read_int64", read_int64))
			return false;
		lua_pushvalue(L, lo);
		bool ok = lua_pcall(L, 1, 1, 0) == 0 && lua_type(L, -1) == LUA_TSTRING;
		size_t size = 0;
		const char* bytes = ok ? lua_tolstring(L, -1, &size) : nullptr;
		ok = bytes && size == sizeof(value);
		if (ok) std::memcpy(&value, bytes, sizeof(value));
		lua_settop(L, top);
		if (!ok) return false;
	}
	else
		return false;

	if (value == -1)
		return false;
	outValue->value = value;
	return true;
}

void ImGuiSelectionToken_to_luaval(lua_State* L, ImGuiSelectionToken value)
{
	if (!L) return;
	if (value.value >= -selection_max_safe_integer && value.value <= selection_max_safe_integer)
	{
		lua_pushnumber(L, static_cast<lua_Number>(value.value));
		return;
	}
	static constexpr auto push_int64 = R"__(
local ffi = require('ffi')
return function(bytes)
    local value = ffi.new('int64_t[1]')
    ffi.copy(value, bytes, 8)
    return value[0]
end
)__";
	if (!lua::_try_get_internal_helper(L, "imgui.selection_userdata.push_int64", push_int64))
	{
		luaL_error(L, "cannot initialize exact ImGuiSelectionUserData output");
		return;
	}
	// The native and FFI integers share the same process byte order; no double conversion.
	lua_pushlstring(L, reinterpret_cast<const char*>(&value.value), sizeof(value.value));
	if (lua_pcall(L, 1, 1, 0) != 0)
		lua_error(L);
}

ImVec2 _luaval_to_imvec2(lua_State *L, int lo)
{
	cocos2d::Vec2 v;
	ImVec2 vec;
	luaval_to_vec2(L, lo, &v);
	vec.x = v.x;
	vec.y = v.y;
	return vec;
}
ImVec4 _luaval_to_imvec4(lua_State *L, int lo)
{
	cocos2d::Vec4 v;
	ImVec4 vec;
	luaval_to_vec4(L, lo, &v);
	vec.x = v.x;
	vec.y = v.y;
	vec.z = v.z;
	vec.w = v.w;
	return vec;
}
ImU32 lua_tou32(lua_State *L, int lo)
{
	return (ImU32)luaL_checknumber(L, lo);
}
ImTextureRef _luaval_to_imTextureRef(lua_State *L, int lo)
{
	ImTextureRef v;
	luaval_to_ImTextureRef(L, lo, &v);
	return v;
}

bool luaval_to_ImTextureRef(lua_State* L, int lo, ImTextureRef* out, const char* name)
{
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, lo, "cc.Sprite", &sp, name))
	{
		*out = sp ? ImTextureRef{(ImTextureID)sp->getTexture()} : ImTextureRef{};
		return true;
	}
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, lo, "cc.Texture2D", &tex, name))
	{
		*out = tex ? ImTextureRef{(ImTextureID)tex} : ImTextureRef{};
		return true;
	}
	return false;
}

static void imgui_copyFontConfigName(ImFontConfig& config, const char* name, size_t length)
{
	const auto count = (std::min)(length, sizeof(config.Name) - 1);
	std::memcpy(config.Name, name, count);
	config.Name[count] = '\0';
}

static int imgui_setFontConfigName(lua_State* lua_S, int lo, ImFontConfig& config)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.Name/setter");
	std::string name;
	LUA_NATIVE_SETTER(name, lo);
	if (name.find('\0') != std::string::npos)
		return luaL_error(lua_S, "%s does not accept embedded NUL bytes", LUA_FNAME);
	imgui_copyFontConfigName(config, name.data(), name.size());
	return 0;
}

static ImFont* imgui_checkFont(lua_State* lua_S, int index)
{
	luaL_checkudata(lua_S, index, "imgui.ImFont");
	auto* font = LUA_TO_COBJ(ImFont*, index);
	if (!font) luaL_error(lua_S, "invalid imgui.ImFont");
	return font;
}

ImFontConfig lua_toImFontConfig(lua_State* L, int lo)
{
#define get_field(_K, _F)\
	lua_getfield(L, lo, #_K);\
	if(!lua_isnil(L, -1)) {\
		(&ret)->_K = _F(L, -1); }\
	lua_pop(L, 1);

	ImFontConfig ret;
	if (!lua_isnoneornil(L, lo))
	{
		// Read the same allowed fields from a configuration table or typed userdata.
		// Do not copy native source/loader pointers out of ImFontConfig.
		if (lua_type(L, lo) == LUA_TUSERDATA)
			luaL_checkudata(L, lo, "imgui.ImFontConfig");
		else
			luaL_checktype(L, lo, LUA_TTABLE);
		lua_getfield(L, lo, "Name");
		if (!lua_isnil(L, -1))
			imgui_setFontConfigName(L, -1, ret);
		lua_pop(L, 1);
		get_field(FontDataSize, luaL_checkinteger);
		get_field(FontDataOwnedByAtlas, lua_toboolean);
		get_field(FontNo, luaL_checkinteger);
		get_field(MergeMode, lua_toboolean);
		lua_getfield(L, lo, "DstFont");
		if (!lua_isnil(L, -1))
			ret.DstFont = imgui_checkFont(L, -1);
		lua_pop(L, 1);
		get_field(PixelSnapH, lua_toboolean);
		get_field(OversampleH, luaL_checkinteger);
		get_field(OversampleV, luaL_checkinteger);
		get_field(EllipsisChar, luaL_checkinteger);
		get_field(SizePixels, luaL_checknumber);
		get_field(GlyphOffset, _luaval_to_imvec2);
		get_field(GlyphMinAdvanceX, luaL_checknumber);
		get_field(GlyphMaxAdvanceX, luaL_checknumber);
		get_field(GlyphExtraAdvanceX, luaL_checknumber);
		get_field(FontNo, luaL_checkinteger);
		get_field(FontLoaderFlags, luaL_checkinteger);
		get_field(RasterizerMultiply, luaL_checknumber);
		get_field(RasterizerDensity, luaL_checknumber);
		get_field(ExtraSizeScale, luaL_checknumber);
		get_field(Flags, luaL_checkinteger);
	}
#undef get_field
	return ret;
}

// Main

static int imgui_showStyleEditor(lua_State *L) {
	ImGui::ShowStyleEditor();
    return 0;
}

// Demo, Debug, Information

static int imgui_showDemoWindow(lua_State *L) {
	SHOW_WINDOW(ImGui::ShowDemoWindow);
}
static int imgui_showAboutWindow(lua_State *L) {
	SHOW_WINDOW(ImGui::ShowAboutWindow);
}
static int imgui_showMetricsWindow(lua_State *L) {
	SHOW_WINDOW(ImGui::ShowMetricsWindow);
}
static int imgui_showDebugLogWindow(lua_State *L) {
	if (lua_gettop(L) > 1) return luaL_error(L, "showDebugLogWindow expects optional open");
	if (lua_isnoneornil(L, 1)) lua_settop(L, 0);
	else luaL_checktype(L, 1, LUA_TBOOLEAN);
	SHOW_WINDOW(ImGui::ShowDebugLogWindow);
}
static int imgui_showIDStackToolWindow(lua_State *L) {
	if (lua_gettop(L) > 1) return luaL_error(L, "showIDStackToolWindow expects optional open");
	if (lua_isnoneornil(L, 1)) lua_settop(L, 0);
	else luaL_checktype(L, 1, LUA_TBOOLEAN);
	SHOW_WINDOW(ImGui::ShowIDStackToolWindow);
}

// Styles

static int imgui_styleColorsDark(lua_State *L) {
	ImGui::StyleColorsDark();
	return 0;
}
static int imgui_styleColorsClassic(lua_State *L) {
	ImGui::StyleColorsClassic();
	return 0;
}
static int imgui_styleColorsLight(lua_State *L) {
	ImGui::StyleColorsLight();
	return 0;
}

// Windows

static int imgui_begin(lua_State *L) {
	const int args = lua_gettop(L);
	bool* p_open = nullptr;
	bool open;
	if (args >= 2)
	{
		// nil for nullptr
		if (!lua_isnil(L, 2))
		{
			open = lua_toboolean(L, 2);
			p_open = &open;
		}
	}
	lua_pushboolean(L,
		ImGui::Begin(luaL_checkstring(L, 1), p_open, lua_opt_int(args, 3, 0)));
	if(p_open)
	{
		lua_pushboolean(L, *p_open);
		return 2;
	}
	return 1;
}
static int imgui_end(lua_State *L) {
    ImGui::End();
    return 0;
}

// Child Windows

static int imgui_beginChild(lua_State *L) {
	const int args = lua_gettop(L);
	ImVec2 size = ImVec2(0, 0);
	bool border = false;
	ImGuiWindowFlags flags = 0;
	if (args > 1)
		size = _luaval_to_imvec2(L, 2);
	if (args > 2)
		border = lua_toboolean(L, 3);
	if (args > 3)
		flags = luaL_checkinteger(L, 4);

	switch (lua_type(L, 1)) {
        case LUA_TSTRING:
			lua_pushboolean(L, ImGui::BeginChild(luaL_checkstring(L, 1), size, border, flags));
			return 1;
        case LUA_TNUMBER:
			lua_pushboolean(L, ImGui::BeginChild(luaL_checkinteger(L, 1), size, border, flags));
			return 1;
        default: ;
    }
    return 0;
}
static int imgui_endChild(lua_State *L) {
    ImGui::EndChild();
    return 0;
}

// Parameters stacks (shared)

static int imgui_pushFont(lua_State* lua_S) {
	LUA_GINVOKE_HEADER("ImGui", "imgui.pushFont");
	LUA_TRY_INVOKE_R(2, (static_cast<void(*)(ImFont*, float)>(&ImGui::PushFont)));
	LUA_TRY_INVOKE_R(1, [](ImFont* font) { ImGui::PushFont(font, font ? font->LegacySize : 0.0f); });
	LUA_TRY_INVOKE_R(0, []() { ImGui::PushFont(nullptr, 0.0f); });
	LUA_GINVOKE_FOOTER("0,1,2");
}
static int imgui_getFont(lua_State *L) {
	object_to_luaval(L, "imgui.ImFont", ImGui::GetFont());
	return 1;
}
static int imgui_pushStyleColor(lua_State *L) {
	const auto idx = luaL_checkinteger(L, 1);
	switch (lua_type(L, 2))
	{
	case LUA_TNUMBER:
		ImGui::PushStyleColor(idx, lua_tou32(L, 2));
		break;
	case LUA_TTABLE:
		ImGui::PushStyleColor(idx, _luaval_to_imvec4(L, 2));
		break;
	default:
		break;
	}
	return 0;
}
static int imgui_pushStyleVar(lua_State *L) {
	const auto idx = luaL_checkinteger(L, 1);
	if (idx < ImGuiStyleVar_Alpha || idx >= ImGuiStyleVar_COUNT)
		return luaL_error(L, "invalid parameter #1");
	auto is_number = true;
	switch (idx)
	{
	//case ImGuiStyleVar_Alpha:					// float 
	case ImGuiStyleVar_WindowPadding:			// ImVec2
	//case ImGuiStyleVar_WindowRounding:		// float 
	//case ImGuiStyleVar_WindowBorderSize:		// float 
	case ImGuiStyleVar_WindowMinSize:			// ImVec2
	case ImGuiStyleVar_WindowTitleAlign:		// ImVec2
	//case ImGuiStyleVar_ChildRounding:			// float 
	//case ImGuiStyleVar_ChildBorderSize:		// float 
	//case ImGuiStyleVar_PopupRounding:			// float 
	//case ImGuiStyleVar_PopupBorderSize:		// float 
	case ImGuiStyleVar_FramePadding:			// ImVec2
	//case ImGuiStyleVar_FrameRounding:			// float 
	//case ImGuiStyleVar_FrameBorderSize:		// float 
	case ImGuiStyleVar_ItemSpacing:				// ImVec2
	case ImGuiStyleVar_ItemInnerSpacing:		// ImVec2
	//case ImGuiStyleVar_IndentSpacing:			// float
	case ImGuiStyleVar_CellPadding:             // ImVec2
	//case ImGuiStyleVar_ScrollbarSize:			// float 
	//case ImGuiStyleVar_ScrollbarRounding:		// float 
	//case ImGuiStyleVar_GrabMinSize:			// float 
	//case ImGuiStyleVar_GrabRounding:			// float 
	//case ImGuiStyleVar_TabRounding:			// float 
	case ImGuiStyleVar_ButtonTextAlign:			// ImVec2
	case ImGuiStyleVar_SelectableTextAlign:		// ImVec2
		is_number = false;
		break;
	default: ;
	}
	switch (lua_type(L, 2))
	{
	case LUA_TNUMBER:
		if(!is_number)
			return luaL_error(L, "invalid parameter #1");
		ImGui::PushStyleVar(idx, luaL_checknumber(L, 2));
		break;
	case LUA_TTABLE:
		if (is_number)
			return luaL_error(L, "invalid parameter #1");
		ImGui::PushStyleVar(idx, _luaval_to_imvec2(L, 2));
		break;
	default:
		break;
	}
	return 0;
}

// Widgets

static int imgui_image(lua_State *L) {
	const int args = lua_gettop(L);
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, 1, "cc.Texture2D", &tex))
	{
		if (!tex)
			return 0;
		CCIMGUI::getInstance()->image(tex,
			lua_opt_imv2(args, 2, ImVec2(0, 0)),
			lua_opt_imv2(args, 3, ImVec2(0, 0)),
			lua_opt_imv2(args, 4, ImVec2(1, 1)),
			lua_opt_imv4(args, 5, ImVec4(1, 1, 1, 1)),
			lua_opt_imv4(args, 6, ImVec4(0, 0, 0, 0)));
		return 0;
	}
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, 1, "cc.Sprite", &sp))
	{
		if (!sp)
			return 0;
		CCIMGUI::getInstance()->image(sp,
			lua_opt_imv2(args, 2, ImVec2(0, 0)),
			lua_opt_imv4(args, 3, ImVec4(1, 1, 1, 1)),
			lua_opt_imv4(args, 4, ImVec4(0, 0, 0, 0)));
		return 0;
	}
    return 0;
}
static int imgui_imageWithBg(lua_State *L) {
	const int args = lua_gettop(L);
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, 1, "cc.Texture2D", &tex))
	{
		if (!tex)
			return 0;
		CCIMGUI::getInstance()->imageWithBg(tex,
			lua_opt_imv2(args, 2, ImVec2(0, 0)),
			lua_opt_imv2(args, 3, ImVec2(0, 0)),
			lua_opt_imv2(args, 4, ImVec2(1, 1)),
			lua_opt_imv4(args, 5, ImVec4(0, 0, 0, 0)),
			lua_opt_imv4(args, 6, ImVec4(1, 1, 1, 1)));
		return 0;
	}
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, 1, "cc.Sprite", &sp))
	{
		if (!sp)
			return 0;
		CCIMGUI::getInstance()->imageWithBg(sp,
			lua_opt_imv2(args, 2, ImVec2(0, 0)),
			lua_opt_imv4(args, 3, ImVec4(0, 0, 0, 0)),
			lua_opt_imv4(args, 4, ImVec4(1, 1, 1, 1)));
		return 0;
	}
	return 0;
}
static int imgui_imageButton(lua_State *L) {
	// retrun 0 to hint an error
	const int args = lua_gettop(L);
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, 2, "cc.Texture2D", &tex))
	{
		if (!tex)
			return 0;
		lua_pushboolean(L,
			CCIMGUI::getInstance()->imageButton(
				luaL_checkstring(L, 1),
				tex,
				lua_opt_imv2(args, 3, ImVec2(0, 0)),
				lua_opt_imv2(args, 4, ImVec2(0, 0)),
				lua_opt_imv2(args, 5, ImVec2(1, 1)),
				lua_opt_imv4(args, 6, ImVec4(0, 0, 0, 0)),
				lua_opt_imv4(args, 7, ImVec4(1, 1, 1, 1)))
		);
		return 1;
	}
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, 2, "cc.Sprite", &sp))
	{
		if (!sp)
			return 0;
		lua_pushboolean(L,
			CCIMGUI::getInstance()->imageButton(
				luaL_checkstring(L, 1),
				sp,
				lua_opt_imv2(args, 3, ImVec2(0, 0)),
				lua_opt_imv4(args, 4, ImVec4(0, 0, 0, 0)),
				lua_opt_imv4(args, 5, ImVec4(1, 1, 1, 1))));
		return 1;
	}
	return 0;
}
static int imgui_collapsingHeader(lua_State *L) {
	const int args = lua_gettop(L);
	auto label = luaL_checkstring(L, 1);
	if (args == 1)
	{
		lua_pushboolean(L, ImGui::CollapsingHeader(label));
		return 1;
	}
	bool open;
	switch (lua_type(L, 2))
	{
	case LUA_TBOOLEAN:
		open = lua_toboolean(L, 2);
		lua_pushboolean(L,
			ImGui::CollapsingHeader(label, &open,
				lua_opt_int(args, 3, 0)));
		lua_pushboolean(L, open);
		return 2;
	case LUA_TNUMBER:
		lua_pushboolean(L,
			ImGui::CollapsingHeader(label, luaL_checkinteger(L, 2)));
		return 1;
	default:
		return 0;
	}
}
static int imgui_checkbox(lua_State *L) {
    bool v = lua_toboolean(L, 2);
    lua_pushboolean(L,
		ImGui::Checkbox(luaL_checkstring(L, 1), &v));
    lua_pushboolean(L, v);
    return 2;
}
static int imgui_checkboxFlags(lua_State *L) {
	auto label = luaL_checkstring(L, 1);
	auto flags = lua_tou32(L, 2);
    lua_pushboolean(L,
		ImGui::CheckboxFlags(label, &flags, lua_tou32(L, 3)));
    lua_pushinteger(L, flags);
    return 2;
}
static int imgui_radioButton(lua_State *L) {
	const int args = lua_gettop(L);
    if (args == 2) {
        lua_pushboolean(L,
			ImGui::RadioButton(luaL_checkstring(L, 1), lua_toboolean(L, 2)));
        return 1;
    } else if (args == 3) {
        int v = luaL_checkinteger(L, 2);
        lua_pushboolean(L,
			ImGui::RadioButton(luaL_checkstring(L, 1), &v, luaL_checkinteger(L, 3)));
        lua_pushinteger(L, v);
        return 2;
	}
    return 0;
}
static int imgui_combo(lua_State *L) {
	const int args = lua_gettop(L);
	auto label = luaL_checkstring(L, 1);
    int current_item = luaL_checkinteger(L, 2);
	std::vector<std::string> arr;
	luaval_to_std_vector_string(L, 3, &arr);
	std::vector<const char*> items;
	items.reserve(arr.size());
	for (auto&& str : arr)
		items.push_back(str.c_str());
    lua_pushboolean(L,
		ImGui::Combo(label, &current_item, items.data(), items.size(),
			lua_opt_int(args, 4, -1)));
    lua_pushinteger(L, current_item);
    return 2;
}

// Shared scalar widget bindings

namespace
{
enum class Widget { Drag, Slider, Vertical, Input };
constexpr double max_safe_integer = 9007199254740991.0;
constexpr int input_callback_flags = ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory |
	ImGuiInputTextFlags_CallbackAlways | ImGuiInputTextFlags_CallbackCharFilter |
	ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackEdit;

// The common arithmetic output converter uses lua_Number. Preserve wide integers
// with byte copies instead; neither direction converts wide cdata through double.
template<typename T>
bool read_scalar(lua_State* L, int index, T* out)
{
	if constexpr (std::is_integral_v<T> && sizeof(T) == 8) {
		if (lua_type(L, index) == lua::LUA_TCDATA) {
			const int top = lua_gettop(L);
			if (index < 0) index += top + 1;
			static constexpr auto code = R"__(
local ffi = require('ffi')
return function(value, signed)
    local name = signed and 'int64_t' or 'uint64_t'
    if not ffi.istype(name, value) then return nil end
    local copy = ffi.new(name .. '[1]')
    copy[0] = value
    return ffi.string(copy, 8)
end
)__";
			if (!lua::_try_get_internal_helper(L, "imgui.scalar.read64", code)) return false;
			lua_pushvalue(L, index);
			lua_pushboolean(L, std::is_signed_v<T>);
			bool ok = lua_pcall(L, 2, 1, 0) == 0 && lua_type(L, -1) == LUA_TSTRING;
			size_t size = 0;
			const char* bytes = ok ? lua_tolstring(L, -1, &size) : nullptr;
			ok = bytes && size == sizeof(T);
			if (ok) std::memcpy(out, bytes, sizeof(T));
			lua_settop(L, top);
			return ok;
		}
	}
	if (lua_type(L, index) != LUA_TNUMBER) return false;
	const double value = lua_tonumber(L, index);
	if (!std::isfinite(value)) return false;
	if constexpr (std::is_integral_v<T>) {
		const double upper = std::ldexp(1.0, std::numeric_limits<T>::digits);
		if (std::trunc(value) != value || value < (std::is_signed_v<T> ? -upper : 0.0) || value >= upper)
			return false;
		if constexpr (sizeof(T) == 8)
			if (std::abs(value) > max_safe_integer) return false;
	} else if (std::abs(value) > (std::numeric_limits<T>::max)()) return false;
	*out = static_cast<T>(value);
	return true;
}

template<typename T>
T check_scalar(lua_State* L, int index)
{
	// Typed widget values/bounds/steps need range checks and exact 64-bit conversion.
	T value{};
	if (!read_scalar(L, index, &value))
		luaL_error(L, "invalid scalar argument #%d: expected a finite value within the selected type's range", index);
	return value;
}

template<typename T>
T optional_scalar(lua_State* L, int index, T fallback = T{})
{
	return lua_isnoneornil(L, index) ? fallback : check_scalar<T>(L, index);
}

template<typename T>
void push_scalar(lua_State* L, T value)
{
	if constexpr (std::is_integral_v<T> && sizeof(T) == 8) {
		bool safe;
		if constexpr (std::is_signed_v<T>)
			safe = value >= -static_cast<T>(max_safe_integer) && value <= static_cast<T>(max_safe_integer);
		else safe = value <= static_cast<T>(max_safe_integer);
		if (!safe) {
			static constexpr auto code = R"__(
local ffi = require('ffi')
return function(bytes, signed)
    local value = ffi.new(signed and 'int64_t[1]' or 'uint64_t[1]')
    ffi.copy(value, bytes, 8)
    return value[0]
end
)__";
			if (!lua::_try_get_internal_helper(L, "imgui.scalar.push64", code))
				luaL_error(L, "cannot initialize exact scalar output");
			lua_pushlstring(L, reinterpret_cast<const char*>(&value), sizeof(T));
			lua_pushboolean(L, std::is_signed_v<T>);
			if (lua_pcall(L, 2, 1, 0) != 0) lua_error(L);
			return;
		}
	}
	lua::native_to_luaval(L, value);
}

// Read the raw 1-based prefix, ignoring extra keys and stopping at the first nil.
// The temporary buffer is Lua-owned, so a Lua error during
// conversion/output cannot bypass a vector destructor and leak its allocation.
template<typename T>
T* read_array(lua_State* L, int index, int* count)
{
	luaL_checktype(L, index, LUA_TTABLE);
	const size_t size = lua_objlen(L, index);
	if (size > static_cast<size_t>((std::numeric_limits<int>::max)()) || size > SIZE_MAX / sizeof(T))
		luaL_error(L, "scalar array is too large");
	*count = static_cast<int>(size);
	auto* data = static_cast<T*>(lua_newuserdata(L, size * sizeof(T)));
	for (int i = 0; i < *count; ++i) {
		lua_rawgeti(L, index, i + 1);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			*count = i;
			break;
		}
		if (!read_scalar(L, -1, data + i))
			luaL_error(L, "invalid scalar array element #%d", i + 1);
		lua_pop(L, 1);
	}
	return data;
}

template<typename T>
void check_slider_bounds(lua_State* L, T low, T high)
{
	// These are native SliderBehavior limits, including for vertical sliders.
	const T min = (std::numeric_limits<T>::lowest)();
	const T max = (std::numeric_limits<T>::max)();
	if (low > high) luaL_error(L, "slider minimum must not exceed maximum");
	if constexpr (std::is_floating_point_v<T> || sizeof(T) >= 4)
		if (low < min / 2 || high > max / 2)
			luaL_error(L, "slider bounds must stay within half the selected type's range");
}

template<Widget W, bool Array, typename T, ImGuiDataType Type>
int scalar_widget(lua_State* L, int value_index, bool alias = false)
{
	constexpr bool int_input = W == Widget::Input && Type == ImGuiDataType_S32;
	const bool short_input = alias && int_input;
	const int tail = W == Widget::Drag ? 5 : (short_input ? 3 : 4);
	const int required = (W == Widget::Slider || W == Widget::Vertical) ? value_index + 2 : value_index;
	if (lua_gettop(L) < required || lua_gettop(L) > value_index + tail)
		return luaL_error(L, "wrong number of scalar widget arguments");
	const char* label = luaL_checkstring(L, 1);
	ImVec2 size;
	if constexpr (W == Widget::Vertical) {
		if (!lua::luaval_to_native(L, 2, &size) || !std::isfinite(size.x) || !std::isfinite(size.y))
			return luaL_error(L, "invalid vertical slider size");
	}
	T low{}, high{};
	const T* p_low = nullptr;
	const T* p_high = nullptr;
	float speed = 1.0f;
	const char* format = nullptr;
	int flags = 0;
	if constexpr (W == Widget::Drag) {
		speed = (float)luaL_optnumber(L, value_index + 1, 1.0f);
		if (!lua_isnoneornil(L, value_index + 2)) { low = check_scalar<T>(L, value_index + 2); p_low = &low; }
		if (!lua_isnoneornil(L, value_index + 3)) { high = check_scalar<T>(L, value_index + 3); p_high = &high; }
		format = luaL_optstring(L, value_index + 4, nullptr);
		flags = luaL_optint(L, value_index + 5, 0);
	} else if constexpr (W == Widget::Input) {
		low = optional_scalar<T>(L, value_index + 1, short_input ? T(1) : T(0));
		high = optional_scalar<T>(L, value_index + 2, short_input ? T(100) : T(0));
		if (low < 0 || high < 0) return luaL_error(L, "input steps must be non-negative");
		if (low > 0) p_low = &low;
		if (high > 0) p_high = &high;
		flags = luaL_optint(L, value_index + (short_input ? 3 : 4), 0);
		if (short_input)
			format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
		else
			format = luaL_optstring(L, value_index + 3, alias ? (Type == ImGuiDataType_Double ? "%.6f" : "%.3f") : nullptr);
		if (flags & input_callback_flags) return luaL_error(L, "scalar input does not accept callback flags");
	} else {
		low = check_scalar<T>(L, value_index + 1);
		high = check_scalar<T>(L, value_index + 2);
		check_slider_bounds(L, low, high);
		p_low = &low;
		p_high = &high;
		format = luaL_optstring(L, value_index + 3, nullptr);
		flags = luaL_optint(L, value_index + 4, 0);
	}
	T value{};
	T* data = &value;
	int count = 1;
	if constexpr (Array) data = read_array<T>(L, value_index, &count);
	else value = check_scalar<T>(L, value_index);
	bool changed = false;
	if (count != 0) {
		if constexpr (W == Widget::Drag) {
			if constexpr (Array) changed = ImGui::DragScalarN(label, Type, data, count, speed, p_low, p_high, format, flags);
			else changed = ImGui::DragScalar(label, Type, data, speed, p_low, p_high, format, flags);
		} else if constexpr (W == Widget::Slider) {
			if constexpr (Array) changed = ImGui::SliderScalarN(label, Type, data, count, p_low, p_high, format, flags);
			else changed = ImGui::SliderScalar(label, Type, data, p_low, p_high, format, flags);
		} else if constexpr (W == Widget::Vertical) {
			changed = ImGui::VSliderScalar(label, size, Type, data, p_low, p_high, format, flags);
		} else {
			if constexpr (Array) changed = ImGui::InputScalarN(label, Type, data, count, p_low, p_high, format, flags);
			else changed = ImGui::InputScalar(label, Type, data, p_low, p_high, format, flags);
		}
	}
	lua_pushboolean(L, changed);
	if constexpr (Array) {
		lua_createtable(L, count, 0);
		for (int i = 0; i < count; ++i) {
			push_scalar(L, data[i]);
			lua_rawseti(L, -2, i + 1);
		}
	} else push_scalar(L, value);
	return 2;
}

template<Widget W, bool Array>
int dispatch_scalar(lua_State* L)
{
	constexpr int type_index = W == Widget::Vertical ? 3 : 2;
	const int type = luaL_checkint(L, type_index);
#define SCALAR_CASE(Name, T) case ImGuiDataType_##Name: return scalar_widget<W, Array, T, ImGuiDataType_##Name>(L, type_index + 1)
	switch (type) {
	SCALAR_CASE(S8, ImS8); SCALAR_CASE(U8, ImU8);
	SCALAR_CASE(S16, ImS16); SCALAR_CASE(U16, ImU16);
	SCALAR_CASE(S32, ImS32); SCALAR_CASE(U32, ImU32);
	SCALAR_CASE(S64, ImS64); SCALAR_CASE(U64, ImU64);
	SCALAR_CASE(Float, float); SCALAR_CASE(Double, double);
	default: return luaL_error(L, "unsupported ImGuiDataType");
	}
#undef SCALAR_CASE
}

template<Widget W, bool Array, typename T, ImGuiDataType Type>
int scalar_alias(lua_State* L)
{
	return scalar_widget<W, Array, T, Type>(L, W == Widget::Vertical ? 3 : 2, true);
}

#define GENERIC(Name, W, N) { #Name, dispatch_scalar<Widget::W, N> }
#define ALIAS(Name, W, N, T, Type) { #Name, scalar_alias<Widget::W, N, T, ImGuiDataType_##Type> }
const luaL_Reg methods[] = {
	GENERIC(dragScalar, Drag, false), GENERIC(dragScalarN, Drag, true),
	GENERIC(sliderScalar, Slider, false), GENERIC(sliderScalarN, Slider, true),
	GENERIC(vSliderScalar, Vertical, false),
	GENERIC(inputScalar, Input, false), GENERIC(inputScalarN, Input, true),
	ALIAS(dragFloat, Drag, false, float, Float), ALIAS(dragFloatN, Drag, true, float, Float),
	ALIAS(dragInt, Drag, false, ImS32, S32), ALIAS(dragIntN, Drag, true, ImS32, S32),
	ALIAS(sliderFloat, Slider, false, float, Float), ALIAS(sliderFloatN, Slider, true, float, Float),
	ALIAS(sliderInt, Slider, false, ImS32, S32), ALIAS(sliderIntN, Slider, true, ImS32, S32),
	ALIAS(vSliderFloat, Vertical, false, float, Float), ALIAS(vSliderInt, Vertical, false, ImS32, S32),
	ALIAS(inputFloat, Input, false, float, Float), ALIAS(inputFloatN, Input, true, float, Float),
	ALIAS(inputDouble, Input, false, double, Double), ALIAS(inputDoubleN, Input, true, double, Double),
	ALIAS(inputInt, Input, false, ImS32, S32), ALIAS(inputIntN, Input, true, ImS32, S32),
	{ nullptr, nullptr }
};
#undef ALIAS
#undef GENERIC
} // namespace

static void luaReg_imguiScalarManual(lua_State* L)
{
	luaL_register(L, "imgui", methods);
	lua_pop(L, 1);
}

// Widgets: Drags

static int imgui_dragFloatRange2(lua_State *L) {
	const char* label = luaL_checkstring(L, 1);
	float current_min = luaL_checknumber(L, 2);
	float current_max = luaL_checknumber(L, 3);
	lua_pushboolean(L,
		ImGui::DragFloatRange2(label, &current_min, &current_max,
			luaL_optnumber(L, 4, 1.0),
			luaL_optnumber(L, 5, 0.0),
			luaL_optnumber(L, 6, 0.0),
			luaL_optstring(L, 7, "%.3f"),
			luaL_optstring(L, 8, nullptr),
			luaL_optint(L, 9, 0)));
	lua_pushnumber(L, current_min);
	lua_pushnumber(L, current_max);
	return 3;
}
static int imgui_dragIntRange2(lua_State *L) {
	const char* label = luaL_checkstring(L, 1);
	int current_min = luaL_checkint(L, 2);
	int current_max = luaL_checkint(L, 3);
	lua_pushboolean(L,
		ImGui::DragIntRange2(label, &current_min, &current_max,
			(float)luaL_optnumber(L, 4, 1.0),
			luaL_optint(L, 5, 0),
			luaL_optint(L, 6, 0),
			luaL_optstring(L, 7, "%d"),
			luaL_optstring(L, 8, nullptr),
			luaL_optint(L, 9, 0)));
	lua_pushinteger(L, current_min);
	lua_pushinteger(L, current_max);
	return 3;
}

// Widgets: Sliders

static int imgui_sliderAngle(lua_State *L) {
	const int args = lua_gettop(L);
    float v_rad = (float)luaL_checknumber(L, 2);
	lua_pushboolean(L,
		ImGui::SliderAngle(luaL_checkstring(L, 1),
			&v_rad,
			lua_opt_number(args, 3, -360.0f),
			lua_opt_number(args, 4, 360.0f),
			lua_opt_string(args, 5, "%.0f deg")));
    lua_pushnumber(L, (lua_Number)v_rad);
    return 2;
}

// Widgets: Input with Keyboard

template<int Mode>
static int imgui_inputTextValue(lua_State* L) {
	const int value_index = Mode == 2 ? 3 : 2;
	const int flags_index = Mode == 0 ? 3 : 4;
	const int args = lua_gettop(L);
	if (args < value_index || args > flags_index)
		return luaL_error(L, "invalid text input argument count");
	const char* label = luaL_checkstring(L, 1);
	const char* hint = Mode == 2 ? luaL_checkstring(L, 2) : nullptr;
	size_t length = 0;
	const char* value = luaL_checklstring(L, value_index, &length);
	if (std::memchr(value, '\0', length))
		return luaL_error(L, "text input cannot contain embedded NUL bytes");
	const int flags = luaL_optint(L, flags_index, 0);
	if (flags & input_callback_flags) return luaL_error(L, "text input does not accept callback flags");
	ImVec2 size(0, 0);
	if constexpr (Mode == 1) {
		if (!lua_isnoneornil(L, 3)) { lua_checknative(L, 3, &size); }
	}
	// Finish argument validation before constructing the native editable string.
	std::string buf(value, length);
	bool changed;
	if constexpr (Mode == 0) changed = ImGui::InputText(label, &buf, flags);
	else if constexpr (Mode == 1) changed = ImGui::InputTextMultiline(label, &buf, size, flags);
	else changed = ImGui::InputTextWithHint(label, hint, &buf, flags);
	lua_pushboolean(L, changed);
	lua_pushlstring(L, buf.c_str(), buf.size());
	return 2;
}
static int imgui_inputText(lua_State* L) { return imgui_inputTextValue<0>(L); }
static int imgui_inputTextMultiline(lua_State* L) { return imgui_inputTextValue<1>(L); }
static int imgui_inputTextWithHint(lua_State* L) { return imgui_inputTextValue<2>(L); }

// Widgets: Color Editor/Picker

static int imgui_colorEdit3(lua_State *L) {
	const int args = lua_gettop(L);
	const char* label = luaL_checkstring(L, 1);
	std::vector<float> arr;
	luaval_to_std_vector_float(L, 2, &arr);
	arr.resize(3);
	lua_pushboolean(L,
		ImGui::ColorEdit3(label, arr.data(), lua_opt_int(args, 3, 0)));
	ccvector_float_to_luaval(L, arr);
	return 2;
}
static int imgui_colorEdit4(lua_State *L) {
	const int args = lua_gettop(L);
	const char* label = luaL_checkstring(L, 1);
	std::vector<float> arr;
	luaval_to_std_vector_float(L, 2, &arr);
	arr.resize(4);
	lua_pushboolean(L,
		ImGui::ColorEdit4(label, arr.data(), lua_opt_int(args, 3, 0)));
	ccvector_float_to_luaval(L, arr);
	return 2;
}
static int imgui_colorPicker3(lua_State *L) {
	const int args = lua_gettop(L);
	const char* label = luaL_checkstring(L, 1);
	std::vector<float> arr;
	luaval_to_std_vector_float(L, 2, &arr);
	arr.resize(3);
	lua_pushboolean(L,
		ImGui::ColorPicker3(label, arr.data(), lua_opt_int(args, 3, 0)));
	ccvector_float_to_luaval(L, arr);
	return 2;
}
static int imgui_colorPicker4(lua_State *L) {
	const int args = lua_gettop(L);
	const char* label = luaL_checkstring(L, 1);
	std::vector<float> arr;
	luaval_to_std_vector_float(L, 2, &arr);
	arr.resize(4);
	std::vector<float> ref;
	const float* pref = nullptr;
	if (args >= 4)
	{
		luaval_to_std_vector_float(L, 4, &ref);
		ref.resize(4);
		pref = ref.data();
	}
	lua_pushboolean(L,
		ImGui::ColorPicker4(label, arr.data(), lua_opt_int(args, 3, 0), pref));
	ccvector_float_to_luaval(L, arr);
	return 2;
}

// Widgets: Selectables

static int imgui_selectable(lua_State *L) {
	const int args = lua_gettop(L);
	const auto label = luaL_checkstring(L, 1);
	lua_pushboolean(L,
		ImGui::Selectable(label,
			lua_opt_bool(args, 2, false),
			lua_opt_int(args, 3, 0),
			lua_opt_imv2(args, 4, ImVec2(0, 0))));
	return 1;
}

// Widgets: List Boxes

static int imgui_listBox(lua_State *L) {
	const int args = lua_gettop(L);
	auto label = luaL_checkstring(L, 1);
	int current_item = luaL_checkinteger(L, 2);
	std::vector<std::string> arr;
	luaval_to_std_vector_string(L, 3, &arr);
	std::vector<const char*> items;
	items.reserve(arr.size());
	for (auto& str : arr)
		items.push_back(str.c_str());
	lua_pushboolean(L,
		ImGui::ListBox(label, &current_item, items.data(), items.size(),
			lua_opt_int(args, 4, -1)));
	lua_pushinteger(L, current_item);
	return 2;
}

// Widgets: Data Plotting

static int imgui_plotLines(lua_State *L) {
	const int args = lua_gettop(L);
	const auto count = lua_opt_int(args, 3, 0);
	std::vector<float> arr;
	luaval_to_std_vector_float(L, 2, &arr);
	ImGui::PlotLines(
		luaL_checkstring(L, 1),
		arr.data(),
		count > 0 ? std::min(count, (int)arr.size()) : (int)arr.size(),
		lua_opt_int(args, 4, 0),
		lua_opt_string_null(args, 5, nullptr),
		lua_opt_number(args, 6, FLT_MAX),
		lua_opt_number(args, 7, FLT_MAX),
		lua_opt_imv2(args, 8, ImVec2(0, 0)),
		lua_opt_int(args, 9, sizeof(float))
	);
	return 0;
}
static int imgui_plotHistogram(lua_State *L) {
	const int args = lua_gettop(L);
	const auto count = lua_opt_int(args, 3, 0);
	std::vector<float> arr;
	luaval_to_std_vector_float(L, 2, &arr);
	ImGui::PlotHistogram(
		luaL_checkstring(L, 1),
		arr.data(),
		count > 0 ? std::min(count, (int)arr.size()) : (int)arr.size(),
		lua_opt_int(args, 4, 0),
		lua_opt_string_null(args, 5, nullptr),
		lua_opt_number(args, 6, FLT_MAX),
		lua_opt_number(args, 7, FLT_MAX),
		lua_opt_imv2(args, 8, ImVec2(0, 0)),
		lua_opt_int(args, 9, sizeof(float))
	);
	return 0;
}

// Widgets: Menus

static int imgui_menuItem(lua_State *L) {
	const int args = lua_gettop(L);
	lua_pushboolean(L,
		ImGui::MenuItem(
			luaL_checkstring(L, 1),
			lua_opt_string_null(args, 2, nullptr),
			lua_opt_bool(args, 3, false),
			lua_opt_bool(args, 4, true)));
    return 1;
}

// Popups, Modals

static int imgui_beginPopupModal(lua_State *L) {
	const int args = lua_gettop(L);
	if (args <= 1)
	{
		lua_pushboolean(L,
			ImGui::BeginPopupModal(
				luaL_checkstring(L, 1)));
		return 1;
	}
	bool open = false;
	bool* p = nullptr;
	if (!lua_isnil(L, 2))
	{
		open = lua_toboolean(L, 2);
		p = &open;
	}
	lua_pushboolean(L,
		ImGui::BeginPopupModal(luaL_checkstring(L, 1), p, lua_opt_int(args, 3, 0)));
	if (p)
	{
		lua_pushboolean(L, open);
		return 2;
	}
	return 1;
}

// Tables

static int imgui_tableGetSortSpecs(lua_State* lua_S)
{
	LUA_GINVOKE_HEADER("ImGui", "imgui.tableGetSortSpecs");
	LUA_TRY_INVOKE_R(0, &ImGui::TableGetSortSpecs);
	LUA_GINVOKE_FOOTER("0");
}

static ImGuiTableSortSpecs* imgui_checkTableSortSpecs(lua_State* lua_S)
{
	luaL_checkudata(lua_S, 1, "imgui.ImGuiTableSortSpecs");
	auto cobj = LUA_TO_COBJ(ImGuiTableSortSpecs*, 1);
	if (!cobj)
		luaL_error(lua_S, "invalid imgui.ImGuiTableSortSpecs object");
	return cobj;
}

static int lua_ImGuiTableSortSpecs_Specs_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkTableSortSpecs(lua_S);
	lua::native_array_to_luaval(lua_S, cobj->Specs, cobj->SpecsCount);
	return 1;
}

static int lua_ImGuiTableSortSpecs_SpecsCount_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkTableSortSpecs(lua_S);
	LUA_PUSH_NATIVE(cobj->SpecsCount);
	return 1;
}

static int lua_ImGuiTableSortSpecs_readonly(lua_State* lua_S)
{
	// A null tolua setter would allow a Lua peer field to shadow the property.
	return luaL_error(lua_S, "imgui.ImGuiTableSortSpecs.Specs and SpecsCount are read-only");
}

static int lua_ImGuiTableSortSpecs_SpecsDirty_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkTableSortSpecs(lua_S);
	LUA_PUSH_NATIVE(cobj->SpecsDirty);
	return 1;
}

static int lua_ImGuiTableSortSpecs_SpecsDirty_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiTableSortSpecs.SpecsDirty/setter");
	auto cobj = imgui_checkTableSortSpecs(lua_S);
	LUA_NATIVE_SETTER(cobj->SpecsDirty, 2);
	return 0;
}

static int luaReg_imguiTableSortSpecs(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImGuiTableSortSpecs, "ImGuiTableSortSpecs", "imgui.ImGuiTableSortSpecs",
		nullptr, nullptr);
	LUA_FIELD("Specs", lua_ImGuiTableSortSpecs_Specs_getter, lua_ImGuiTableSortSpecs_readonly);
	LUA_FIELD("SpecsCount", lua_ImGuiTableSortSpecs_SpecsCount_getter, lua_ImGuiTableSortSpecs_readonly);
	LUA_FIELD("SpecsDirty", lua_ImGuiTableSortSpecs_SpecsDirty_getter, lua_ImGuiTableSortSpecs_SpecsDirty_setter);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}

// Multi-selection

static int imgui_setNextItemSelectionUserData(lua_State* lua_S)
{
	LUA_GINVOKE_HEADER("ImGui", "imgui.setNextItemSelectionUserData");
	LUA_TRY_INVOKE_R(1, [](ImGuiSelectionToken value) {
		ImGui::SetNextItemSelectionUserData(static_cast<ImGuiSelectionUserData>(value.value));
	});
	LUA_GINVOKE_FOOTER("1");
}

static ImGuiMultiSelectIO* imgui_checkMultiSelectIO(lua_State* lua_S)
{
	luaL_checkudata(lua_S, 1, "imgui.ImGuiMultiSelectIO");
	auto cobj = LUA_TO_COBJ(ImGuiMultiSelectIO*, 1);
	if (!cobj)
		luaL_error(lua_S, "invalid imgui.ImGuiMultiSelectIO object");
	return cobj;
}

static int lua_ImGuiMultiSelectIO_Requests_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkMultiSelectIO(lua_S);
	// Copy the whole request list before Lua allocations can run GC or reenter ImGui.
	std::vector<ImGuiSelectionRequest> requests;
	if (cobj->Requests.Size > 0)
		requests.assign(cobj->Requests.Data, cobj->Requests.Data + cobj->Requests.Size);
	lua::native_array_to_luaval(lua_S, requests.data(), requests.size());
	return 1;
}

static int lua_ImGuiMultiSelectIO_RangeSrcItem_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkMultiSelectIO(lua_S);
	LUA_PUSH_NATIVE(ImGuiSelectionToken{static_cast<int64_t>(cobj->RangeSrcItem)});
	return 1;
}

static int lua_ImGuiMultiSelectIO_NavIdItem_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkMultiSelectIO(lua_S);
	LUA_PUSH_NATIVE(ImGuiSelectionToken{static_cast<int64_t>(cobj->NavIdItem)});
	return 1;
}

static int lua_ImGuiMultiSelectIO_NavIdSelected_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkMultiSelectIO(lua_S);
	LUA_PUSH_NATIVE(cobj->NavIdSelected);
	return 1;
}

static int lua_ImGuiMultiSelectIO_ItemsCount_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkMultiSelectIO(lua_S);
	LUA_PUSH_NATIVE(cobj->ItemsCount);
	return 1;
}

static int lua_ImGuiMultiSelectIO_RangeSrcReset_getter(lua_State* lua_S)
{
	auto cobj = imgui_checkMultiSelectIO(lua_S);
	LUA_PUSH_NATIVE(cobj->RangeSrcReset);
	return 1;
}

static int lua_ImGuiMultiSelectIO_RangeSrcReset_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiMultiSelectIO.RangeSrcReset/setter");
	auto cobj = imgui_checkMultiSelectIO(lua_S);
	LUA_NATIVE_SETTER(cobj->RangeSrcReset, 2);
	return 0;
}

static int lua_ImGuiMultiSelectIO_readonly(lua_State* lua_S)
{
	return luaL_error(lua_S, "imgui.ImGuiMultiSelectIO: only RangeSrcReset is writable");
}

static int luaReg_imguiMultiSelectIO(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImGuiMultiSelectIO, "ImGuiMultiSelectIO", "imgui.ImGuiMultiSelectIO",
		nullptr, nullptr);
	LUA_FIELD("Requests", lua_ImGuiMultiSelectIO_Requests_getter, lua_ImGuiMultiSelectIO_readonly);
	LUA_FIELD("RangeSrcItem", lua_ImGuiMultiSelectIO_RangeSrcItem_getter, lua_ImGuiMultiSelectIO_readonly);
	LUA_FIELD("NavIdItem", lua_ImGuiMultiSelectIO_NavIdItem_getter, lua_ImGuiMultiSelectIO_readonly);
	LUA_FIELD("NavIdSelected", lua_ImGuiMultiSelectIO_NavIdSelected_getter, lua_ImGuiMultiSelectIO_readonly);
	LUA_FIELD("ItemsCount", lua_ImGuiMultiSelectIO_ItemsCount_getter, lua_ImGuiMultiSelectIO_readonly);
	LUA_FIELD("RangeSrcReset", lua_ImGuiMultiSelectIO_RangeSrcReset_getter, lua_ImGuiMultiSelectIO_RangeSrcReset_setter);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}

// Own the native storage and its adapter data together. Never copy a self-referencing adapter.
struct LuaImGuiSelectionBasicStorage
{
	ImGuiSelectionBasicStorage storage;
	std::vector<ImGuiID> ids;
	bool mapped = false;
	LuaImGuiSelectionBasicStorage()
	{
		storage.UserData = this;
		storage.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* native, int index) {
			auto* self = static_cast<LuaImGuiSelectionBasicStorage*>(native->UserData);
			return self->mapped ? self->ids[static_cast<size_t>(index)] : static_cast<ImGuiID>(index);
		};
	}
	LuaImGuiSelectionBasicStorage(const LuaImGuiSelectionBasicStorage&) = delete;
	LuaImGuiSelectionBasicStorage& operator=(const LuaImGuiSelectionBasicStorage&) = delete;
};

template<> struct lua::value_type<LuaImGuiSelectionBasicStorage> : std::true_type {};

static LuaImGuiSelectionBasicStorage* selection_check(lua_State* L, int argc)
{
	if (argc >= 0 && lua_gettop(L) != argc + 1)
		luaL_error(L, "selection storage expects %d arguments", argc);
	LuaImGuiSelectionBasicStorage* self = nullptr;
	if (!lua::read_value_pointer(L, 1, &self))
		luaL_error(L, "invalid imgui.ImGuiSelectionBasicStorage object");
	return self;
}

static bool selection_readId(lua_State* L, int index, ImGuiID* result)
{
	if (lua_type(L, index) != LUA_TNUMBER) return false;
	const auto value = lua_tonumber(L, index);
	if (!std::isfinite(value) || std::trunc(value) != value || value < 0 || value > UINT32_MAX)
		return false;
	*result = static_cast<ImGuiID>(value);
	return true;
}

static ImGuiID selection_checkId(lua_State* L, int index)
{
	ImGuiID id = 0;
	if (!selection_readId(L, index, &id))
		luaL_error(L, "selection ID must be an integer in [0, 4294967295]");
	return id;
}

// No Lua callbacks/metamethods during validation. Commit the replacement only after all checks.
static bool selection_readMapping(lua_State* L, LuaImGuiSelectionBasicStorage& self)
{
	if (lua_isnil(L, 2)) { self.ids.clear(); self.mapped = false; return true; }
	if (lua_type(L, 2) != LUA_TTABLE) return false;
	const size_t size = lua_objlen(L, 2);
	if (size > static_cast<size_t>((std::numeric_limits<int>::max)())) return false;
	size_t keys = 0;
	lua_pushnil(L);
	while (lua_next(L, 2))
	{
		ImGuiID index;
		const bool ok = selection_readId(L, -2, &index) && index >= 1 && index <= size;
		lua_pop(L, 1);
		if (!ok) { lua_pop(L, 1); return false; }
		++keys;
	}
	if (keys != size) return false;
	std::vector<ImGuiID> ids;
	std::unordered_set<ImGuiID> unique;
	ids.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		lua_rawgeti(L, 2, static_cast<int>(i + 1));
		ImGuiID id;
		const bool ok = selection_readId(L, -1, &id);
		lua_pop(L, 1);
		if (!ok || !unique.insert(id).second) return false;
		ids.push_back(id);
	}
	self.ids.swap(ids);
	self.mapped = true;
	return true;
}

static int selection_new(lua_State* lua_S)
{
	if (lua_gettop(lua_S) > 1 || (lua_gettop(lua_S) == 1 && !lua_istable(lua_S, 1)))
		return luaL_error(lua_S, "ImGuiSelectionBasicStorage expects no arguments");
	lua::push_value_owned(lua_S, new LuaImGuiSelectionBasicStorage());
	return 1;
}

static int selection_Size_getter(lua_State* lua_S)
{
	// tolua property getters receive both the object and the property key.
	auto* self = selection_check(lua_S, -1);
	LUA_PUSH_NATIVE(self->storage.Size);
	return 1;
}
static int selection_Size_setter(lua_State* lua_S)
{
	return luaL_error(lua_S, "ImGuiSelectionBasicStorage.Size is read-only");
}
static int selection_PreserveOrder_getter(lua_State* lua_S)
{
	auto* self = selection_check(lua_S, -1);
	LUA_PUSH_NATIVE(self->storage.PreserveOrder);
	return 1;
}
static int selection_PreserveOrder_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiSelectionBasicStorage.PreserveOrder/setter");
	auto* self = selection_check(lua_S, 1);
	LUA_NATIVE_SETTER(self->storage.PreserveOrder, 2);
	return 0;
}
static int selection_contains(lua_State* lua_S)
{
	auto* self = selection_check(lua_S, 1);
	LUA_PUSH_NATIVE(self->storage.Contains(selection_checkId(lua_S, 2)));
	return 1;
}
static int selection_setItemSelected(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiSelectionBasicStorage:setItemSelected");
	auto* self = selection_check(lua_S, 2);
	const auto id = selection_checkId(lua_S, 2);
	bool selected = false;
	LUA_NATIVE_SETTER(selected, 3);
	if (selected && !self->storage.Contains(id) && self->storage._SelectionOrder == (std::numeric_limits<int>::max)())
		return luaL_error(lua_S, "selection order exhausted; clear the storage first");
	self->storage.SetItemSelected(id, selected);
	return 0;
}
static int selection_clear(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImGuiSelectionBasicStorage", "imgui.ImGuiSelectionBasicStorage:clear");
	LUA_TRY_INVOKE_R(0, [](LuaImGuiSelectionBasicStorage& self) { self.storage.Clear(); });
	LUA_INVOKE_FOOTER("0");
}
static int selection_swap(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImGuiSelectionBasicStorage", "imgui.ImGuiSelectionBasicStorage:swap");
	LUA_TRY_INVOKE_R(1, ([](LuaImGuiSelectionBasicStorage& self, LuaImGuiSelectionBasicStorage& other) {
		self.storage.Swap(other.storage);
	}));
	LUA_INVOKE_FOOTER("1");
}
static int selection_setIndexToStorageIds(lua_State* lua_S)
{
	auto* self = selection_check(lua_S, 1);
	if (!selection_readMapping(lua_S, *self))
		return luaL_error(lua_S, "ID mapping must be nil or a dense array of unique uint32 IDs");
	return 0;
}
static int selection_getStorageIdFromIndex(lua_State* lua_S)
{
	auto* self = selection_check(lua_S, 1);
	const auto index = selection_checkId(lua_S, 2);
	if (index > static_cast<ImGuiID>((std::numeric_limits<int>::max)()) ||
		(self->mapped && index >= self->ids.size()))
		return luaL_error(lua_S, "selection index is outside the mapping/int range");
	LUA_PUSH_NATIVE(self->storage.GetStorageIdFromIndex(static_cast<int>(index)));
	return 1;
}

static const char* selection_validateRequests(const LuaImGuiSelectionBasicStorage& self, const ImGuiMultiSelectIO& io)
{
	if (io.ItemsCount < 0) return "applyRequests requires a known non-negative ItemsCount";
	if (self.mapped && static_cast<size_t>(io.ItemsCount) > self.ids.size())
		return "ID mapping does not cover ItemsCount";
	// Bound all native int order arithmetic before mutating anything.
	int64_t order = self.storage._SelectionOrder;
	for (const auto& request : io.Requests)
	{
		if (request.Type == ImGuiSelectionRequestType_SetAll)
			order = 1 + (request.Selected ? static_cast<int64_t>(io.ItemsCount) : 0);
		else if (request.Type == ImGuiSelectionRequestType_SetRange)
		{
			if (request.RangeFirstItem < 0 || request.RangeLastItem < request.RangeFirstItem ||
				request.RangeLastItem >= io.ItemsCount)
				return "SetRange endpoints must be ordered indices within ItemsCount";
			if (request.RangeDirection != 1 && request.RangeDirection != -1)
				return "SetRange direction must be +1 or -1";
			order += request.RangeLastItem - request.RangeFirstItem + 1;
		}
		else if (request.Type != ImGuiSelectionRequestType_None)
			return "unknown selection request type";
		if (order >= (std::numeric_limits<int>::max)())
			return "selection order exhausted; clear the storage first";
	}
	return nullptr;
}
static int selection_applyRequests(lua_State* lua_S)
{
	auto* self = selection_check(lua_S, 1);
	ImGuiMultiSelectIO* io = nullptr;
	if (!luaval_to_native(lua_S, 2, &io) || !io)
		return luaL_error(lua_S, "applyRequests expects a live ImGuiMultiSelectIO");
	if (const char* error = selection_validateRequests(*self, *io))
		return luaL_error(lua_S, "%s", error);
	self->storage.ApplyRequests(io);
	return 0;
}
static int selection_getSelectedItems(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImGuiSelectionBasicStorage", "imgui.ImGuiSelectionBasicStorage:getSelectedItems");
	LUA_TRY_INVOKE_R(0, [](const LuaImGuiSelectionBasicStorage& self) {
		std::vector<ImGuiStoragePair> records;
		for (const auto& record : self.storage._Storage.Data)
			if (record.val_i != 0) records.push_back(record);
		if (self.storage.PreserveOrder)
			std::stable_sort(records.begin(), records.end(), [](const ImGuiStoragePair& a, const ImGuiStoragePair& b) {
				return a.val_i < b.val_i;
			});
		std::vector<ImGuiID> snapshot;
		snapshot.reserve(records.size());
		for (const auto& record : records) snapshot.push_back(record.key);
		return snapshot;
	});
	LUA_INVOKE_FOOTER("0");
}
static int luaReg_imguiSelectionBasicStorage(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(LuaImGuiSelectionBasicStorage, "ImGuiSelectionBasicStorage", "imgui.ImGuiSelectionBasicStorage", nullptr, nullptr);
	LUA_METHOD("new", selection_new);
	LUA_METHOD(".call", selection_new);
	LUA_FIELD("Size", selection_Size_getter, selection_Size_setter);
	LUA_FIELD("PreserveOrder", selection_PreserveOrder_getter, selection_PreserveOrder_setter);
	LUA_METHOD("contains", selection_contains);
	LUA_METHOD("setItemSelected", selection_setItemSelected);
	LUA_METHOD("clear", selection_clear);
	LUA_METHOD("swap", selection_swap);
	LUA_METHOD("setIndexToStorageIds", selection_setIndexToStorageIds);
	LUA_METHOD("getStorageIdFromIndex", selection_getStorageIdFromIndex);
	LUA_METHOD("applyRequests", selection_applyRequests);
	LUA_METHOD("getSelectedItems", selection_getSelectedItems);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	lua::register_value_metatable<LuaImGuiSelectionBasicStorage>(lua_S, "imgui.ImGuiSelectionBasicStorage");
	return 0;
}

// Tab Bars, Tabs

static int imgui_beginTabItem(lua_State *L) {
	const int args = lua_gettop(L);
	if (args <= 1)
	{
		lua_pushboolean(L,
			ImGui::BeginTabItem(
				luaL_checkstring(L, 1)));
		return 1;
	}
	bool open = false;
	bool* p = nullptr;
	if (!lua_isnil(L, 2))
	{
		open = lua_toboolean(L, 2);
		p = &open;
	}
	lua_pushboolean(L,
		ImGui::BeginTabItem(luaL_checkstring(L, 1), p, lua_opt_int(args, 3, 0)));
	if (p)
	{
		lua_pushboolean(L, open);
		return 2;
	}
	return 1;
}

// Docking

static int imgui_dockSpace(lua_State* lua_S)
{
	if (lua_gettop(lua_S) == 4 && lua_isnil(lua_S, 4))
		lua_pop(lua_S, 1);
	LUA_GINVOKE_HEADER("ImGui", "imgui.dockSpace");
	LUA_TRY_INVOKE_R(4, ([](ImGuiID id, const ImVec2& size, ImGuiDockNodeFlags flags, const ImGuiWindowClass& window_class) {
		return ImGui::DockSpace(id, size, flags, &window_class);
	}));
	LUA_TRY_INVOKE_R(3, ([](ImGuiID id, const ImVec2& size, ImGuiDockNodeFlags flags) {
		return ImGui::DockSpace(id, size, flags);
	}));
	LUA_TRY_INVOKE_R(2, ([](ImGuiID id, const ImVec2& size) { return ImGui::DockSpace(id, size); }));
	LUA_TRY_INVOKE_R(1, [](ImGuiID id) { return ImGui::DockSpace(id); });
	LUA_GINVOKE_FOOTER("1,2,3,4");
}

static int imgui_dockSpaceOverViewport(lua_State* lua_S)
{
	if (lua_gettop(lua_S) == 4 && lua_isnil(lua_S, 4))
		lua_pop(lua_S, 1);
	LUA_GINVOKE_HEADER("ImGui", "imgui.dockSpaceOverViewport");
	LUA_TRY_INVOKE_R(4, ([](ImGuiID id, const ImGuiViewport* viewport, ImGuiDockNodeFlags flags, const ImGuiWindowClass& window_class) {
		return ImGui::DockSpaceOverViewport(id, viewport, flags, &window_class);
	}));
	LUA_TRY_INVOKE_R(3, ([](ImGuiID id, const ImGuiViewport* viewport, ImGuiDockNodeFlags flags) {
		return ImGui::DockSpaceOverViewport(id, viewport, flags);
	}));
	LUA_TRY_INVOKE_R(2, ([](ImGuiID id, const ImGuiViewport* viewport) { return ImGui::DockSpaceOverViewport(id, viewport); }));
	LUA_TRY_INVOKE_R(1, [](ImGuiID id) { return ImGui::DockSpaceOverViewport(id); });
	LUA_TRY_INVOKE_R(0, []() { return ImGui::DockSpaceOverViewport(); });
	LUA_GINVOKE_FOOTER("0,1,2,3,4");
}

static int imgui_setNextWindowClass(lua_State* lua_S)
{
	LUA_GINVOKE_HEADER("ImGui", "imgui.setNextWindowClass");
	LUA_TRY_INVOKE_R(1, [](const ImGuiWindowClass& window_class) { ImGui::SetNextWindowClass(&window_class); });
	LUA_GINVOKE_FOOTER("1");
}

// Drag and Drop TODO: release lua values in CCImGuiLayer?

static int payload_reg_key = 0;
static int imgui_setDragDropPayload(lua_State *L) {
	const int args = lua_gettop(L);
	auto type_str = luaL_checkstring(L, 1);
	void* p_key = &payload_reg_key;
	lua_pushlightuserdata(L, p_key);
	// ... key
	lua_gettable(L, LUA_REGISTRYINDEX);
	// ... t
	if (lua_type(L, -1) != LUA_TTABLE)
	{
		lua_pop(L, 1);
		lua_newtable(L);
		// ... t
		lua_pushlightuserdata(L, p_key);
		// ... t key
		lua_pushvalue(L, -2);
		// ... t key t
		lua_settable(L, LUA_REGISTRYINDEX);
		// ... t
	}
	// warning: the stored value cannot be released
	lua_pushvalue(L, 1);
	// ... t type
	lua_pushvalue(L, 2);
	// ... t type value
	lua_settable(L, -3);
	// ... t
	lua_pushboolean(L,
		ImGui::SetDragDropPayload(type_str, &p_key, sizeof(p_key), lua_opt_int(args, 3, 0)));
	return 1;
}
static int imgui_acceptDragDropPayload(lua_State *L) {
	const int args = lua_gettop(L);
	auto payload = ImGui::AcceptDragDropPayload(
		luaL_checkstring(L, 1), lua_opt_int(args, 2, 0));
	// note: return false if SetDragDropPayload is not called from lua
	if (!payload || payload->DataSize != sizeof(void*) || *(void**)payload->Data != &payload_reg_key)
	{
		lua_pushboolean(L, false);
		return 1;
	}
	lua_pushboolean(L, true);
	lua_pushlightuserdata(L, &payload_reg_key);
	// ... true key
	lua_gettable(L, LUA_REGISTRYINDEX);
	// ... true t
	if (lua_type(L, -1) != LUA_TTABLE)
	{
		lua_pushboolean(L, false);
		return 1;
	}
	lua_pushvalue(L, 1);
	// ... true t type
	lua_gettable(L, -2);
	// ... true value
	return 2;
}
static int imgui_getDragDropPayload(lua_State *L) {
	auto payload = ImGui::GetDragDropPayload();
	// note: return false if SetDragDropPayload is not called from lua
	if (!payload || payload->DataSize != sizeof(void*) || *(void**)payload->Data != &payload_reg_key)
	{
		lua_pushboolean(L, false);
		return 1;
	}
	lua_pushstring(L, payload->DataType);
	// ... type
	lua_pushboolean(L, true);
	lua_pushlightuserdata(L, &payload_reg_key);
	// ... type true key
	lua_gettable(L, LUA_REGISTRYINDEX);
	// ... type true t
	if (lua_type(L, -1) != LUA_TTABLE)
	{
		lua_pushboolean(L, false);
		return 1;
	}
	lua_pushvalue(L, -3);
	// ... type true t type
	lua_gettable(L, -2);
	// ... type true value
	return 2;
}

// Miscellaneous Utilities

static int imgui_calcListClipping(lua_State *L) {
	int start = 0;
	int end = 0;
	//TODO:
	//ImGui::CalcListClipping(luaL_checkint(L, 1), luaL_checknumber(L, 2), &start, &end);
	lua_pushinteger(L, start);
	lua_pushinteger(L, end);
	return 2;
}

// Color Utilities

static int imgui_colorConvertRGBtoHSV(lua_State* L) {
	float h = 0, s = 0, v = 0;
	ImGui::ColorConvertRGBtoHSV(
		(float)luaL_checknumber(L, 1),
		(float)luaL_checknumber(L, 2),
		(float)luaL_checknumber(L, 3),
		h, s, v);
	lua_pushinteger(L, h);
	lua_pushinteger(L, s);
	lua_pushinteger(L, v);
	return 3;
}

static int imgui_colorConvertHSVtoRGB(lua_State* L) {
	float r = 0, g = 0, b = 0;
	ImGui::ColorConvertHSVtoRGB(
		(float)luaL_checknumber(L, 1),
		(float)luaL_checknumber(L, 2),
		(float)luaL_checknumber(L, 3),
		r, g, b);
	lua_pushinteger(L, r);
	lua_pushinteger(L, g);
	lua_pushinteger(L, b);
	return 3;
}

// Inputs Utilities

static int imgui_isMousePosValid(lua_State *L) {
	if (lua_gettop(L) == 0)
	{
		lua_pushboolean(L, ImGui::IsMousePosValid());
		return 1;
	}
	auto mouse_pos = _luaval_to_imvec2(L, 1);
	lua_pushboolean(L, ImGui::IsMousePosValid(&mouse_pos));
	native_to_luaval(L, mouse_pos);
	return 2;
}

// ID scopes

static int imgui_pushID(lua_State *L) {
	const auto type = lua_type(L, 1);
    if (type == LUA_TNUMBER)
	    ImGui::PushID(luaL_checkinteger(L, 1));
    else if (type == LUA_TSTRING)
	{
		size_t size;
		const auto str = luaL_checklstring(L, 1, &size);
		ImGui::PushID(str, str + size);
    }
    return 0;
}

// IO

static int imgui_getIO(lua_State *L) {
	object_to_luaval(L, "imgui.ImGuiIO", &ImGui::GetIO());
	return 1;
}

// Font

enum GlyphRanges
{
	GlyphRangesDefault,                // Basic Latin, Extended Latin
	GlyphRangesKorean,                 // Default + Korean characters
	GlyphRangesJapanese,               // Default + Hiragana, Katakana, Half-Width, Selection of 1946 Ideographs
	GlyphRangesChineseFull,            // Default + Half-Width + Japanese Hiragana/Katakana + full set of about 21000 CJK Unified Ideographs
	GlyphRangesChineseSimplifiedCommon,// Default + Half-Width + Japanese Hiragana/Katakana + set of 2500 CJK Unified Ideographs for common simplified Chinese
	GlyphRangesCyrillic,               // Default + about 400 Cyrillic characters
	GlyphRangesThai,                   // Default + Thai characters
	GlyphRangesVietnamese,             // Default + Vietname characters
};
static const ImWchar* luaval_to_GlyphRanges(lua_State* L, int lo)
{
	auto& io = ImGui::GetIO();
	auto ranges = io.Fonts->GetGlyphRangesChineseFull();
	if (lua_istable(L, lo))
	{
		const size_t count = lua_objlen(L, lo);
		if (count > static_cast<size_t>((std::numeric_limits<int>::max)()))
			luaL_error(L, "glyph range array is too large");
		// Validate raw keys as well as values: Lua's length alone does not reject holes.
		size_t keys = 0;
		lua_pushnil(L);
		while (lua_next(L, lo) != 0)
		{
			const auto key = lua_tonumber(L, -2);
			if (lua_type(L, -2) != LUA_TNUMBER || !std::isfinite(key) ||
				std::trunc(key) != key || key < 1 || key > count)
				luaL_error(L, "glyph ranges must be a dense array");
			++keys;
			lua_pop(L, 1);
		}
		if (keys != count)
			luaL_error(L, "glyph ranges must be a dense array");
		size_t endpoints = count;
		lua_Number start = 0;
		for (size_t i = 0; i < count; ++i)
		{
			lua_rawgeti(L, lo, static_cast<int>(i + 1));
			const auto value = lua_tonumber(L, -1);
			if (lua_type(L, -1) != LUA_TNUMBER || !std::isfinite(value) ||
				std::trunc(value) != value || value < 0 || value > IM_UNICODE_CODEPOINT_MAX)
				luaL_error(L, "glyph range endpoints must be supported integer code points");
			if (value == 0)
			{
				if (i != count - 1 || i % 2 != 0)
					luaL_error(L, "glyph ranges only allow a final zero after complete pairs");
				--endpoints;
			}
			else if (i % 2 == 0) start = value;
			else if (value < start)
				luaL_error(L, "glyph range start exceeds its end");
			lua_pop(L, 1);
		}
		if (endpoints % 2 != 0)
			luaL_error(L, "glyph ranges require start/end pairs");
		std::vector<ImWchar> arr;
		arr.reserve(endpoints + 1);
		for (size_t i = 0; i < endpoints; ++i)
		{
			lua_rawgeti(L, lo, static_cast<int>(i + 1));
			arr.push_back(static_cast<ImWchar>(lua_tonumber(L, -1)));
			lua_pop(L, 1);
		}
		arr.push_back(0);
		// Immutable storage is keyed by content, never by a truncated debug name.
		const std::string key(reinterpret_cast<const char*>(arr.data()), arr.size() * sizeof(ImWchar));
		ranges = CCIMGUI::getInstance()->addGlyphRanges(key, arr);
	}
	else
	{
		const auto preset = lua_tonumber(L, lo);
		if (lua_type(L, lo) != LUA_TNUMBER || !std::isfinite(preset) ||
			std::trunc(preset) != preset || (int)preset < GlyphRangesDefault || (int)preset > GlyphRangesVietnamese)
			luaL_error(L, "invalid glyph range preset");
		switch (static_cast<GlyphRanges>(static_cast<int>(preset))) {
		case GlyphRangesDefault: ranges = io.Fonts->GetGlyphRangesDefault(); break;
		case GlyphRangesKorean: ranges = io.Fonts->GetGlyphRangesKorean(); break;
		case GlyphRangesJapanese: ranges = io.Fonts->GetGlyphRangesJapanese(); break;
		case GlyphRangesChineseFull: ranges = io.Fonts->GetGlyphRangesChineseFull(); break;
		case GlyphRangesChineseSimplifiedCommon: ranges = io.Fonts->GetGlyphRangesChineseSimplifiedCommon(); break;
		case GlyphRangesCyrillic: ranges = io.Fonts->GetGlyphRangesCyrillic(); break;
		case GlyphRangesThai: ranges = io.Fonts->GetGlyphRangesThai(); break;
		case GlyphRangesVietnamese: ranges = io.Fonts->GetGlyphRangesVietnamese(); break;
		default:;
		}
	}
	return ranges;
}
static int imgui_addFontTTF(lua_State *L) {
	const auto args = lua_gettop(L);
	if (args < 2 || args > 4)
		return luaL_error(L, "addFontTTF expects path, size, optional config and ranges");
	if (!ImGui::GetCurrentContext())
		return luaL_error(L, "addFontTTF requires an ImGui context");
	const float size_pixels = check_scalar<float>(L, 2);
	if (size_pixels < 0)
		return luaL_error(L, "font size must be non-negative");
	ImFontConfig cfg;
	if (args >= 3)
		cfg = lua_toImFontConfig(L, 3);
	if (!std::isfinite(cfg.SizePixels) || cfg.SizePixels < 0 ||
		!std::isfinite(cfg.RasterizerDensity) || cfg.RasterizerDensity <= 0)
		return luaL_error(L, "invalid font reference size or rasterizer density");
	const float reference_size = size_pixels > 0 ? size_pixels : cfg.SizePixels;
	if (reference_size == 0 && (cfg.GlyphOffset.x != 0 || cfg.GlyphOffset.y != 0 ||
		cfg.GlyphMinAdvanceX != 0 || cfg.GlyphMaxAdvanceX != (std::numeric_limits<float>::max)()))
		return luaL_error(L, "glyph offsets and advance limits require an explicit font size");
	auto* atlas = ImGui::GetIO().Fonts;
	if (!atlas || atlas->Locked)
		return luaL_error(L, "addFontTTF requires an unlocked font atlas");
	if (cfg.DstFont && !cfg.MergeMode)
		return luaL_error(L, "DstFont requires MergeMode = true");
	if (cfg.MergeMode)
	{
		if (atlas->Fonts.empty())
			return luaL_error(L, "MergeMode requires an existing font");
		if (!cfg.DstFont)
			cfg.DstFont = atlas->Fonts.back();
		// Compare against live atlas entries before dereferencing a borrowed font.
		if (!atlas->Fonts.contains(cfg.DstFont))
			return luaL_error(L, "DstFont must belong to the current font atlas");
		if (reference_size > 0 && (cfg.DstFont->Flags & ImFontFlags_ImplicitRefSize))
			return luaL_error(L, "an implicit-size destination requires an implicit-size merge source");
	}
	cfg.FontDataOwnedByAtlas = true;
	auto ranges = atlas->GetGlyphRangesChineseFull();
	if (args >= 4)
	{
		ranges = luaval_to_GlyphRanges(L, 4);
	}
	std::string path;
	if (!luaval_to_std_string(L, 1, &path))
		return 0;
	if (cfg.Name[0] == 0)
	{
		auto name = path + ", " + std::to_string(reference_size);
		const auto count = (std::min)(name.size(), sizeof(cfg.Name) - 1);
		imgui_copyFontConfigName(cfg, name.data() + name.size() - count, count);
	}
	auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(path);
	if (data.getSize() == 0)
		return 0;
	if (data.getSize() <= 100 || data.getSize() > (std::numeric_limits<int>::max)())
		return luaL_error(L, "invalid font data size");
	// Modern ImGui retains font bytes for later glyph loads; cocos Data is local.
	auto* fontData = ImGui::MemAlloc(data.getSize());
	if (!fontData)
		return luaL_error(L, "failed to allocate font data");
	std::memcpy(fontData, data.getBytes(), data.getSize());
	object_to_luaval(L, "imgui.ImFont",
		atlas->AddFontFromMemoryTTF(
			fontData, static_cast<int>(data.getSize()), size_pixels, &cfg, ranges));
	return 1;
}

// Style

static int imgui_getStyle(lua_State *L) {
	object_to_luaval(L, "imgui.ImGuiStyle", &ImGui::GetStyle());
	return 1;
}

static int imgui_setStyleColor(lua_State *L) {
	const auto idx = luaL_checkinteger(L, 1);
	if (idx < 0 || idx >= ImGuiCol_COUNT)
		return luaL_error(L, "invalid parameter #1");
	ImGui::GetStyle().Colors[idx] = _luaval_to_imvec4(L, 2);
	return 0;
}

// Settings/.Ini Utilities

static int imgui_saveIniSettingsToMemory(lua_State *L) {
	size_t out_ini_size = 0;
	const auto str = ImGui::SaveIniSettingsToMemory(&out_ini_size);
	lua_pushlstring(L, str, out_ini_size);
	return 1;
}

static int imgui_createLayer(lua_State *L) {
	object_to_luaval(L, "cc.Layer", ImGuiLayer::create());
	return 1;
}

static int imgui_shutdown(lua_State *L) {
	CCIMGUI::destroyInstance();
	return 0;
}

// cocos

static int imgui_ccNode(lua_State *L) {
	const int args = lua_gettop(L);
	cocos2d::Node* node = nullptr;
	if (luaval_to_object(L, 1, "cc.Node", &node))
	{
		if (!node)
			return 0;
		CCIMGUI::getInstance()->node(node,
			lua_opt_imv4(args, 2, ImVec4(1, 1, 1, 1)),
			lua_opt_imv4(args, 3, ImVec4(0, 0, 0, 0)));
		return 0;
	}
	return 0;
}
static int imgui_ccNodeButton(lua_State *L) {
	const int args = lua_gettop(L);
	cocos2d::Node* node = nullptr;
	if (luaval_to_object(L, 2, "cc.Node", &node))
	{
		if (!node)
			return 0;
		lua_pushboolean(L,
			CCIMGUI::getInstance()->nodeButton(
				luaL_checkstring(L, 1),
				node,
				lua_opt_imv4(args, 3, ImVec4(0, 0, 0, 0)),
				lua_opt_imv4(args, 4, ImVec4(1, 1, 1, 1)))
		);
		return 1;
	}
	return 0;
}
static int imgui_setCCNodeColor(lua_State *L)
{
	cocos2d::Node* node = nullptr;
	if (luaval_to_object(L, 1, "cc.Node", &node))
	{
		if (!node) return 0;
		const auto type = lua_type(L, 2);
		if (type == LUA_TNUMBER)
		{
			const auto idx = luaL_checkinteger(L, 2);
			if (idx < 0 || idx >= ImGuiCol_COUNT)
				return luaL_error(L, "invalid parameter #2");
			CCIMGUI::setNodeColor(node, idx);
		}
		else if (type == LUA_TTABLE)
		{
			CCIMGUI::setNodeColor(node, _luaval_to_imvec4(L, 2));
		}
	}
	return 0;
}
static int imgui_setCCLabelColor(lua_State *L)
{
	cocos2d::Label* label = nullptr;
	if (luaval_to_object(L, 1, "cc.Label", &label))
	{
		if (!label) return 0;
		const auto type = lua_type(L, 2);
		if (type == LUA_TNUMBER)
		{
			const auto idx = luaL_checkinteger(L, 2);
			if (idx < 0 || idx >= ImGuiCol_COUNT)
				return luaL_error(L, "invalid parameter #2");
			CCIMGUI::setLabelColor(label, (ImGuiCol)idx);
		}
		else if (type == LUA_TTABLE)
			CCIMGUI::setLabelColor(label, _luaval_to_imvec4(L, 2));
		else
			CCIMGUI::setLabelColor(label, lua_toboolean(L, 2) != 0);
	}
	return 0;
}

// markdown

static int imgui_setMarkdownLinkCallback(lua_State* L)
{
	if (lua_isnil(L, 1))
	{
		CCIMGUI::getInstance()->setMarkdownLinkCallback(nullptr);
		return 0;
	}
	luaL_checktype(L, 1, LUA_TFUNCTION);
	lua_pushlightuserdata(L, (void*)imgui_setMarkdownLinkCallback);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	auto f = [=](const std::string& p1, const std::string& p2, bool p3)
	{
		lua_pushlightuserdata(L, (void*)imgui_setMarkdownLinkCallback);
		lua_rawget(L, LUA_REGISTRYINDEX);
		// ... f
		lua_pushlstring(L, p1.c_str(), p1.size());
		lua_pushlstring(L, p2.c_str(), p2.size());
		lua_pushboolean(L, p3);
		// ... f, p1, p2, p3
		lua_call(L, 3, 0);
	};
	CCIMGUI::getInstance()->setMarkdownLinkCallback(f);
	return 0;
}
static int imgui_setMarkdownTooltipCallback(lua_State* L)
{
	if (lua_isnil(L, 1))
	{
		CCIMGUI::getInstance()->setMarkdownTooltipCallback(nullptr);
		return 0;
	}
	luaL_checktype(L, 1, LUA_TFUNCTION);
	lua_pushlightuserdata(L, (void*)imgui_setMarkdownTooltipCallback);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	auto f = [=](const std::string& p1, const std::string& p2, bool p3, const std::string& p4)
	{
		lua_pushlightuserdata(L, (void*)imgui_setMarkdownTooltipCallback);
		lua_rawget(L, LUA_REGISTRYINDEX);
		// ... f
		lua_pushlstring(L, p1.c_str(), p1.size());
		lua_pushlstring(L, p2.c_str(), p2.size());
		lua_pushboolean(L, p3);
		lua_pushlstring(L, p4.c_str(), p4.size());
		// ... f, p1, p2, p3, p4
		lua_call(L, 4, 0);
	};
	CCIMGUI::getInstance()->setMarkdownTooltipCallback(f);
	return 0;
}
static int imgui_setMarkdownImageCallback(lua_State* L)
{
	if (lua_isnil(L, 1))
	{
		CCIMGUI::getInstance()->setMarkdownLinkCallback(nullptr);
		return 0;
	}
	luaL_checktype(L, 1, LUA_TFUNCTION);
	lua_pushlightuserdata(L, (void*)imgui_setMarkdownImageCallback);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	auto f = [=](const std::string& p1, const std::string& p2)
	{
		lua_pushlightuserdata(L, (void*)imgui_setMarkdownImageCallback);
		lua_rawget(L, LUA_REGISTRYINDEX);
		// ... f
		lua_pushlstring(L, p1.c_str(), p1.size());
		lua_pushlstring(L, p2.c_str(), p2.size());
		// ... f, p1, p2
		lua_call(L, 2, 4);
		// ... f, r1, r2, r3, r4
		cocos2d::Sprite* sp = nullptr;
		luaval_to_object(L, -4, "cc.Sprite", &sp);
		ImVec2 size(0, 0);
		luaval_to_native(L, -3, &size);
		ImVec4 tint_col(1, 1, 1, 1);
		luaval_to_native(L, -2, &tint_col);
		ImVec4 border_col(0, 0, 0, 0);
		luaval_to_native(L, -1, &border_col);
		return std::make_tuple(sp, size, tint_col, border_col);
	};
	CCIMGUI::getInstance()->setMarkdownImageCallback(f);
	return 0;
}
static int imgui_setMarkdownFormatCallback(lua_State* L)
{
	if (lua_isnil(L, 1))
	{
		CCIMGUI::getInstance()->setMarkdownFormatCallback(nullptr);
		return 0;
	}
	luaL_checktype(L, 1, LUA_TFUNCTION);
	lua_pushlightuserdata(L, (void*)imgui_setMarkdownFormatCallback);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	auto f = [=](int p1, int32_t p2, bool p3, bool p4)
	{
		lua_pushlightuserdata(L, (void*)imgui_setMarkdownFormatCallback);
		lua_rawget(L, LUA_REGISTRYINDEX);
		// ... f
		lua_pushinteger(L, p1);
		lua_pushinteger(L, p2);
		lua_pushboolean(L, p3);
		lua_pushboolean(L, p4);
		// ... f, p1, p2, p3, p4
		lua_call(L, 4, 0);
	};
	CCIMGUI::getInstance()->setMarkdownFormatCallback(f);
	return 0;
}
static int imgui_setMarkdownFont(lua_State* L)
{
	const int index = luaL_checkinteger(L, 1);
	ImFont* font = nullptr;
	luaval_to_object(L, 2, "imgui.ImFont", &font);
	CCIMGUI::getInstance()->setMarkdownFont(index, font, lua_toboolean(L, 3));
	return 0;
}
static int imgui_setMarkdownLinkIcon(lua_State* L)
{
	size_t size;
	const auto s = luaL_checklstring(L, 1, &size);
	CCIMGUI::getInstance()->setMarkdownLinkIcon(std::string(s, size));
	return 0;
}
static int imgui_markdown(lua_State* L)
{
	size_t size;
	const auto s = luaL_checklstring(L, 1, &size);
	CCIMGUI::getInstance()->markdown(std::string(s, size));
	return 0;
}

struct LuaTextRange
{
	const char* begin;
	const char* end;
};

static LuaTextRange imgui_checkTextRange(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TSTRING);
	size_t length = 0;
	const char* text = lua_tolstring(L, index, &length);
	if (length > static_cast<size_t>((std::numeric_limits<int>::max)()))
		luaL_error(L, "text exceeds the native length limit");
	// The original Lua argument roots the bytes throughout the synchronous call.
	return { text, text + length };
}

static float imgui_checkTextSize(lua_State* L, int index)
{
	const float size = (float)luaL_checknumber(L, index);
	if (!std::isfinite(size) || size <= 0) luaL_error(L, "font size must be finite and positive");
	return size;
}

static int imgui_textUnformatted(lua_State* L)
{
	if (lua_gettop(L) != 1) return luaL_error(L, "textUnformatted expects one string");
	const auto text = imgui_checkTextRange(L, 1);
	ImGui::TextUnformatted(text.begin, text.end);
	return 0;
}

static int imgui_calcTextSize(lua_State* lua_S)
{
	if (lua_gettop(lua_S) < 1 || lua_gettop(lua_S) > 3)
		return luaL_error(lua_S, "calcTextSize expects text, optional hide_text_after_double_hash and wrap_width");
	LUA_CUR_FNAME("imgui.calcTextSize");
	const auto text = imgui_checkTextRange(lua_S, 1);
	bool hide = false;
	if (!lua_isnoneornil(lua_S, 2)) { LUA_NATIVE_SETTER(hide, 2); }
	const float wrap = (float)luaL_optnumber(lua_S, 3, -1.0f);
	LUA_PUSH_NATIVE(ImGui::CalcTextSize(text.begin, text.end, hide, wrap));
	return 1;
}

static int font_calcTextSizeA(lua_State* lua_S)
{
	if (lua_gettop(lua_S) != 5)
		return luaL_error(lua_S, "calcTextSizeA expects size, max_width, wrap_width and text");
	auto* font = imgui_checkFont(lua_S, 1);
	const float size = imgui_checkTextSize(lua_S, 2);
	const float max_width = (float)luaL_checknumber(lua_S, 3);
	const float wrap = (float)luaL_checknumber(lua_S, 4);
	const auto text = imgui_checkTextRange(lua_S, 5);
	const char* remaining = text.begin;
	const auto measured = font->CalcTextSizeA(size, max_width, wrap, text.begin, text.end, &remaining);
	LUA_PUSH_NATIVE(measured);
	lua_pushinteger(lua_S, static_cast<lua_Integer>(remaining - text.begin));
	return 2;
}

static int font_calcWordWrapPosition(lua_State* lua_S)
{
	if (lua_gettop(lua_S) != 4)
		return luaL_error(lua_S, "calcWordWrapPosition expects size, text and wrap_width");
	auto* font = imgui_checkFont(lua_S, 1);
	const float size = imgui_checkTextSize(lua_S, 2);
	const auto text = imgui_checkTextRange(lua_S, 3);
	const float wrap = (float)luaL_checknumber(lua_S, 4);
	const char* position = font->CalcWordWrapPosition(size, text.begin, text.end, wrap);
	lua_pushinteger(lua_S, static_cast<lua_Integer>(position - text.begin));
	return 1;
}

static int font_renderText(lua_State* lua_S)
{
	if (lua_gettop(lua_S) < 7 || lua_gettop(lua_S) > 9)
		return luaL_error(lua_S, "renderText expects draw_list, size, pos, col, clip_rect, text, optional wrap_width and flags");
	LUA_CUR_FNAME("imgui.ImFont:renderText");
	auto* font = imgui_checkFont(lua_S, 1);
	luaL_checkudata(lua_S, 2, "imgui.ImDrawList");
	auto* draw_list = LUA_TO_COBJ(ImDrawList*, 2);
	if (!draw_list) return luaL_error(lua_S, "invalid imgui.ImDrawList");
	const float size = imgui_checkTextSize(lua_S, 3);
	ImVec2 pos;
	ImVec4 clip;
	LUA_NATIVE_SETTER(pos, 4);
	const ImU32 col = lua_tou32(lua_S, 5);
	LUA_NATIVE_SETTER(clip, 6);
	const auto text = imgui_checkTextRange(lua_S, 7);
	const float wrap = (float)luaL_optnumber(lua_S, 8, 0);
	const int flags = luaL_optint(lua_S, 9, 0);
	font->RenderText(draw_list, size, pos, col, clip, text.begin, text.end, wrap, flags);
	return 0;
}

#define M(n) {#n, imgui_##n}
static const luaL_Reg imgui_methods[] = {
	M(calcTextSize), M(textUnformatted),
	// Main
	M(showStyleEditor),

	// Demo, Debug, Information
	M(showDemoWindow), M(showAboutWindow), M(showMetricsWindow),
	M(showDebugLogWindow), M(showIDStackToolWindow),

	// Styles
	M(styleColorsDark), M(styleColorsClassic), M(styleColorsLight),

	// Window
	{"endToLua", imgui_end}, M(begin), M(beginChild), M(endChild),

	// Parameters stacks (shared)
	M(pushFont), M(getFont), M(pushStyleColor), M(pushStyleVar),

	// Widgets
    M(image), M(imageWithBg), M(imageButton), M(collapsingHeader), M(checkbox),
	M(checkboxFlags), M(radioButton), M(combo),

    // Widgets: Drags
    M(dragFloatRange2),M(dragIntRange2),

    // Widgets: Sliders
    M(sliderAngle),

    // Widgets: Input with Keyboard
    M(inputText),M(inputTextMultiline),M(inputTextWithHint),

	// Widgets: Color Editor/Picker
	M(colorEdit3),M(colorEdit4),M(colorPicker3),M(colorPicker4),

	// Widgets: Selectables
	M(selectable),

	// Widgets: List Boxes
	M(listBox),

	// Widgets: Data Plotting
	M(plotLines),M(plotHistogram),

    // Menus
    M(menuItem),

	// Popups, Modals
	M(beginPopupModal),

	// Tab Bars, Tabs
	M(beginTabItem),

	// Docking
	M(dockSpace), M(dockSpaceOverViewport), M(setNextWindowClass),

	// Tables
	M(tableGetSortSpecs),

	// Multi-selection
	M(setNextItemSelectionUserData),

	// Drag and Drop
	M(setDragDropPayload),M(acceptDragDropPayload),M(getDragDropPayload),

	// Miscellaneous Utilities
	M(calcListClipping),

	// Color Utilities
	M(colorConvertRGBtoHSV),
	M(colorConvertHSVtoRGB),

	// Inputs Utilities
	M(isMousePosValid),

	// ID
	M(pushID),

	// IO
    M(getIO),

    M(addFontTTF),

	// Style
    M(getStyle), M(setStyleColor),

	// Settings/.Ini Utilities
	M(saveIniSettingsToMemory),

	// launch/shutdown
	M(createLayer),
	M(shutdown),

	// cocos
	M(ccNode), M(ccNodeButton),
	M(setCCNodeColor), M(setCCLabelColor),

	// markdown
	M(setMarkdownLinkCallback),
	M(setMarkdownTooltipCallback),
	M(setMarkdownImageCallback),
	M(setMarkdownFormatCallback),
	M(setMarkdownFont),
	M(setMarkdownLinkIcon),
	M(markdown),

    {NULL,  NULL}
};

// drawlist

static int drawlist_addCCNode(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	cocos2d::Node* node = nullptr;
	if (luaval_to_object(L, 2, "cc.Node", &node))
	{
		if (!node) return 0;
		auto [tid, pmin, pmax, id] = CCIMGUI::getInstance()->useNode(node, _luaval_to_imvec2(L, 3));
		if (!tid) return 0;
		cobj->AddImage(
			tid,
			pmin,
			pmax);
		return 0;
	}
	return 0;
}
static int drawlist_addText(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	const int args = lua_gettop(L);
	if (lua_istable(L, 2))
	{
		std::string s;
		luaval_to_std_string(L, 4, &s);
		cobj->AddText(_luaval_to_imvec2(L, 2), luaL_checknumber(L, 3), s.c_str(), s.c_str() + s.size());
	}
	else
	{
		ImFont* font = nullptr;
		luaval_to_object(L, 2, "imgui.ImFont", &font);
		std::string s;
		luaval_to_std_string(L, 6, &s);
		ImVec4 v;
		ImVec4* p = nullptr;
		if (args >= 8)
		{
			luaval_to_native(L, 8, &v);
			p = &v;
		}
		cobj->AddText(
			font,
			(float)luaL_checknumber(L, 3),
			_luaval_to_imvec2(L, 4),
			luaL_checknumber(L, 5),
			s.c_str(), s.c_str() + s.size(),
			lua_opt_number(args, 7, 0),
			p);
	}
	return 0;
}
static int drawlist_addImage(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	const int args = lua_gettop(L);
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, 2, "cc.Texture2D", &tex))
	{
		if (!tex) return 0;
		auto [tid, id] = CCIMGUI::getInstance()->useTexture(tex);
		if (!tid) return 0;
		cobj->AddImage(
			tid,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			lua_opt_imv2(args, 5, ImVec2(0, 0)),
			lua_opt_imv2(args, 6, ImVec2(1, 1)),
			lua_opt_u32(args, 7, IM_COL32_WHITE));
		return 0;
	}
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, 2, "cc.Sprite", &sp))
	{
		if (!sp) return 0;
		auto [tid, uv_min, uv_max, id] = CCIMGUI::getInstance()->useSprite(sp);
		if (!tid) return 0;
		cobj->AddImage(
			tid,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			uv_min,
			uv_max,
			lua_opt_u32(args, 5, IM_COL32_WHITE));
		return 0;
	}
	return 0;
}
static int drawlist_addImageQuad(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	const int args = lua_gettop(L);
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, 2, "cc.Texture2D", &tex))
	{
		if (!tex) return 0;
		auto [tid, id] = CCIMGUI::getInstance()->useTexture(tex);
		if (!tid) return 0;
		cobj->AddImageQuad(
			tid,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			_luaval_to_imvec2(L, 5),
			_luaval_to_imvec2(L, 6),
			lua_opt_imv2(args, 7, ImVec2(0, 0)),
			lua_opt_imv2(args, 8, ImVec2(1, 0)),
			lua_opt_imv2(args, 9, ImVec2(1, 1)),
			lua_opt_imv2(args, 10, ImVec2(0, 1)),
			lua_opt_u32(args, 11, IM_COL32_WHITE));
		return 0;
	}
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, 2, "cc.Sprite", &sp))
	{
		if (!sp) return 0;
		auto [tid, uva, uvc, id] = CCIMGUI::getInstance()->useSprite(sp);
		if (!tid) return 0;
		cobj->AddImageQuad(
			tid,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			_luaval_to_imvec2(L, 5),
			_luaval_to_imvec2(L, 6),
			uva,
			ImVec2(uvc.x, uva.y),
			uvc,
			ImVec2(uva.x, uvc.y),
			lua_opt_u32(args, 7, IM_COL32_WHITE));
		return 0;
	}
	return 0;
}
static int drawlist_addImageRounded(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	const int args = lua_gettop(L);
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, 2, "cc.Texture2D", &tex))
	{
		if (!tex) return 0;
		auto [tid, id] = CCIMGUI::getInstance()->useTexture(tex);
		if (!tid) return 0;
		cobj->AddImageRounded(
			tid,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			_luaval_to_imvec2(L, 5),
			_luaval_to_imvec2(L, 6),
			lua_tou32(L, 7),
			(float)luaL_checknumber(L, 8),
			lua_opt_int(args, 9, 0));
		return 0;
	}
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, 2, "cc.Sprite", &sp))
	{
		if (!sp) return 0;
		auto [tid, uv_min, uv_max, id] = CCIMGUI::getInstance()->useSprite(sp);
		if (!tid) return 0;
		cobj->AddImageRounded(
			tid,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			uv_min,
			uv_max,
			lua_tou32(L, 5),
			(float)luaL_checknumber(L, 6),
			lua_opt_int(args, 7, 0));
		return 0;
	}
	return 0;
}
static int drawlist_addPolyline(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	std::vector<cocos2d::Vec2> arr;
	if (!luaval_to_native(L, 2, &arr))
		return 0;
	cobj->AddPolyline(
		(ImVec2*)arr.data(),
		arr.size(),
		lua_tou32(L, 3),
		(float)luaL_checknumber(L, 4),
		(ImDrawFlags)luaL_checkinteger(L, 5));
	return 0;
}
static int drawlist_addConvexPolyFilled(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	std::vector<cocos2d::Vec2> arr;
	if (!luaval_to_native(L, 2, &arr))
		return 0;
	cobj->AddConvexPolyFilled(
		(ImVec2*)arr.data(),
		arr.size(),
		lua_tou32(L, 3));
	return 0;
}
static int drawlist_addConcavePolyFilled(lua_State *L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	std::vector<cocos2d::Vec2> arr;
	if (!luaval_to_native(L, 2, &arr))
		return 0;
	cobj->AddConcavePolyFilled(
		(ImVec2*)arr.data(),
		arr.size(),
		lua_tou32(L, 3));
	return 0;
}
static int drawlist_pushTexture(lua_State* L)
{
	auto cobj = (ImDrawList*)tolua_tousertype(L, 1, nullptr);
	if (!cobj) return 0;
	cobj->PushTexture(_luaval_to_imTextureRef(L, 2));
	return 0;
}

#undef M
#define M(n) {#n, drawlist_##n}
static const luaL_Reg drawlist_methods[] = {
	M(addCCNode),
	M(addText),
	M(addImage),
	M(addImageQuad),
	M(addImageRounded),
	M(addPolyline),
	M(addConvexPolyFilled),
	M(addConcavePolyFilled),
	M(pushTexture),
};

// font

static ImFontConfig* imgui_checkFontConfig(lua_State* lua_S)
{
	luaL_checkudata(lua_S, 1, "imgui.ImFontConfig");
	auto* config = LUA_TO_COBJ(ImFontConfig*, 1);
	if (!config)
		luaL_error(lua_S, "invalid imgui.ImFontConfig");
	return config;
}

static int lua_ImFontConfig_Name_getter(lua_State* lua_S)
{
	const auto* config = imgui_checkFontConfig(lua_S);
	size_t length = 0;
	while (length < sizeof(config->Name) && config->Name[length] != '\0')
		++length;
	lua_pushlstring(lua_S, config->Name, length);
	return 1;
}

static int lua_ImFontConfig_Name_setter(lua_State* lua_S)
{
	return imgui_setFontConfigName(lua_S, 2, *imgui_checkFontConfig(lua_S));
}

static int luaReg_imguiFontConfigManual(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_ENTRY("ImFontConfig");
	LUA_FIELD("Name", lua_ImFontConfig_Name_getter, lua_ImFontConfig_Name_setter);
	LUA_ENTRY_END(2);
	return 0;
}

static int fontAtlas_getGlyphRangesDefault(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:getGlyphRangesDefault");
	LUA_TRY_INVOKE_R(0, ([](ImFontAtlas* atlas) {
		std::vector<ImWchar> snapshot;
		// Copy inclusive start/end pairs before converting to a fresh Lua table.
		for (const ImWchar* ranges = atlas->GetGlyphRangesDefault(); ranges[0]; ranges += 2)
		{
			snapshot.push_back(ranges[0]);
			snapshot.push_back(ranges[1]);
		}
		return snapshot;
	}));
	LUA_INVOKE_FOOTER("0");
}

template<bool Add>
static int fontAtlas_customRect(lua_State* lua_S)
{
	if (lua_gettop(lua_S) != (Add ? 3 : 2))
		return luaL_error(lua_S, "invalid custom rectangle argument count");
	LUA_CUR_FNAME("imgui.ImFontAtlas:customRect");
	luaL_checkudata(lua_S, 1, "imgui.ImFontAtlas");
	auto* atlas = LUA_TO_COBJ(ImFontAtlas*, 1);
	if (!atlas) return luaL_error(lua_S, "invalid imgui.ImFontAtlas");
	ImFontAtlasRect rect;
	bool found;
	if constexpr (Add) {
		const int width = luaL_checkint(lua_S, 2);
		const int height = luaL_checkint(lua_S, 3);
		if (width <= 0 || width > 0xffff || height <= 0 || height > 0xffff)
			return luaL_error(lua_S, "rectangle dimensions must be in 1..65535");
		if (atlas->Locked) return luaL_error(lua_S, "font atlas is locked");
		const auto id = atlas->AddCustomRect(width, height, &rect);
		found = id != ImFontAtlasRectId_Invalid;
		LUA_PUSH_NATIVE(id);
	} else {
		found = atlas->GetCustomRect(luaL_checkint(lua_S, 2), &rect);
		LUA_PUSH_NATIVE(found);
	}
	if (found) { LUA_PUSH_NATIVE(rect); }
	else lua_pushnil(lua_S);
	return 2;
}

static int luaReg_imguiFontAtlasManual(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_ENTRY("ImFontAtlas");
	LUA_METHOD("getGlyphRangesDefault", fontAtlas_getGlyphRangesDefault);
	LUA_METHOD("addCustomRect", fontAtlas_customRect<true>);
	LUA_METHOD("getCustomRect", fontAtlas_customRect<false>);
	LUA_ENTRY_END(2);
	return 0;
}

#undef M
#define M(n) {#n, font_##n}
static const luaL_Reg font_methods[] = {
	M(calcTextSizeA), M(calcWordWrapPosition), M(renderText)
};

static int fontBaked_findGlyph(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontBaked", "imgui.ImFontBaked:findGlyph");
	LUA_TRY_INVOKE_R(1, ([](ImFontBaked* font, ImWchar codepoint) {
		return ImFontGlyphSnapshot(font->FindGlyph(codepoint));
	}));
	LUA_INVOKE_FOOTER("1");
}

static int fontBaked_findGlyphNoFallback(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontBaked", "imgui.ImFontBaked:findGlyphNoFallback");
	LUA_TRY_INVOKE_R(1, ([](ImFontBaked* font, ImWchar codepoint) {
		return ImFontGlyphSnapshot(font->FindGlyphNoFallback(codepoint));
	}));
	LUA_INVOKE_FOOTER("1");
}

static const luaL_Reg font_baked_methods[] = {
	{"findGlyph", fontBaked_findGlyph},
	{"findGlyphNoFallback", fontBaked_findGlyphNoFallback}
};

int luaopen_imgui(lua_State *L)
{
    luaL_register(L, "imgui", imgui_methods);
    lua_pushstring(L, ImGui::GetVersion());
    lua_setfield(L, -2, "version");
	lua_pop(L, 1);

	luaReg_imgui(L);
	luaReg_imguiScalarManual(L);
	luaReg_imguiStyle(L);
	luaReg_imguiIO(L);
	luaReg_imguiFont(L);
	luaReg_imguiFontConfigManual(L);
	luaReg_imguiFontAtlasManual(L);
	luaReg_imguiViewport(L);
	luaReg_imguiDrawList(L);
	luaReg_imguiTableSortSpecs(L);
	luaReg_imguiMultiSelectIO(L);
	luaReg_imguiSelectionBasicStorage(L);

	lua_pushstring(L, "imgui.ImDrawList");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		for (auto&& reg : drawlist_methods)
			tolua_function(L, reg.name, reg.func);
	}
	lua_pop(L, 1);

	lua_pushstring(L, "imgui.ImFont");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		for (auto&& reg : font_methods)
			tolua_function(L, reg.name, reg.func);
	}
	lua_pop(L, 1);

	lua_pushliteral(L, "imgui.ImFontBaked");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		for (auto&& reg : font_baked_methods)
			tolua_function(L, reg.name, reg.func);
	}
	lua_pop(L, 1);

	luaReg_imguiColorTextEdit(L);
	luaReg_implotManual(L);

	CCIMGUI::setOnInit([](CCIMGUI* ins)
	{
		ins->addCallback([]()
		{
			auto* stack = LuaEngine::getInstance()->getLuaStack();
			// Keep lookup and drawing temporaries inside the protected callback frame.
			stack->executeCallback([stack]() -> int
			{
				auto* state = stack->getLuaState();
				lua_getglobal(state, "imgui");
				if (!lua_istable(state, -1))
					return 0;
				lua_getfield(state, -1, "draw");
				if (!lua_isfunction(state, -1))
					return 0;
				return stack->executeFunction(0);
			});
		}, "__luadraw__");
	});
    return 0;
}

