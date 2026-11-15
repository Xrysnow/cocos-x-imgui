#pragma once
#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include "scripting/lua-bindings/manual/lua_conversion/lua_conversion.hpp"
#include <cstdint>

bool luaval_to_ImTextureRef(lua_State* L, int lo, ImTextureRef* out, const char* name = "");

int luaopen_imgui(lua_State *L);

// A distinct binding type keeps exact selection IDs separate from ordinary integers.
struct ImGuiSelectionToken
{
    int64_t value = 0;
};

bool luaval_to_ImGuiSelectionToken(lua_State* L, int lo, ImGuiSelectionToken* outValue, const char* fName = "");
void ImGuiSelectionToken_to_luaval(lua_State* L, ImGuiSelectionToken value);

struct ImFontGlyphSnapshot
{
    ImFontGlyph value;
    bool found;

    // Copy before Lua allocation can run GC or trigger more glyph loads.
    explicit ImFontGlyphSnapshot(const ImFontGlyph* glyph) : found(glyph != nullptr) {
        if (glyph) value = *glyph;
    }
};

namespace lua
{
    template<> struct to_native<ImGuiSelectionToken> {
        static bool F(lua_State* L, int lo, ImGuiSelectionToken* outValue, const char* fName = "") {
            return luaval_to_ImGuiSelectionToken(L, lo, outValue, fName);
        }
    };

    template<> struct to_lua<ImGuiSelectionToken> {
        static void F(lua_State* L, ImGuiSelectionToken value) { ImGuiSelectionToken_to_luaval(L, value); }
    };

    template<> struct to_lua<ImGuiSelectionRequest> {
        static void F(lua_State* L, ImGuiSelectionRequest inValue) {
            if (!L) return;
            lua_createtable(L, 0, 5);
            LUA_FOR_EACH(LUA_FIELD_TO_LUA, Type, Selected, RangeDirection)
            native_to_luaval_field(L, -1, "RangeFirstItem", ImGuiSelectionToken{static_cast<int64_t>(inValue.RangeFirstItem)});
            native_to_luaval_field(L, -1, "RangeLastItem", ImGuiSelectionToken{static_cast<int64_t>(inValue.RangeLastItem)});
        }
    };

    template<> struct to_lua<ImFontGlyphSnapshot> {
        static void F(lua_State* L, const ImFontGlyphSnapshot& snapshot) {
            if (!L) return;
            if (!snapshot.found) { lua_pushnil(L); return; }
            const auto& inValue = snapshot.value;
            lua_createtable(L, 0, 13);
            native_to_luaval_field(L, -1, "Colored", static_cast<bool>(inValue.Colored));
            native_to_luaval_field(L, -1, "Visible", static_cast<bool>(inValue.Visible));
            native_to_luaval_field(L, -1, "SourceIdx", static_cast<unsigned int>(inValue.SourceIdx));
            native_to_luaval_field(L, -1, "Codepoint", static_cast<unsigned int>(inValue.Codepoint));
            LUA_FOR_EACH(LUA_FIELD_TO_LUA, AdvanceX, X0, Y0, X1, Y1, U0, V0, U1, V1)
        }
    };

    LUA_TO_LUA_STRUCT2(ImVec2, x, y);
    LUA_TO_NATIVE_STRUCT2(ImVec2, x, y);

    LUA_TO_NATIVE_STRUCT4(ImVec4, x, y, z, w);
    LUA_TO_LUA_STRUCT4(ImVec4, x, y, z, w);

    LUA_TO_LUA_STRUCT6(ImFontAtlasRect, x, y, w, h, uv0, uv1);

    LUA_TO_LUA_STRUCT2(ImPlotPoint, x, y);
    LUA_TO_NATIVE_STRUCT2(ImPlotPoint, x, y);

    template<> struct to_lua<ImPlotRange> { static void F(lua_State* L, const ImPlotRange& inValue) { if (!L) return; lua_createtable(L, 0, 2); native_to_luaval_field(L, -1, "min", inValue.Min); native_to_luaval_field(L, -1, "max", inValue.Max); } };
    template<> struct to_native<ImPlotRange> { static bool F(lua_State* L, int lo, ImPlotRange* outValue, const char* fName = "") { LUA_CHECK_TO_NATIVE; if (!is_indexable(L, lo)) return false; ImPlotRange v; bool ok = luaval_field_to_native(L, lo, "min", &v.Min, fName) && luaval_field_to_native(L, lo, "max", &v.Max, fName); if (ok) *outValue = v; return ok; } };

    template<> struct to_lua<ImPlotRect> { static void F(lua_State* L, const ImPlotRect& inValue) { if (!L) return; lua_createtable(L, 0, 2); native_to_luaval_field(L, -1, "x", inValue.X); native_to_luaval_field(L, -1, "y", inValue.Y); } };
    template<> struct to_native<ImPlotRect> { static bool F(lua_State* L, int lo, ImPlotRect* outValue, const char* fName = "") { LUA_CHECK_TO_NATIVE; if (!is_indexable(L, lo)) return false; ImPlotRect v; bool ok = luaval_field_to_native(L, lo, "x", &v.X, fName) && luaval_field_to_native(L, lo, "y", &v.Y, fName); if (ok) *outValue = v; return ok; } };

    LUA_TO_LUA_STRUCT4(ImGuiTableColumnSortSpecs, ColumnUserID, ColumnIndex, SortOrder, SortDirection);
    LUA_TO_NATIVE_STRUCT4(ImGuiTableColumnSortSpecs, ColumnUserID, ColumnIndex, SortOrder, SortDirection);

    template<> struct to_native<ImGuiWindowClass> {
        static bool F(lua_State* L, int lo, ImGuiWindowClass* outValue, const char* fName = "") {
            LUA_CHECK_TO_NATIVE;
            if (!is_indexable(L, lo)) return false;
            ImGuiWindowClass value;
            const auto read_field = [&](const char* name, auto* field) {
                lua_getfield(L, lo, name);
                const bool ok = lua_isnil(L, -1) || luaval_to_native(L, -1, field, fName);
                lua_pop(L, 1);
                return ok;
            };
            if (!read_field("ClassId", &value.ClassId) ||
                !read_field("ParentViewportId", &value.ParentViewportId) ||
                !read_field("FocusRouteParentWindowId", &value.FocusRouteParentWindowId) ||
                !read_field("ViewportFlagsOverrideSet", &value.ViewportFlagsOverrideSet) ||
                !read_field("ViewportFlagsOverrideClear", &value.ViewportFlagsOverrideClear) ||
                !read_field("TabItemFlagsOverrideSet", &value.TabItemFlagsOverrideSet) ||
                !read_field("DockNodeFlagsOverrideSet", &value.DockNodeFlagsOverrideSet) ||
                !read_field("DockingAlwaysTabBar", &value.DockingAlwaysTabBar) ||
                !read_field("DockingAllowUnclassed", &value.DockingAllowUnclassed)) return false;
            *outValue = value;
            return true;
        }
    };
}
