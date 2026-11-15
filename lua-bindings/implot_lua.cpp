#include "implot_lua.hpp"
#include "imgui_lua_internal.hpp"
#include "lua_implot_auto.hpp"
#include "../implot/implot_internal.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "../CCIMGUI.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

#pragma warning (disable: 4244)

using lua::luaval_to_native;
using lua::native_to_luaval;
using lua::LUA_TCDATA;

#define lua_opt_impoint(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, _luaval_to_implotPoint, _DEF)
#define lua_opt_imrange(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, _luaval_to_implotRange, _DEF)
#define lua_opt_imrect(_N, _IDX, _DEF) lua_opt_value(_N, _IDX, _luaval_to_implotRect, _DEF)

ImPlotPoint _luaval_to_implotPoint(lua_State* L, int lo)
{
	ImPlotPoint v;
	luaval_to_native(L, lo, &v);
	return v;
}
ImPlotRange _luaval_to_implotRange(lua_State* L, int lo)
{
	ImPlotRange v;
	luaval_to_native(L, lo, &v);
	return v;
}
ImPlotRect _luaval_to_implotRect(lua_State* L, int lo)
{
	ImPlotRect v;
	luaval_to_native(L, lo, &v);
	return v;
}
ImPlotSpec lua_toImPlotSpec(lua_State* L, int lo)
{
#define get_field(_K, _F)\
	lua_getfield(L, lo, #_K);\
	if(!lua_isnil(L, -1)) {\
		(&ret)->_K = _F(L, -1); }\
	lua_pop(L, 1);

	ImPlotSpec ret;
	if (lua_istable(L, lo))
	{
		get_field(LineColor, _luaval_to_imvec4);
		get_field(LineWeight, luaL_checknumber);
		get_field(FillColor, _luaval_to_imvec4);
		get_field(FillAlpha, luaL_checknumber);
		get_field(Marker, luaL_checkinteger);
		get_field(MarkerSize, luaL_checknumber);
		get_field(MarkerLineColor, _luaval_to_imvec4);
		get_field(MarkerFillColor, _luaval_to_imvec4);
		get_field(Size, luaL_checknumber);
		get_field(Offset, luaL_checkinteger);
		get_field(Stride, luaL_checkinteger);
		get_field(Flags, luaL_checkinteger);
	}
#undef get_field
	return ret;
}

enum PlotSpecArrays {
	PlotSpecArrays_None = 0,
	PlotSpecArrays_Line = 1,
	PlotSpecArrays_Fill = 2,
	PlotSpecArrays_Marker = 4,
};

// Leave each copied buffer on the Lua stack until the PlotX call returns. Lua owns
// the allocation even if a later conversion raises an error (no vector unwinding).
// Read the raw 1-based prefix, ignoring extra keys and stopping at the first nil.
template<typename T>
static T* lua_plot_spec_array(lua_State* L, int spec_index, const char* field, int required_count)
{
	lua_getfield(L, spec_index, field);
	if (lua_isnil(L, -1)) { lua_pop(L, 1); return nullptr; }
	luaL_checktype(L, -1, LUA_TTABLE);
	const int array_index = lua_gettop(L);
	size_t count = lua_objlen(L, array_index);
	if (count > static_cast<size_t>((std::numeric_limits<int>::max)()) || count > SIZE_MAX / sizeof(T))
		luaL_error(L, "ImPlotSpec.%s array is too large", field);
	auto* data = static_cast<T*>(lua_newuserdata(L, count * sizeof(T)));
	for (size_t i = 0; i < count; ++i) {
		lua_rawgeti(L, array_index, static_cast<int>(i + 1));
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			count = i;
			break;
		}
		const double value = luaL_checknumber(L, -1);
#if CC_DEBUG >= 1
		if (lua_type(L, -1) != LUA_TNUMBER || !std::isfinite(value) || value < 0 ||
			value > static_cast<double>((std::numeric_limits<T>::max)()) ||
			(std::is_integral_v<T> && std::trunc(value) != value))
			luaL_error(L, "ImPlotSpec.%s has an out-of-range element #%d", field, static_cast<int>(i + 1));
#endif
		data[i] = static_cast<T>(value);
		lua_pop(L, 1);
	}
#if CC_DEBUG >= 1
	if (count < static_cast<size_t>(required_count))
		luaL_error(L, "ImPlotSpec.%s requires at least %d elements", field, required_count);
#else
	(void)required_count;
#endif
	lua_remove(L, array_index);
	return data;
}

static void lua_plot_spec_arrays(lua_State* L, int lo, ImPlotSpec& spec, int count, unsigned arrays)
{
	if (!lua_istable(L, lo)) return;
	if (lo < 0) lo = lua_gettop(L) + lo + 1;
	spec.LineColors = lua_plot_spec_array<ImU32>(L, lo, "LineColors", arrays & PlotSpecArrays_Line ? count : 0);
	spec.FillColors = lua_plot_spec_array<ImU32>(L, lo, "FillColors", arrays & PlotSpecArrays_Fill ? count : 0);
	spec.MarkerSizes = lua_plot_spec_array<float>(L, lo, "MarkerSizes", arrays & PlotSpecArrays_Marker ? count : 0);
	spec.MarkerLineColors = lua_plot_spec_array<ImU32>(L, lo, "MarkerLineColors", arrays & PlotSpecArrays_Marker ? count : 0);
	spec.MarkerFillColors = lua_plot_spec_array<ImU32>(L, lo, "MarkerFillColors", arrays & PlotSpecArrays_Marker ? count : 0);
}

// implot

// Link arguments must own stable writable storage, unlike temporary numeric copies.
// Registry roots are grouped by native context and deduplicated by buffer address.
static char axis_link_roots_key;

static double* axis_link_pointer(lua_State* L, int index) {
	if (lua_isnoneornil(L, index)) return nullptr;
	static constexpr auto code = R"__(
local ffi = require('ffi')
local cell = ffi.typeof('double[1]')
local pointer = ffi.typeof('void*[1]')
local pointer_size = ffi.sizeof('void*')
return function(value, out)
    if not ffi.istype(cell, value) then return false end
    value[0] = value[0] -- Reject read-only cdata too.
    ffi.copy(out, pointer(value), pointer_size)
    return true
end
)__";
	if (!lua::_try_get_internal_helper(L, "implot.axis_link.pointer", code))
		luaL_error(L, "cannot initialize axis link conversion");
	double* pointer = nullptr;
	lua_pushvalue(L, index);
	lua_pushlightuserdata(L, &pointer);
	const bool ok = lua_pcall(L, 2, 1, 0) == 0 && lua_toboolean(L, -1);
	lua_pop(L, 1);
	if (!ok || !pointer || !std::isfinite(*pointer))
		luaL_error(L, "axis link argument #%d must be nil or a writable double[1] cdata with a finite value", index);
	return pointer;
}

static void pin_axis_link(lua_State* L, ImPlotContext* context, int index, double* pointer) {
	if (!pointer) return;
	const int top = lua_gettop(L);
	lua_pushlightuserdata(L, &axis_link_roots_key);
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushlightuserdata(L, &axis_link_roots_key);
		lua_pushvalue(L, -2);
		lua_rawset(L, LUA_REGISTRYINDEX);
	}
	lua_pushlightuserdata(L, context);
	lua_rawget(L, -2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushlightuserdata(L, context);
		lua_pushvalue(L, -2);
		lua_rawset(L, -4);
	}
	lua_pushlightuserdata(L, pointer);
	lua_pushvalue(L, index);
	lua_rawset(L, -3);
	lua_settop(L, top);
}

static void release_axis_links(lua_State* L, ImPlotContext* context) {
	const int top = lua_gettop(L);
	lua_pushlightuserdata(L, &axis_link_roots_key);
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (!lua_istable(L, -1)) { lua_settop(L, top); return; }
	lua_pushlightuserdata(L, context);
	lua_rawget(L, -2);
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			auto* pointer = static_cast<double*>(lua_touserdata(L, -2));
			// Disabled axes retain LinkedMin/Max across native Reset() and EndPlot
			// writes every axis. Detach only our pointers before dropping their owners.
			for (int p = 0; p < context->Plots.GetMapSize(); ++p) {
				if (auto* plot = context->Plots.TryGetMapData(p)) {
					for (auto& axis : plot->Axes) {
						if (axis.LinkedMin == pointer) axis.LinkedMin = nullptr;
						if (axis.LinkedMax == pointer) axis.LinkedMax = nullptr;
					}
				}
			}
			for (int axis = 0; axis < ImAxis_COUNT; ++axis) {
				if (context->NextPlotData.LinkedMin[axis] == pointer) context->NextPlotData.LinkedMin[axis] = nullptr;
				if (context->NextPlotData.LinkedMax[axis] == pointer) context->NextPlotData.LinkedMax[axis] = nullptr;
			}
			lua_pop(L, 1);
		}
	}
	lua_pop(L, 1);
	lua_pushlightuserdata(L, context);
	lua_pushnil(L);
	lua_rawset(L, -3);
	lua_settop(L, top);
}

// Call the generated BeginPlot/EndPlot binding unchanged, including its overloads
// and return values. EndPlot must write the linked values before roots are released.
static int implot_plotBoundary(lua_State* L) {
	auto* context = ImPlot::GetCurrentContext();
	const int args = lua_gettop(L);
	lua_pushvalue(L, lua_upvalueindex(1));
	lua_insert(L, 1);
	lua_call(L, args, LUA_MULTRET);
	const int results = lua_gettop(L);
	const bool begin = lua_toboolean(L, lua_upvalueindex(2)) != 0;
	if (context && (!begin || (results > 0 && !lua_toboolean(L, 1))))
		release_axis_links(L, context);
	return results;
}

template<bool Next>
static int implot_axisLinks(lua_State* L) {
	if (lua_gettop(L) < 1 || lua_gettop(L) > 3)
		return luaL_error(L, "axis links expect axis, optional link_min and optional link_max");
	const double axis_value = luaL_checknumber(L, 1);
	if (!std::isfinite(axis_value) || std::trunc(axis_value) != axis_value || axis_value < 0 || axis_value >= ImAxis_COUNT)
		return luaL_error(L, "invalid ImAxis");
	const auto axis = static_cast<ImAxis>(axis_value);
	auto* context = ImPlot::GetCurrentContext();
	if (!context) return luaL_error(L, "axis links require an ImPlot context");
	if constexpr (Next) {
		if (context->CurrentPlot) return luaL_error(L, "setNextAxisLinks must precede beginPlot");
	} else {
		if (!context->CurrentPlot || context->CurrentPlot->SetupLocked || !context->CurrentPlot->Axes[axis].Enabled)
			return luaL_error(L, "setupAxisLinks requires an enabled axis during plot setup");
	}
	double* min = axis_link_pointer(L, 2);
	double* max = axis_link_pointer(L, 3);
	pin_axis_link(L, context, 2, min);
	pin_axis_link(L, context, 3, max);
	if constexpr (Next) ImPlot::SetNextAxisLinks(axis, min, max);
	else ImPlot::SetupAxisLinks(axis, min, max);
	return 0;
}

static int implot_beginSubplots(lua_State* L) {
	const int args = lua_gettop(L);
	lua_pushboolean(L,
		ImPlot::BeginSubplots(
			luaL_checkstring(L, 1),
			luaL_checkint(L, 2),
			luaL_checkint(L, 3),
			_luaval_to_imvec2(L, 4),
			lua_opt_int(args, 5, 0)
		));
	return 1;
}
static int implot_setupAxisTicks(lua_State* L) {
	const int args = lua_gettop(L);
	if (lua_isnumber(L, 2))
	{
		// void SetupAxisTicks(ImAxis axis, double v_min, double v_max, int n_ticks, const char* const labels[]=NULL, bool keep_default=false);
		const char* const* labels = nullptr;
		std::vector<const char*> arr;
		if (args >= 5 && luaval_to_native(L, 5, &arr))
			labels = arr.data();
		ImPlot::SetupAxisTicks(
			luaL_checkint(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3),
			luaL_checkint(L, 4),
			labels,
			lua_opt_bool(args, 6, false)
		);
	}
	else
	{
		// void SetupAxisTicks(ImAxis axis, const double* values, int n_ticks, const char* const labels[]=NULL, bool keep_default=false);
		const char* const* labels = nullptr;
		std::vector<const char*> arr;
		if (args >= 4 && luaval_to_native(L, 4, &arr))
			labels = arr.data();
		std::vector<double> valuesArr;
		lua_checknative(L, 2, &valuesArr);
		const double* values = nullptr;
		ImPlot::SetupAxisTicks(
			luaL_checkint(L, 1),
			valuesArr.data(),
			luaL_checkint(L, 3),
			labels,
			lua_opt_bool(args, 6, false)
		);
	}
	return 1;
}

#define LUA_GET_PTR(_name, _i)\
	T* _name = (T*)lua_to_cptr(L, (_i));\
	if (!(_name)) return lua_param_error(_i);
#define DEF_PLOT_T(_name)\
	template<typename T, int Offset = 1>\
	struct _name##T { static int F(lua_State* L); };\
	template<typename T, int Offset>\
	int _name##T<T, Offset>::F(lua_State* L)
#define DEF_PLOT_C(_name)\
	static int _name##C(lua_State* L) { return implotCFunc<_name##T>::F(L); }

static bool lua_type_indexable(int t)
{
	return t == LUA_TTABLE || t == LUA_TUSERDATA;
}

static void lua_push_convert_ptr(lua_State* L)
{
	static const char* convert_ptr_key = "ffi.convert_ptr";
	lua_pushstring(L, convert_ptr_key);
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_type(L, -1) != LUA_TFUNCTION)
	{
		lua_pop(L, 1);
		static const char* convert_ptr = R"__(
local ffi = require('ffi')
local _ptr_size = ffi.sizeof('void*')
local _pptr_t = ffi.typeof('void*[1]')
return function(cdata_ptr, userdata)
    ffi.copy(userdata, _pptr_t(cdata_ptr), _ptr_size)
end
)__";
		if (luaL_dostring(L, convert_ptr) == 0)
		{
			lua_pushvalue(L, -1);
			lua_setfield(L, LUA_REGISTRYINDEX, convert_ptr_key);
		}
		else
		{
			luaL_error(L, "failed to load %s", convert_ptr_key);
		}
	}
}

static void* lua_to_cptr(lua_State* L, int lo)
{
	const auto top = lua_gettop(L);
	if (lo < 0)
		lo = top + 1 + lo;
	if (lo < 1 || lo > top || lua_type(L, lo) != LUA_TCDATA)
		return nullptr;
	lua_push_convert_ptr(L);
	void* ret = nullptr;
	lua_pushvalue(L, lo);
	lua_pushlightuserdata(L, &ret);
	if (lua_pcall(L, 2, 0, 0) == 0)
		return ret;
	lua_settop(L, top);
	return nullptr;
}

static int lua_plot_count(lua_State* L, int lo)
{
	const double value = luaL_checknumber(L, lo);
	if (!std::isfinite(value) || std::trunc(value) != value || value < 0 || value > (std::numeric_limits<int>::max)())
		luaL_error(L, "plot count/dimension must be a non-negative int");
	return static_cast<int>(value);
}

static int lua_check_count(lua_State* L, int lo, size_t size)
{
	return static_cast<int>(std::min(static_cast<size_t>(lua_plot_count(L, lo)), size));
}

// Packed Lua arrays cannot honor arbitrary byte strides. C callers own their buffers.
static ImPlotSpec lua_plot_spec_values(lua_State* L, int lo, int element_size, bool packed, bool legacy_layout = false)
{
	ImPlotSpec spec;
	if (lua_isnumber(L, lo))
		spec.Flags = luaL_checkint(L, lo);
	else if (lua_istable(L, lo) || lua_isnoneornil(L, lo))
		spec = lua_toImPlotSpec(L, lo);
	else
		luaL_error(L, "plot specification must be a table or flags integer");
	if (legacy_layout && !lua_istable(L, lo)) {
		spec.Offset = luaL_optint(L, lo + 1, 0);
		spec.Stride = luaL_optint(L, lo + 2, IMPLOT_AUTO);
	}
	if (spec.Stride != IMPLOT_AUTO &&
		(spec.Stride < element_size || spec.Stride % element_size != 0 || (packed && spec.Stride != element_size)))
		luaL_error(L, "invalid plot offset/stride for the data layout");
	return spec;
}

static ImPlotSpec lua_plot_spec(lua_State* L, int lo, int element_size, bool packed,
	bool legacy_layout = false, int array_count = 0, unsigned arrays = PlotSpecArrays_None)
{
	ImPlotSpec spec = lua_plot_spec_values(L, lo, element_size, packed, legacy_layout);
	lua_plot_spec_arrays(L, lo, spec, array_count, arrays);
	return spec;
}

template<typename T>
static ImPlotSpec lua_histogram_spec(lua_State* L, int lo, const T* values, int count,
	int bins, ImPlotRange range, bool packed)
{
	ImPlotSpec spec = lua_plot_spec_values(L, lo, sizeof(T), packed);
	int array_count = 0;
#if CC_DEBUG >= 1
	bool has_colors = false;
	if (lua_istable(L, lo)) {
		lua_getfield(L, lo, "LineColors");
		lua_getfield(L, lo, "FillColors");
		has_colors = !lua_isnil(L, -1) || !lua_isnil(L, -2);
		lua_pop(L, 2);
	}
	if (has_colors && count > 0 && bins != 0) {
		array_count = bins;
		if (bins < 0) {
			// Match the native histogram's offset/stride and automatic range/binning.
			if (static_cast<size_t>(count) > SIZE_MAX / sizeof(double))
				luaL_error(L, "histogram sample array is too large");
			auto* samples = static_cast<double*>(lua_newuserdata(L, static_cast<size_t>(count) * sizeof(double)));
			const int offset = ImPosMod(spec.Offset, count);
			const int stride = spec.Stride == IMPLOT_AUTO ? sizeof(T) : spec.Stride;
			for (int i = 0; i < count; ++i) {
				const size_t index = (static_cast<size_t>(i) + offset) % count;
				samples[i] = static_cast<double>(*reinterpret_cast<const T*>(reinterpret_cast<const char*>(values) + index * stride));
			}
			if (range.Min == 0 && range.Max == 0)
				ImMinMaxArray(samples, count, &range.Min, &range.Max);
			if (bins < ImPlotBin_Scott || bins > ImPlotBin_Sqrt)
				luaL_error(L, "invalid automatic histogram binning method");
			if (bins == ImPlotBin_Scott) {
				const double width = 3.49 * ImStdDev(samples, count) / cbrt(count);
				const double estimate = std::round(range.Size() / width);
				if (!std::isfinite(estimate) || estimate < 1 || estimate > (std::numeric_limits<int>::max)())
					luaL_error(L, "invalid automatic histogram bin count");
			}
			double width;
			ImPlot::CalculateBins(samples, count, bins, range, array_count, width);
		}
	}
#endif
	lua_plot_spec_arrays(L, lo, spec, array_count, PlotSpecArrays_Line | PlotSpecArrays_Fill);
	return spec;
}

static int lua_plot_area(lua_State* L, int rows, int cols)
{
	if (rows != 0 && cols > (std::numeric_limits<int>::max)() / rows)
		luaL_error(L, "plot dimensions overflow int");
	return rows * cols;
}

template<
	template <typename T, int _Offset = 1> class Fn,
	int TypeIdx = 2, int Offset = 1>
struct implotCFunc
{
	static int F(lua_State* L) {
		// note: ImS64/ImU64 may different from int64_t/uint64_t
		switch ((ImGuiDataType_)lua_plot_count(L, TypeIdx))
		{
		case ImGuiDataType_S8: return Fn<ImS8, Offset>::F(L);
		case ImGuiDataType_U8: return Fn<ImU8, Offset>::F(L);
		case ImGuiDataType_S16: return Fn<ImS16, Offset>::F(L);
		case ImGuiDataType_U16: return Fn<ImU16, Offset>::F(L);
		case ImGuiDataType_S32: return Fn<ImS32, Offset>::F(L);
		case ImGuiDataType_U32: return Fn<ImU32, Offset>::F(L);
		case ImGuiDataType_S64: return Fn<ImS64, Offset>::F(L);
		case ImGuiDataType_U64: return Fn<ImU64, Offset>::F(L);
		case ImGuiDataType_Float: return Fn<float, Offset>::F(L);
		case ImGuiDataType_Double: return Fn<double, Offset>::F(L);
		case ImGuiDataType_COUNT: break;
		default:;
		}
		return lua_param_error(TypeIdx);
	}
};

static int implot_plotLine(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	const auto type = lua_type(L, 3);
	if (type == LUA_TNUMBER) {
		// values
		const auto count = lua_check_count(L, 3, v1.size());
		if (count > 0)
			ImPlot::PlotLine<double>(label_id, v1.data(), count,
				lua_opt_number(args, 4, 1), // double xscale=1
				lua_opt_number(args, 5, 0), // double xstart=0,
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	if (type == LUA_TTABLE) {
		// xs, ys
		std::vector<double> v2;
		if (!luaval_to_native(L, 3, &v2))
			return lua_param_error(3);
		const auto count = lua_check_count(L, 4, std::min(v1.size(), v2.size()));
		if (count > 0)
			ImPlot::PlotLine<double>(label_id, v1.data(), v2.data(), count,
				lua_plot_spec(L, 5, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	return lua_param_error(3);
}
DEF_PLOT_T(implot_plotLine)
{
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	const auto type3 = lua_type(L, 3 + Offset);
	if (type3 == LUA_TNUMBER) {
		const int count = lua_plot_count(L, 3 + Offset);
		if (count > 0)
			ImPlot::PlotLine<T>(label_id, v1, count,
				lua_opt_number(args, 4 + Offset, 1),
				lua_opt_number(args, 5 + Offset, 0),
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	if (type3 == LUA_TCDATA) {
		LUA_GET_PTR(v2, 3 + Offset);
		const int count = lua_plot_count(L, 4 + Offset);
		if (count > 0)
			ImPlot::PlotLine<T>(label_id, v1, v2, count,
				lua_plot_spec(L, 5 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	return lua_param_error(3 + Offset);
}
DEF_PLOT_C(implot_plotLine);
static int implot_plotScatter(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	const auto type = lua_type(L, 3);
	if (type == LUA_TNUMBER) {
		const auto count = lua_check_count(L, 3, v1.size());
		if (count > 0)
			ImPlot::PlotScatter<double>(label_id, v1.data(), count,
				lua_opt_number(args, 4, 1),
				lua_opt_number(args, 5, 0),
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Marker));
		return 0;
	}
	if (type == LUA_TTABLE) {
		std::vector<double> v2;
		if (!luaval_to_native(L, 3, &v2))
			return lua_param_error(3);
		const auto count = lua_check_count(L, 4, std::min(v1.size(), v2.size()));
		if (count > 0)
			ImPlot::PlotScatter<double>(label_id, v1.data(), v2.data(), count,
				lua_plot_spec(L, 5, sizeof(double), true, false, count, PlotSpecArrays_Marker));
		return 0;
	}
	return lua_param_error(3);
}
DEF_PLOT_T(implot_plotScatter) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	const auto type3 = lua_type(L, 3 + Offset);
	if (type3 == LUA_TNUMBER) {
		const int count = lua_plot_count(L, 3 + Offset);
		if (count > 0)
			ImPlot::PlotScatter<T>(label_id, v1, count,
				lua_opt_number(args, 4 + Offset, 1),
				lua_opt_number(args, 5 + Offset, 0),
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Marker));
		return 0;
	}
	if (type3 == LUA_TCDATA) {
		LUA_GET_PTR(v2, 3 + Offset);
		const int count = lua_plot_count(L, 4 + Offset);
		if (count > 0)
			ImPlot::PlotScatter<T>(label_id, v1, v2, count,
				lua_plot_spec(L, 5 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Marker));
		return 0;
	}
	return lua_param_error(3 + Offset);
}
DEF_PLOT_C(implot_plotScatter);
static int implot_plotBubbles(lua_State* L) {
	const int args = lua_gettop(L);
	const char* label_id = luaL_checkstring(L, 1);
	std::vector<double> v1, v2;
	if (!luaval_to_native(L, 2, &v1)) return lua_param_error(2);
	if (!luaval_to_native(L, 3, &v2)) return lua_param_error(3);
	const auto type = lua_type(L, 4);
	if (type == LUA_TNUMBER) {
		// values, radii, count
		const int count = lua_check_count(L, 4, std::min(v1.size(), v2.size()));
		if (count > 0)
			ImPlot::PlotBubbles<double>(label_id, v1.data(), v2.data(), count,
				lua_opt_number(args, 5, 1), lua_opt_number(args, 6, 0),
				lua_plot_spec(L, 7, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	if (type == LUA_TTABLE) {
		// xs, ys, radii, count
		std::vector<double> radii;
		if (!luaval_to_native(L, 4, &radii)) return lua_param_error(4);
		const int count = lua_check_count(L, 5, std::min({ v1.size(), v2.size(), radii.size() }));
		if (count > 0)
			ImPlot::PlotBubbles<double>(label_id, v1.data(), v2.data(), radii.data(), count,
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	return lua_param_error(4);
}
DEF_PLOT_T(implot_plotBubbles) {
	const int args = lua_gettop(L);
	const char* label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	LUA_GET_PTR(v2, 3 + Offset);
	const auto type = lua_type(L, 4 + Offset);
	if (type == LUA_TNUMBER) {
		const int count = lua_plot_count(L, 4 + Offset);
		if (count > 0)
			ImPlot::PlotBubbles<T>(label_id, v1, v2, count,
				lua_opt_number(args, 5 + Offset, 1), lua_opt_number(args, 6 + Offset, 0),
				lua_plot_spec(L, 7 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	if (type == LUA_TCDATA) {
		LUA_GET_PTR(radii, 4 + Offset);
		const int count = lua_plot_count(L, 5 + Offset);
		if (count > 0)
			ImPlot::PlotBubbles<T>(label_id, v1, v2, radii, count,
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	return lua_param_error(4 + Offset);
}
DEF_PLOT_C(implot_plotBubbles);
static int implot_plotPolygon(lua_State* L) {
	const char* label_id = luaL_checkstring(L, 1);
	std::vector<double> xs, ys;
	if (!luaval_to_native(L, 2, &xs)) return lua_param_error(2);
	if (!luaval_to_native(L, 3, &ys)) return lua_param_error(3);
	const int count = lua_check_count(L, 4, std::min(xs.size(), ys.size()));
	if (count > 0)
		ImPlot::PlotPolygon<double>(label_id, xs.data(), ys.data(), count,
			lua_plot_spec(L, 5, sizeof(double), true));
	return 0;
}
DEF_PLOT_T(implot_plotPolygon) {
	const char* label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(xs, 2 + Offset);
	LUA_GET_PTR(ys, 3 + Offset);
	const int count = lua_plot_count(L, 4 + Offset);
	if (count > 0)
		ImPlot::PlotPolygon<T>(label_id, xs, ys, count,
			lua_plot_spec(L, 5 + Offset, sizeof(T), false, true));
	return 0;
}
DEF_PLOT_C(implot_plotPolygon);
static int implot_plotStairs(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	const auto type2 = lua_type(L, 3);
	if (type2 == LUA_TNUMBER) {
		const auto count = lua_check_count(L, 3, v1.size());
		if (count > 0)
			ImPlot::PlotStairs<double>(label_id, v1.data(), count,
				lua_opt_number(args, 4, 1),
				lua_opt_number(args, 5, 0),
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	if (type2 == LUA_TTABLE) {
		std::vector<double> v2;
		if (!luaval_to_native(L, 3, &v2))
			return lua_param_error(3);
		const auto count = lua_check_count(L, 4, std::min(v1.size(), v2.size()));
		if (count > 0)
			ImPlot::PlotStairs<double>(label_id, v1.data(), v2.data(), count,
				lua_plot_spec(L, 5, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	return lua_param_error(3);
}
DEF_PLOT_T(implot_plotStairs) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	const auto type3 = lua_type(L, 3 + Offset);
	if (type3 == LUA_TNUMBER) {
		const int count = lua_plot_count(L, 3 + Offset);
		if (count > 0)
			ImPlot::PlotStairs<T>(label_id, v1, count,
				lua_opt_number(args, 4 + Offset, 1),
				lua_opt_number(args, 5 + Offset, 0),
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	if (type3 == LUA_TCDATA) {
		LUA_GET_PTR(v2, 3 + Offset);
		const int count = lua_plot_count(L, 4 + Offset);
		if (count > 0)
			ImPlot::PlotStairs<T>(label_id, v1, v2, count,
				lua_plot_spec(L, 5 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill | PlotSpecArrays_Marker));
		return 0;
	}
	return lua_param_error(3 + Offset);
}
DEF_PLOT_C(implot_plotStairs);
static int implot_plotShaded(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	const auto type3 = lua_type(L, 3);
	if (type3 == LUA_TNUMBER) {
		// values
		const auto count = lua_check_count(L, 3, v1.size());
		if (count > 0)
			ImPlot::PlotShaded<double>(label_id, v1.data(), count,
				lua_opt_number(args, 4, 0),
				lua_opt_number(args, 5, 1),
				lua_opt_number(args, 6, 0),
				lua_plot_spec(L, 7, sizeof(double), true, false, count, PlotSpecArrays_Fill));
		return 0;
	}
	std::vector<double> v2;
	if (!luaval_to_native(L, 3, &v2))
		return lua_param_error(3);
	const auto type4 = lua_type(L, 4);
	if (type4 == LUA_TNUMBER) {
		// xs, ys
		const auto count = lua_check_count(L, 4, std::min(v1.size(), v2.size()));
		if (count > 0)
			ImPlot::PlotShaded<double>(label_id, v1.data(), v2.data(), count,
				lua_opt_number(args, 5, 0),
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Fill));
		return 0;
	}
	if (type4 == LUA_TTABLE) {
		// xs, ys1, ys2
		std::vector<double> v3;
		if (!luaval_to_native(L, 4, &v3))
			return lua_param_error(4);
		const auto count = lua_check_count(L, 5,
			std::min({ v1.size(), v2.size(), v3.size() }));
		if (count > 0)
			ImPlot::PlotShaded<double>(label_id,
				v1.data(), v2.data(), v3.data(), count,
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Fill));
		return 0;
	}
	return lua_param_error(4);
}
DEF_PLOT_T(implot_plotShaded) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	const auto type3 = lua_type(L, 3 + Offset);
	if (type3 == LUA_TNUMBER) {
		// values
		const int count = lua_plot_count(L, 3 + Offset);
		if (count > 0)
			ImPlot::PlotShaded<T>(label_id, v1, count,
				lua_opt_number(args, 4 + Offset, 0),
				lua_opt_number(args, 5 + Offset, 1),
				lua_opt_number(args, 6 + Offset, 0),
				lua_plot_spec(L, 7 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Fill));
		return 0;
	}
	LUA_GET_PTR(v2, 3 + Offset);
	const auto type4 = lua_type(L, 4 + Offset);
	if (type4 == LUA_TNUMBER) {
		// xs, ys
		const int count = lua_plot_count(L, 4 + Offset);
		if (count > 0)
			ImPlot::PlotShaded<T>(label_id, v1, v2, count,
				lua_opt_number(args, 5 + Offset, 0),
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Fill));
		return 0;
	}
	if (type4 == LUA_TCDATA) {
		// xs, ys1, ys2
		LUA_GET_PTR(v3, 4 + Offset);
		const int count = lua_plot_count(L, 5 + Offset);
		if (count > 0)
			ImPlot::PlotShaded<T>(label_id, v1, v2, v3, count,
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Fill));
		return 0;
	}
	return lua_param_error(4 + Offset);
}
DEF_PLOT_C(implot_plotShaded);
static int implot_plotBars(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	const auto type3 = lua_type(L, 3);
	if (type3 == LUA_TNUMBER) {
		// values
		const auto count = lua_check_count(L, 3, v1.size());
		if (count > 0)
			ImPlot::PlotBars<double>(label_id, v1.data(), count,
				lua_opt_number(args, 4, 0.67),
				lua_opt_number(args, 5, 0),
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	if (type3 == LUA_TTABLE) {
		// xs, ys
		std::vector<double> v2;
		if (!luaval_to_native(L, 3, &v2))
			return lua_param_error(3);
		const auto count = lua_check_count(L, 4, std::min(v1.size(), v2.size()));
		if (count > 0)
			ImPlot::PlotBars<double>(label_id, v1.data(), v2.data(), count,
				luaL_checknumber(L, 5),
				lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	return lua_param_error(3);
}
DEF_PLOT_T(implot_plotBars) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	const auto type3 = lua_type(L, 3 + Offset);
	if (type3 == LUA_TNUMBER) {
		// values
		const int count = lua_plot_count(L, 3 + Offset);
		if (count > 0)
			ImPlot::PlotBars<T>(label_id, v1, count,
				lua_opt_number(args, 4 + Offset, 0.67),
				lua_opt_number(args, 5 + Offset, 0),
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	if (type3 == LUA_TCDATA) {
		// xs, ys
		LUA_GET_PTR(v2, 3 + Offset);
		const int count = lua_plot_count(L, 4 + Offset);
		if (count > 0)
			ImPlot::PlotBars<T>(label_id, v1, v2, count,
				luaL_checknumber(L, 5 + Offset),
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Fill));
		return 0;
	}
	return lua_param_error(3 + Offset);
}
DEF_PLOT_C(implot_plotBars);
static int implot_plotBarGroups(lua_State* L) {
	const int args = lua_gettop(L);
	std::vector<std::string> label_storage;
	if (!luaval_to_native(L, 1, &label_storage))
		return lua_param_error(1);
	std::vector<const char*> labelsArr;
	for (const auto& label : label_storage) labelsArr.push_back(label.c_str());
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	const int count = lua_plot_count(L, 3);
	const int groups = lua_plot_count(L, 4);
	const int total = lua_plot_area(L, count, groups);
	if (static_cast<size_t>(count) > labelsArr.size() || static_cast<size_t>(total) > v1.size())
		return luaL_error(L, "bar groups require item_count labels and item_count * group_count values");
	if (groups > (std::numeric_limits<int>::max)() / 4)
		return luaL_error(L, "too many bar groups");
	if (count > 0 && groups > 0)
		ImPlot::PlotBarGroups<double>(labelsArr.data(), v1.data(),
			count, groups,
			lua_opt_number(args, 5, 0.67),
			lua_opt_number(args, 6, 0),
			lua_plot_spec(L, 7, sizeof(double), true, false, groups, PlotSpecArrays_Line | PlotSpecArrays_Fill));
	return 0;
}
DEF_PLOT_T(implot_plotBarGroups) {
	const int args = lua_gettop(L);
	std::vector<std::string> label_storage;
	if (!luaval_to_native(L, 1, &label_storage))
		return lua_param_error(1);
	std::vector<const char*> labelsArr;
	for (const auto& label : label_storage) labelsArr.push_back(label.c_str());
	LUA_GET_PTR(v1, 2 + Offset);
	const int count = lua_plot_count(L, 3 + Offset);
	const int groups = lua_plot_count(L, 4 + Offset);
	lua_plot_area(L, count, groups);
	if (static_cast<size_t>(count) > labelsArr.size())
		return luaL_error(L, "bar groups require item_count labels");
	if (groups > (std::numeric_limits<int>::max)() / 4)
		return luaL_error(L, "too many bar groups");
	if (count > 0 && groups > 0)
		ImPlot::PlotBarGroups<T>(labelsArr.data(), v1,
			count,
			groups,
			lua_opt_number(args, 5 + Offset, 0.67),
			lua_opt_number(args, 6 + Offset, 0),
			lua_plot_spec(L, 7 + Offset, sizeof(T), false, false, groups, PlotSpecArrays_Line | PlotSpecArrays_Fill));
	return 0;
}
DEF_PLOT_C(implot_plotBarGroups);
static int implot_plotErrorBars(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	std::vector<double> v2;
	if (!luaval_to_native(L, 3, &v2))
		return lua_param_error(3);
	std::vector<double> v3;
	if (!luaval_to_native(L, 4, &v3))
		return lua_param_error(4);
	const auto type5 = lua_type(L, 5);
	if (type5 == LUA_TNUMBER) {
		// xs, ys, err
		const auto count = lua_check_count(L, 5,
			std::min({ v1.size(), v2.size(), v3.size() }));
		if (count > 0)
			ImPlot::PlotErrorBars<double>(label_id, v1.data(), v2.data(), v3.data(), count,
				lua_plot_spec(L, 6, sizeof(double), true));
		return 0;
	}
	if (type5 == LUA_TTABLE) {
		// xs, ys, neg, pos
		std::vector<double> v4;
		if (!luaval_to_native(L, 5, &v4))
			return lua_param_error(5);
		const auto count = lua_check_count(L, 6,
			std::min({ v1.size(), v2.size(), v3.size(), v4.size() }));
		if (count > 0)
			ImPlot::PlotErrorBars<double>(label_id, v1.data(), v2.data(), v3.data(), v4.data(), count,
				lua_plot_spec(L, 7, sizeof(double), true));
		return 0;
	}
	return lua_param_error(5);
}
DEF_PLOT_T(implot_plotErrorBars) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	LUA_GET_PTR(v2, 3 + Offset);
	LUA_GET_PTR(v3, 4 + Offset);
	const auto type5 = lua_type(L, 5 + Offset);
	if (type5 == LUA_TNUMBER) {
		const int count = lua_plot_count(L, 5 + Offset);
		if (count > 0)
			ImPlot::PlotErrorBars<T>(label_id, v1, v2, v3, count,
				lua_plot_spec(L, 6 + Offset, sizeof(T), false, true));
		return 0;
	}
	if (type5 == LUA_TCDATA) {
		LUA_GET_PTR(v4, 5 + Offset);
		const int count = lua_plot_count(L, 6 + Offset);
		if (count > 0)
			ImPlot::PlotErrorBars<T>(label_id, v1, v2, v3, v4, count,
				lua_plot_spec(L, 7 + Offset, sizeof(T), false, true));
		return 0;
	}
	return lua_param_error(5 + Offset);
}
DEF_PLOT_C(implot_plotErrorBars);
static int implot_plotStems(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	const auto type3 = lua_type(L, 3);
	if (type3 == LUA_TNUMBER) {
		// values
		const auto count = lua_check_count(L, 3, v1.size());
		if (count > 0)
			ImPlot::PlotStems<double>(label_id, v1.data(), count,
				lua_opt_number(args, 4, 0),
				lua_opt_number(args, 5, 1),
				lua_opt_number(args, 6, 0),
				lua_plot_spec(L, 7, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Marker));
		return 0;
	}
	std::vector<double> v2;
	if (!luaval_to_native(L, 3, &v2))
		return lua_param_error(3);
	// xs, ys
	const auto count = lua_check_count(L, 4, std::min(v1.size(), v2.size()));
	if (count > 0)
		ImPlot::PlotStems<double>(label_id, v1.data(), v2.data(), count,
			lua_opt_number(args, 5, 0),
			lua_plot_spec(L, 6, sizeof(double), true, false, count, PlotSpecArrays_Line | PlotSpecArrays_Marker));
	return 0;
}
DEF_PLOT_T(implot_plotStems) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	const auto type3 = lua_type(L, 3 + Offset);
	if (type3 == LUA_TNUMBER) {
		// values
		const int count = lua_plot_count(L, 3 + Offset);
		if (count > 0)
			ImPlot::PlotStems<T>(label_id, v1, count,
				lua_opt_number(args, 4 + Offset, 0),
				lua_opt_number(args, 5 + Offset, 1),
				lua_opt_number(args, 6 + Offset, 0),
				lua_plot_spec(L, 7 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Marker));
		return 0;
	}
	LUA_GET_PTR(v2, 3 + Offset);
	// xs, ys
	const int count = lua_plot_count(L, 4 + Offset);
	if (count > 0)
		ImPlot::PlotStems<T>(label_id, v1, v2, count,
			lua_opt_number(args, 5 + Offset, 0),
			lua_plot_spec(L, 6 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line | PlotSpecArrays_Marker));
	return 0;
}
DEF_PLOT_C(implot_plotStems);
static int implot_plotInfLines(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> xs;
	if (!luaval_to_native(L, 2, &xs))
		return lua_param_error(2);
	const auto count = lua_check_count(L, 3, xs.size());
	if (count > 0)
		ImPlot::PlotInfLines<double>(
			label_id, xs.data(),
			count,
			lua_plot_spec(L, 4, sizeof(double), true, false, count, PlotSpecArrays_Line));
	return 0;
}
DEF_PLOT_T(implot_plotInfLines) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(xs, 2 + Offset);
	const int count = lua_plot_count(L, 3 + Offset);
	if (count > 0)
		ImPlot::PlotInfLines<T>(label_id, xs, count,
			lua_plot_spec(L, 4 + Offset, sizeof(T), false, true, count, PlotSpecArrays_Line));
	return 0;
}
DEF_PLOT_C(implot_plotInfLines);
static int implot_plotPieChart(lua_State* L) {
	const int args = lua_gettop(L);
	std::vector<std::string> label_storage;
	if (!luaval_to_native(L, 1, &label_storage))
		return lua_param_error(1);
	std::vector<const char*> label_ids;
	for (const auto& label : label_storage) label_ids.push_back(label.c_str());
	std::vector<double> values;
	if (!luaval_to_native(L, 2, &values))
		return lua_param_error(2);
	const int count = lua_check_count(L, 3, std::min(label_ids.size(), values.size()));
	if (count == 0) return 0;
	ImPlot::PlotPieChart<double>(label_ids.data(), values.data(), count,
		luaL_checknumber(L, 4),
		luaL_checknumber(L, 5),
		luaL_checknumber(L, 6),
		lua_opt_string(args, 7, "%.1f"),
		lua_opt_number(args, 8, 90),
		lua_plot_spec(L, 9, sizeof(double), true));
	return 0;
}
DEF_PLOT_T(implot_plotPieChart) {
	const int args = lua_gettop(L);
	std::vector<std::string> label_storage;
	if (!luaval_to_native(L, 1, &label_storage))
		return lua_param_error(1);
	std::vector<const char*> label_ids;
	for (const auto& label : label_storage) label_ids.push_back(label.c_str());
	LUA_GET_PTR(v1, 2 + Offset);
	const int count = lua_check_count(L, 3 + Offset, label_ids.size());
	if (count == 0) return 0;
	ImPlot::PlotPieChart<T>(label_ids.data(), v1, count,
		luaL_checknumber(L, 4 + Offset),
		luaL_checknumber(L, 5 + Offset),
		luaL_checknumber(L, 6 + Offset),
		lua_opt_string(args, 7 + Offset, "%.1f"),
		lua_opt_number(args, 8 + Offset, 90),
		lua_plot_spec(L, 9 + Offset, sizeof(T), false));
	return 0;
}
DEF_PLOT_C(implot_plotPieChart);
static int implot_plotHeatmap(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> values;
	if (!luaval_to_native(L, 2, &values))
		return lua_param_error(2);
	const int rows = lua_plot_count(L, 3);
	const int cols = lua_plot_count(L, 4);
	const int total = lua_plot_area(L, rows, cols);
	if (static_cast<size_t>(total) > values.size())
		return luaL_error(L, "heatmap requires rows * cols values");
	if (total == 0) return 0;
	ImPlot::PlotHeatmap<double>(label_id, values.data(), rows, cols,
		luaL_optnumber(L, 5, 0),
		luaL_optnumber(L, 6, 0),
		lua_opt_string(args, 7, "%.1f"),
		lua_opt_impoint(args, 8, ImPlotPoint(0, 0)),
		lua_opt_impoint(args, 9, ImPlotPoint(1, 1)),
		lua_plot_spec(L, 10, sizeof(double), true));
	return 0;
}
DEF_PLOT_T(implot_plotHeatmap) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	const int rows = lua_plot_count(L, 3 + Offset);
	const int cols = lua_plot_count(L, 4 + Offset);
	if (lua_plot_area(L, rows, cols) == 0) return 0;
	ImPlot::PlotHeatmap<T>(label_id, v1, rows, cols,
		luaL_optnumber(L, 5 + Offset, 0),
		luaL_optnumber(L, 6 + Offset, 0),
		lua_opt_string(args, 7 + Offset, "%.1f"),
		lua_opt_impoint(args, 8 + Offset, ImPlotPoint(0, 0)),
		lua_opt_impoint(args, 9 + Offset, ImPlotPoint(1, 1)),
		lua_plot_spec(L, 10 + Offset, sizeof(T), false));
	return 0;
}
DEF_PLOT_C(implot_plotHeatmap);
static int implot_plotHistogram(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> values;
	if (!luaval_to_native(L, 2, &values))
		return lua_param_error(2);
	const int count = lua_check_count(L, 3, values.size());
	if (count == 0) { lua_pushnumber(L, 0); return 1; }
	const int bins = lua_opt_int(args, 4, ImPlotBin_Sturges);
	const double bar_scale = lua_opt_number(args, 5, 1.0);
	const ImPlotRange range = lua_opt_imrange(args, 6, ImPlotRange());
	const ImPlotSpec spec = lua_histogram_spec(L, 7, values.data(), count, bins, range, true);
	lua_pushnumber(L, ImPlot::PlotHistogram<double>(label_id, values.data(), count, bins, bar_scale, range, spec));
	return 1;
}
DEF_PLOT_T(implot_plotHistogram) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(values, 2 + Offset);
	const int count = lua_plot_count(L, 3 + Offset);
	if (count == 0) { lua_pushnumber(L, 0); return 1; }
	const int bins = lua_opt_int(args, 4 + Offset, ImPlotBin_Sturges);
	const double bar_scale = lua_opt_number(args, 5 + Offset, 1.0);
	const ImPlotRange range = lua_opt_imrange(args, 6 + Offset, ImPlotRange());
	const ImPlotSpec spec = lua_histogram_spec(L, 7 + Offset, values, count, bins, range, false);
	lua_pushnumber(L, ImPlot::PlotHistogram<T>(label_id, values, count, bins, bar_scale, range, spec));
	return 1;
}
DEF_PLOT_C(implot_plotHistogram);
static int implot_plotHistogram2D(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> xs;
	if (!luaval_to_native(L, 2, &xs))
		return lua_param_error(2);
	std::vector<double> ys;
	if (!luaval_to_native(L, 3, &ys))
		return lua_param_error(3);
	const int count = lua_check_count(L, 4, std::min(xs.size(), ys.size()));
	if (count == 0) { lua_pushnumber(L, 0); return 1; }
	lua_pushnumber(L, ImPlot::PlotHistogram2D<double>(
		label_id, xs.data(), ys.data(), count,
		lua_opt_int(args, 5, ImPlotBin_Sturges),
		lua_opt_int(args, 6, ImPlotBin_Sturges),
		lua_opt_imrect(args, 7, ImPlotRect()),
		lua_plot_spec(L, 8, sizeof(double), true)));
	return 1;
}
DEF_PLOT_T(implot_plotHistogram2D) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(xs, 2 + Offset);
	LUA_GET_PTR(ys, 3 + Offset);
	const int count = lua_plot_count(L, 4 + Offset);
	if (count == 0) { lua_pushnumber(L, 0); return 1; }
	lua_pushnumber(L, ImPlot::PlotHistogram2D<T>(
		label_id, xs, ys, count,
		lua_opt_int(args, 5 + Offset, ImPlotBin_Sturges),
		lua_opt_int(args, 6 + Offset, ImPlotBin_Sturges),
		lua_opt_imrect(args, 7 + Offset, ImPlotRect()),
		lua_plot_spec(L, 8 + Offset, sizeof(T), false)));
	return 1;
}
DEF_PLOT_C(implot_plotHistogram2D);
static int implot_plotDigital(lua_State* L) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	std::vector<double> v1;
	if (!luaval_to_native(L, 2, &v1))
		return lua_param_error(2);
	std::vector<double> v2;
	if (!luaval_to_native(L, 3, &v2))
		return lua_param_error(3);
	const auto count = lua_check_count(L, 4, std::min({ v1.size(), v2.size() }));
	if (count > 0)
		ImPlot::PlotDigital(
			label_id, v1.data(), v2.data(),
			count,
			lua_plot_spec(L, 5, sizeof(double), true));
	return 0;
}
DEF_PLOT_T(implot_plotDigital) {
	const int args = lua_gettop(L);
	const auto label_id = luaL_checkstring(L, 1);
	LUA_GET_PTR(v1, 2 + Offset);
	LUA_GET_PTR(v2, 3 + Offset);
	const int count = lua_plot_count(L, 4 + Offset);
	if (count > 0)
		ImPlot::PlotDigital(label_id, v1, v2, count,
			lua_plot_spec(L, 5 + Offset, sizeof(T), false, true));
	return 0;
}
DEF_PLOT_C(implot_plotDigital);
static int implot_plotImage(lua_State* L) {
	const int args = lua_gettop(L);
	cocos2d::Texture2D* tex = nullptr;
	if (luaval_to_object(L, 2, "cc.Texture2D", &tex))
	{
		if (!tex)
			return 0;
		CCIMGUI::getInstance()->plotImage(
			luaL_checkstring(L, 1),
			tex,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			lua_opt_imv2(args, 5, ImVec2(0, 0)),
			lua_opt_imv2(args, 6, ImVec2(1, 1)),
			lua_opt_imv4(args, 7, ImVec4(1, 1, 1, 1)));
		return 0;
	}
	cocos2d::Sprite* sp = nullptr;
	if (luaval_to_object(L, 2, "cc.Sprite", &sp))
	{
		if (!sp)
			return 0;
		CCIMGUI::getInstance()->plotImage(
			luaL_checkstring(L, 1),
			sp,
			_luaval_to_imvec2(L, 3),
			_luaval_to_imvec2(L, 4),
			lua_opt_imv4(args, 5, ImVec4(0, 0, 0, 0)));
		return 0;
	}
	return 0;
}
static int implot_dragPoint(lua_State* L) {
	const int args = lua_gettop(L);
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	lua_pushboolean(L,
		ImPlot::DragPoint(
			luaL_checkint(L, 1),
			&x,
			&y,
			_luaval_to_imvec4(L, 4),
			lua_opt_number(args, 5, 4),
			lua_opt_int(args, 6, 0)));
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 3;
}
static int implot_dragLineX(lua_State* L) {
	const int args = lua_gettop(L);
	double value = luaL_checknumber(L, 2);
	lua_pushboolean(L,
		ImPlot::DragLineX(
			luaL_checkint(L, 1),
			&value,
			_luaval_to_imvec4(L, 3),
			lua_opt_number(args, 4, 1),
			lua_opt_int(args, 5, 0)));
	lua_pushnumber(L, value);
	return 2;
}
static int implot_dragLineY(lua_State* L) {
	const int args = lua_gettop(L);
	double value = luaL_checknumber(L, 2);
	lua_pushboolean(L,
		ImPlot::DragLineY(
			luaL_checkint(L, 1),
			&value,
			_luaval_to_imvec4(L, 3),
			lua_opt_number(args, 4, 1),
			lua_opt_int(args, 5, 0)));
	lua_pushnumber(L, value);
	return 2;
}
static int implot_dragRect(lua_State* L) {
	const int args = lua_gettop(L);
	double x1 = luaL_checknumber(L, 2);
	double y1 = luaL_checknumber(L, 3);
	double x2 = luaL_checknumber(L, 4);
	double y2 = luaL_checknumber(L, 5);
	lua_pushboolean(L,
		ImPlot::DragRect(
			luaL_checkint(L, 1),
			&x1, &y1, &x2, &y2,
			_luaval_to_imvec4(L, 6),
			lua_opt_int(args, 7, 0)));
	lua_pushnumber(L, x1);
	lua_pushnumber(L, y1);
	lua_pushnumber(L, x2);
	lua_pushnumber(L, y2);
	return 3;
}
static int implot_annotation(lua_State* L) {
	const int args = lua_gettop(L);
	if (args >= 6 && lua_isstring(L, 6))
	{
		ImPlot::Annotation(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			_luaval_to_imvec4(L, 3),
			_luaval_to_imvec2(L, 4),
			lua_toboolean(L, 5),
			"%s",
			luaL_checkstring(L, 6));
	}
	else
	{
		ImPlot::Annotation(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			_luaval_to_imvec4(L, 3),
			_luaval_to_imvec2(L, 4),
			lua_toboolean(L, 5),
			lua_opt_bool(args, 6, false));
	}
	return 0;
}
static int implot_tagX(lua_State* L) {
	const int args = lua_gettop(L);
	if (args >= 3 && lua_isstring(L, 3))
	{
		ImPlot::TagX(
			luaL_checknumber(L, 1),
			_luaval_to_imvec4(L, 2),
			"%s",
			luaL_checkstring(L, 3));
	}
	else
	{
		ImPlot::TagX(
			luaL_checknumber(L, 1),
			_luaval_to_imvec4(L, 2),
			lua_opt_bool(args, 3, false));
	}
	return 0;
}
static int implot_tagY(lua_State* L) {
	const int args = lua_gettop(L);
	if (args >= 3 && lua_isstring(L, 3))
	{
		ImPlot::TagY(
			luaL_checknumber(L, 1),
			_luaval_to_imvec4(L, 2),
			"%s",
			luaL_checkstring(L, 3));
	}
	else
	{
		ImPlot::TagY(
			luaL_checknumber(L, 1),
			_luaval_to_imvec4(L, 2),
			lua_opt_bool(args, 3, false));
	}
	return 0;
}
static int implot_getStyle(lua_State* L) {
	object_to_luaval(L, "implot.ImPlotStyle", &ImPlot::GetStyle());
	return 1;
}
static int implot_pushStyleColor(lua_State* L) {
	const auto idx = (ImPlotCol)luaL_checkinteger(L, 1);
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
static int implot_pushStyleVar(lua_State* L) {
	enum class ImPlotStyleVarType { Int, Float, ImVec2 };
	const auto idx = (ImPlotStyleVar)luaL_checkinteger(L, 1);
	if (idx < 0 || idx >= ImPlotStyleVar_COUNT)
		return lua_param_error(1);
	ImPlotStyleVarType type = ImPlotStyleVarType::ImVec2;
	if (idx == ImPlotStyleVar_PlotBorderSize
		|| idx == ImPlotStyleVar_MinorAlpha
		|| idx == ImPlotStyleVar_DigitalPadding
		|| idx == ImPlotStyleVar_DigitalSpacing)
		type = ImPlotStyleVarType::Float;

	switch (type)
	{
	case ImPlotStyleVarType::Int:
		ImPlot::PushStyleVar(idx, (int)luaL_checkinteger(L, 2));
		break;
	case ImPlotStyleVarType::Float:
		ImPlot::PushStyleVar(idx, (float)luaL_checknumber(L, 2));
		break;
	case ImPlotStyleVarType::ImVec2:
		ImPlot::PushStyleVar(idx, _luaval_to_imvec2(L, 2));
		break;
	default: ;
	}
	return 0;
}
static int implot_addColormap(lua_State* L) {
	const int args = lua_gettop(L);
	const auto name = luaL_checkstring(L, 1);
	if (!lua_istable(L, 2))
		return lua_param_error(2);
	lua_pushinteger(L, 1);
	lua_gettable(L, 2);
	const auto type_element = lua_type(L, -1);
	lua_pop(L, 1);
	if (type_element == LUA_TNUMBER)
	{
		std::vector<ImU32> cols;
		if (!luaval_to_native(L, 2, &cols))
			return lua_param_error(2);
		lua_pushinteger(L,
			ImPlot::AddColormap(name, cols.data(), cols.size(), lua_opt_bool(args, 3, true)));
	}
	else if (type_element == LUA_TTABLE)
	{
		std::vector<ImVec4> cols;
		if (!luaval_to_native(L, 2, &cols))
			return lua_param_error(2);
		lua_pushinteger(L,
			ImPlot::AddColormap(name, cols.data(), cols.size(), lua_opt_bool(args, 3, true)));
	}
	else
	{
		return lua_param_error(2);
	}
	return 1;
}
static int implot_pushColormap(lua_State* L) {
	switch (lua_type(L, 1))
	{
	case LUA_TNUMBER:
	{
		const auto idx = (ImPlotColormap)luaL_checkinteger(L, 1);
		if (idx < 0 || idx >= ImPlot::GetColormapCount())
			return lua_param_error(1);
		ImPlot::PushColormap(idx);
		return 0;
	}
	case LUA_TSTRING:
	{
		const auto idx = ImPlot::GetColormapIndex(luaL_checkstring(L, 1));
		if (idx < 0)
			return lua_param_error(1);
		ImPlot::PushColormap(idx);
		return 0;
	}
	default:
		break;
	}
	return lua_param_error(1);
}
static int implot_colormapSlider(lua_State* L) {
	const int args = lua_gettop(L);
	float t = luaL_checknumber(L, 2);
	ImVec4 out;
	lua_pushboolean(L,
		ImPlot::ColormapSlider(
			luaL_checkstring(L, 1), &t, &out, lua_opt_string(args, 4, ""), lua_opt_int(args, 5, IMPLOT_AUTO)));
	lua_pushnumber(L, (lua_Number)t);
	native_to_luaval(L, out);
	return 3;
}
static int implot_bustColorCache(lua_State* L) {
	const int args = lua_gettop(L);
	ImPlot::BustColorCache(lua_opt_string_null(args, 1, nullptr));
	return 0;
}
static int implot_itemIcon(lua_State* L) {
	switch (lua_type(L, 1))
	{
	case LUA_TNUMBER:
	{
		ImPlot::ItemIcon(lua_tou32(L,1));
		return 0;
	}
	case LUA_TSTRING:
	{
		ImPlot::ItemIcon(_luaval_to_imvec4(L, 1));
		return 0;
	}
	default:
		break;
	}
	return lua_param_error(1);
}
static int implot_showMetricsWindow(lua_State* L) {
	SHOW_WINDOW(ImPlot::ShowMetricsWindow);
}
static int implot_showDemoWindow(lua_State* L) {
	SHOW_WINDOW(ImPlot::ShowDemoWindow);
}

#undef M
#define M(n) {#n, implot_##n}
#define M_PLOT(n) {#n, implot_##n}, {#n "C", implot_##n##C}
static const luaL_Reg implot_methods[] = {
	{ "setupAxisLinks", implot_axisLinks<false> },
	{ "setNextAxisLinks", implot_axisLinks<true> },
	M(beginSubplots),
	M(setupAxisTicks),
	M_PLOT(plotLine),
	M_PLOT(plotScatter),
	M_PLOT(plotBubbles),
	M_PLOT(plotPolygon),
	M_PLOT(plotStairs),
	M_PLOT(plotShaded),
	M_PLOT(plotBars),
	M_PLOT(plotBarGroups),
	M_PLOT(plotErrorBars),
	M_PLOT(plotStems),
	M_PLOT(plotInfLines),
	M_PLOT(plotPieChart),
	M_PLOT(plotHeatmap),
	M_PLOT(plotHistogram),
	M_PLOT(plotHistogram2D),
	M_PLOT(plotDigital),
	M(plotImage),
	M(dragPoint),
	M(dragLineX),
	M(dragLineY),
	M(dragRect),
	M(annotation),
	M(tagX),
	M(tagY),
	M(getStyle),
	M(pushStyleColor),
	M(pushStyleVar),
	M(addColormap),
	M(pushColormap),
	M(colormapSlider),
	M(bustColorCache),
	M(itemIcon),
	M(showMetricsWindow),
	M(showDemoWindow)
};

#undef M
#undef M_PLOT
#undef DEF_PLOT_C
#undef DEF_PLOT_T
#undef LUA_GET_PTR

void luaReg_implotManual(lua_State* L)
{
	luaReg_implot(L);

	lua_getglobal(L, "implot");
	if (lua_istable(L, -1))
	{
		for (auto&& reg : implot_methods)
			tolua_function(L, reg.name, reg.func);
		for (const char* name : { "beginPlot", "endPlot" }) {
			lua_getfield(L, -1, name);
			lua_pushboolean(L, name[0] == 'b');
			lua_pushcclosure(L, implot_plotBoundary, 2);
			lua_setfield(L, -2, name);
		}
	}
	lua_pushliteral(L, IMPLOT_VERSION);
	lua_setfield(L, -2, "version");
	lua_pop(L, 1);
}
