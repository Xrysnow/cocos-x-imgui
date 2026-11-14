#include "lua_imguiStyle_auto.hpp"
#include "imgui.h"
#include "imgui_lua.hpp"

int lua_ImGuiStyle_Alpha_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.Alpha/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->Alpha);
	return 1;
}
int lua_ImGuiStyle_Alpha_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.Alpha/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->Alpha, 2);
	return 0;
}
int lua_ImGuiStyle_AntiAliasedFill_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.AntiAliasedFill/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->AntiAliasedFill);
	return 1;
}
int lua_ImGuiStyle_AntiAliasedFill_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.AntiAliasedFill/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->AntiAliasedFill, 2);
	return 0;
}
int lua_ImGuiStyle_AntiAliasedLineEnds_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.AntiAliasedLineEnds/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->AntiAliasedLineEnds);
	return 1;
}
int lua_ImGuiStyle_AntiAliasedLineEnds_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.AntiAliasedLineEnds/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->AntiAliasedLineEnds, 2);
	return 0;
}
int lua_ImGuiStyle_AntiAliasedLines_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.AntiAliasedLines/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->AntiAliasedLines);
	return 1;
}
int lua_ImGuiStyle_AntiAliasedLines_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.AntiAliasedLines/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->AntiAliasedLines, 2);
	return 0;
}
int lua_ImGuiStyle_ButtonTextAlign_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ButtonTextAlign/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ButtonTextAlign);
	return 1;
}
int lua_ImGuiStyle_ButtonTextAlign_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ButtonTextAlign/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ButtonTextAlign, 2);
	return 0;
}
int lua_ImGuiStyle_CellPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.CellPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->CellPadding);
	return 1;
}
int lua_ImGuiStyle_CellPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.CellPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->CellPadding, 2);
	return 0;
}
int lua_ImGuiStyle_ChildBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ChildBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ChildBorderSize);
	return 1;
}
int lua_ImGuiStyle_ChildBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ChildBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ChildBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_ChildRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ChildRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ChildRounding);
	return 1;
}
int lua_ImGuiStyle_ChildRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ChildRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ChildRounding, 2);
	return 0;
}
int lua_ImGuiStyle_CircleTessellationMaxError_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.CircleTessellationMaxError/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->CircleTessellationMaxError);
	return 1;
}
int lua_ImGuiStyle_CircleTessellationMaxError_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.CircleTessellationMaxError/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->CircleTessellationMaxError, 2);
	return 0;
}
int lua_ImGuiStyle_ColorButtonPosition_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ColorButtonPosition/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ColorButtonPosition);
	return 1;
}
int lua_ImGuiStyle_ColorButtonPosition_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ColorButtonPosition/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ColorButtonPosition, 2);
	return 0;
}
int lua_ImGuiStyle_ColorMarkerSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ColorMarkerSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ColorMarkerSize);
	return 1;
}
int lua_ImGuiStyle_ColorMarkerSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ColorMarkerSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ColorMarkerSize, 2);
	return 0;
}
int lua_ImGuiStyle_ColumnsMinSpacing_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ColumnsMinSpacing/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ColumnsMinSpacing);
	return 1;
}
int lua_ImGuiStyle_ColumnsMinSpacing_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ColumnsMinSpacing/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ColumnsMinSpacing, 2);
	return 0;
}
int lua_ImGuiStyle_CurveTessellationMaxError_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.CurveTessellationMaxError/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->CurveTessellationMaxError);
	return 1;
}
int lua_ImGuiStyle_CurveTessellationMaxError_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.CurveTessellationMaxError/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->CurveTessellationMaxError, 2);
	return 0;
}
int lua_ImGuiStyle_DisabledAlpha_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DisabledAlpha/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DisabledAlpha);
	return 1;
}
int lua_ImGuiStyle_DisabledAlpha_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DisabledAlpha/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DisabledAlpha, 2);
	return 0;
}
int lua_ImGuiStyle_DisplaySafeAreaPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DisplaySafeAreaPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DisplaySafeAreaPadding);
	return 1;
}
int lua_ImGuiStyle_DisplaySafeAreaPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DisplaySafeAreaPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DisplaySafeAreaPadding, 2);
	return 0;
}
int lua_ImGuiStyle_DisplayWindowPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DisplayWindowPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DisplayWindowPadding);
	return 1;
}
int lua_ImGuiStyle_DisplayWindowPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DisplayWindowPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DisplayWindowPadding, 2);
	return 0;
}
int lua_ImGuiStyle_DockingNodeHasCloseButton_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DockingNodeHasCloseButton/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DockingNodeHasCloseButton);
	return 1;
}
int lua_ImGuiStyle_DockingNodeHasCloseButton_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DockingNodeHasCloseButton/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DockingNodeHasCloseButton, 2);
	return 0;
}
int lua_ImGuiStyle_DockingSeparatorSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DockingSeparatorSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DockingSeparatorSize);
	return 1;
}
int lua_ImGuiStyle_DockingSeparatorSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DockingSeparatorSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DockingSeparatorSize, 2);
	return 0;
}
int lua_ImGuiStyle_DragDropTargetBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DragDropTargetBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DragDropTargetBorderSize);
	return 1;
}
int lua_ImGuiStyle_DragDropTargetBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DragDropTargetBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DragDropTargetBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_DragDropTargetPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DragDropTargetPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DragDropTargetPadding);
	return 1;
}
int lua_ImGuiStyle_DragDropTargetPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DragDropTargetPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DragDropTargetPadding, 2);
	return 0;
}
int lua_ImGuiStyle_DragDropTargetRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DragDropTargetRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->DragDropTargetRounding);
	return 1;
}
int lua_ImGuiStyle_DragDropTargetRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.DragDropTargetRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->DragDropTargetRounding, 2);
	return 0;
}
int lua_ImGuiStyle_FontScaleDpi_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FontScaleDpi/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->FontScaleDpi);
	return 1;
}
int lua_ImGuiStyle_FontScaleDpi_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FontScaleDpi/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->FontScaleDpi, 2);
	return 0;
}
int lua_ImGuiStyle_FontScaleMain_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FontScaleMain/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->FontScaleMain);
	return 1;
}
int lua_ImGuiStyle_FontScaleMain_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FontScaleMain/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->FontScaleMain, 2);
	return 0;
}
int lua_ImGuiStyle_FontSizeBase_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FontSizeBase/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->FontSizeBase);
	return 1;
}
int lua_ImGuiStyle_FontSizeBase_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FontSizeBase/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->FontSizeBase, 2);
	return 0;
}
int lua_ImGuiStyle_FrameBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FrameBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->FrameBorderSize);
	return 1;
}
int lua_ImGuiStyle_FrameBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FrameBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->FrameBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_FramePadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FramePadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->FramePadding);
	return 1;
}
int lua_ImGuiStyle_FramePadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FramePadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->FramePadding, 2);
	return 0;
}
int lua_ImGuiStyle_FrameRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FrameRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->FrameRounding);
	return 1;
}
int lua_ImGuiStyle_FrameRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.FrameRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->FrameRounding, 2);
	return 0;
}
int lua_ImGuiStyle_GrabMinSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.GrabMinSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->GrabMinSize);
	return 1;
}
int lua_ImGuiStyle_GrabMinSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.GrabMinSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->GrabMinSize, 2);
	return 0;
}
int lua_ImGuiStyle_GrabRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.GrabRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->GrabRounding);
	return 1;
}
int lua_ImGuiStyle_GrabRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.GrabRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->GrabRounding, 2);
	return 0;
}
int lua_ImGuiStyle_HoverDelayNormal_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverDelayNormal/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->HoverDelayNormal);
	return 1;
}
int lua_ImGuiStyle_HoverDelayNormal_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverDelayNormal/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->HoverDelayNormal, 2);
	return 0;
}
int lua_ImGuiStyle_HoverDelayShort_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverDelayShort/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->HoverDelayShort);
	return 1;
}
int lua_ImGuiStyle_HoverDelayShort_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverDelayShort/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->HoverDelayShort, 2);
	return 0;
}
int lua_ImGuiStyle_HoverFlagsForTooltipMouse_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverFlagsForTooltipMouse/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->HoverFlagsForTooltipMouse);
	return 1;
}
int lua_ImGuiStyle_HoverFlagsForTooltipMouse_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverFlagsForTooltipMouse/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->HoverFlagsForTooltipMouse, 2);
	return 0;
}
int lua_ImGuiStyle_HoverFlagsForTooltipNav_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverFlagsForTooltipNav/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->HoverFlagsForTooltipNav);
	return 1;
}
int lua_ImGuiStyle_HoverFlagsForTooltipNav_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverFlagsForTooltipNav/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->HoverFlagsForTooltipNav, 2);
	return 0;
}
int lua_ImGuiStyle_HoverStationaryDelay_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverStationaryDelay/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->HoverStationaryDelay);
	return 1;
}
int lua_ImGuiStyle_HoverStationaryDelay_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.HoverStationaryDelay/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->HoverStationaryDelay, 2);
	return 0;
}
int lua_ImGuiStyle_ImageBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ImageBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ImageBorderSize);
	return 1;
}
int lua_ImGuiStyle_ImageBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ImageBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ImageBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_ImageRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ImageRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ImageRounding);
	return 1;
}
int lua_ImGuiStyle_ImageRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ImageRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ImageRounding, 2);
	return 0;
}
int lua_ImGuiStyle_IndentSpacing_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.IndentSpacing/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->IndentSpacing);
	return 1;
}
int lua_ImGuiStyle_IndentSpacing_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.IndentSpacing/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->IndentSpacing, 2);
	return 0;
}
int lua_ImGuiStyle_InputTextCursorSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.InputTextCursorSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->InputTextCursorSize);
	return 1;
}
int lua_ImGuiStyle_InputTextCursorSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.InputTextCursorSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->InputTextCursorSize, 2);
	return 0;
}
int lua_ImGuiStyle_ItemInnerSpacing_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ItemInnerSpacing/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ItemInnerSpacing);
	return 1;
}
int lua_ImGuiStyle_ItemInnerSpacing_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ItemInnerSpacing/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ItemInnerSpacing, 2);
	return 0;
}
int lua_ImGuiStyle_ItemSpacing_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ItemSpacing/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ItemSpacing);
	return 1;
}
int lua_ImGuiStyle_ItemSpacing_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ItemSpacing/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ItemSpacing, 2);
	return 0;
}
int lua_ImGuiStyle_LogSliderDeadzone_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.LogSliderDeadzone/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->LogSliderDeadzone);
	return 1;
}
int lua_ImGuiStyle_LogSliderDeadzone_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.LogSliderDeadzone/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->LogSliderDeadzone, 2);
	return 0;
}
int lua_ImGuiStyle_MenuItemRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.MenuItemRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->MenuItemRounding);
	return 1;
}
int lua_ImGuiStyle_MenuItemRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.MenuItemRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->MenuItemRounding, 2);
	return 0;
}
int lua_ImGuiStyle_MouseCursorScale_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.MouseCursorScale/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->MouseCursorScale);
	return 1;
}
int lua_ImGuiStyle_MouseCursorScale_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.MouseCursorScale/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->MouseCursorScale, 2);
	return 0;
}
int lua_ImGuiStyle_PopupBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.PopupBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->PopupBorderSize);
	return 1;
}
int lua_ImGuiStyle_PopupBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.PopupBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->PopupBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_PopupRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.PopupRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->PopupRounding);
	return 1;
}
int lua_ImGuiStyle_PopupRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.PopupRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->PopupRounding, 2);
	return 0;
}
int lua_ImGuiStyle_ScrollbarPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ScrollbarPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ScrollbarPadding);
	return 1;
}
int lua_ImGuiStyle_ScrollbarPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ScrollbarPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ScrollbarPadding, 2);
	return 0;
}
int lua_ImGuiStyle_ScrollbarRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ScrollbarRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ScrollbarRounding);
	return 1;
}
int lua_ImGuiStyle_ScrollbarRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ScrollbarRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ScrollbarRounding, 2);
	return 0;
}
int lua_ImGuiStyle_ScrollbarSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ScrollbarSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->ScrollbarSize);
	return 1;
}
int lua_ImGuiStyle_ScrollbarSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.ScrollbarSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->ScrollbarSize, 2);
	return 0;
}
int lua_ImGuiStyle_SelectableRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SelectableRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->SelectableRounding);
	return 1;
}
int lua_ImGuiStyle_SelectableRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SelectableRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->SelectableRounding, 2);
	return 0;
}
int lua_ImGuiStyle_SelectableTextAlign_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SelectableTextAlign/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->SelectableTextAlign);
	return 1;
}
int lua_ImGuiStyle_SelectableTextAlign_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SelectableTextAlign/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->SelectableTextAlign, 2);
	return 0;
}
int lua_ImGuiStyle_SeparatorSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->SeparatorSize);
	return 1;
}
int lua_ImGuiStyle_SeparatorSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->SeparatorSize, 2);
	return 0;
}
int lua_ImGuiStyle_SeparatorTextAlign_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorTextAlign/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->SeparatorTextAlign);
	return 1;
}
int lua_ImGuiStyle_SeparatorTextAlign_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorTextAlign/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->SeparatorTextAlign, 2);
	return 0;
}
int lua_ImGuiStyle_SeparatorTextBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorTextBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->SeparatorTextBorderSize);
	return 1;
}
int lua_ImGuiStyle_SeparatorTextBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorTextBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->SeparatorTextBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_SeparatorTextPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorTextPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->SeparatorTextPadding);
	return 1;
}
int lua_ImGuiStyle_SeparatorTextPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.SeparatorTextPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->SeparatorTextPadding, 2);
	return 0;
}
int lua_ImGuiStyle_TabBarBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabBarBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabBarBorderSize);
	return 1;
}
int lua_ImGuiStyle_TabBarBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabBarBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabBarBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_TabBarOverlineSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabBarOverlineSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabBarOverlineSize);
	return 1;
}
int lua_ImGuiStyle_TabBarOverlineSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabBarOverlineSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabBarOverlineSize, 2);
	return 0;
}
int lua_ImGuiStyle_TabBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabBorderSize);
	return 1;
}
int lua_ImGuiStyle_TabBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_TabCloseButtonMinWidthSelected_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabCloseButtonMinWidthSelected/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabCloseButtonMinWidthSelected);
	return 1;
}
int lua_ImGuiStyle_TabCloseButtonMinWidthSelected_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabCloseButtonMinWidthSelected/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabCloseButtonMinWidthSelected, 2);
	return 0;
}
int lua_ImGuiStyle_TabCloseButtonMinWidthUnselected_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabCloseButtonMinWidthUnselected/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabCloseButtonMinWidthUnselected);
	return 1;
}
int lua_ImGuiStyle_TabCloseButtonMinWidthUnselected_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabCloseButtonMinWidthUnselected/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabCloseButtonMinWidthUnselected, 2);
	return 0;
}
int lua_ImGuiStyle_TabMinWidthBase_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabMinWidthBase/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabMinWidthBase);
	return 1;
}
int lua_ImGuiStyle_TabMinWidthBase_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabMinWidthBase/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabMinWidthBase, 2);
	return 0;
}
int lua_ImGuiStyle_TabMinWidthShrink_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabMinWidthShrink/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabMinWidthShrink);
	return 1;
}
int lua_ImGuiStyle_TabMinWidthShrink_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabMinWidthShrink/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabMinWidthShrink, 2);
	return 0;
}
int lua_ImGuiStyle_TabRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TabRounding);
	return 1;
}
int lua_ImGuiStyle_TabRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TabRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TabRounding, 2);
	return 0;
}
int lua_ImGuiStyle_TableAngledHeadersAngle_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TableAngledHeadersAngle/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TableAngledHeadersAngle);
	return 1;
}
int lua_ImGuiStyle_TableAngledHeadersAngle_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TableAngledHeadersAngle/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TableAngledHeadersAngle, 2);
	return 0;
}
int lua_ImGuiStyle_TableAngledHeadersTextAlign_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TableAngledHeadersTextAlign/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TableAngledHeadersTextAlign);
	return 1;
}
int lua_ImGuiStyle_TableAngledHeadersTextAlign_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TableAngledHeadersTextAlign/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TableAngledHeadersTextAlign, 2);
	return 0;
}
int lua_ImGuiStyle_TouchExtraPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TouchExtraPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TouchExtraPadding);
	return 1;
}
int lua_ImGuiStyle_TouchExtraPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TouchExtraPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TouchExtraPadding, 2);
	return 0;
}
int lua_ImGuiStyle_TreeLinesFlags_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TreeLinesFlags/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TreeLinesFlags);
	return 1;
}
int lua_ImGuiStyle_TreeLinesFlags_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TreeLinesFlags/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TreeLinesFlags, 2);
	return 0;
}
int lua_ImGuiStyle_TreeLinesRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TreeLinesRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TreeLinesRounding);
	return 1;
}
int lua_ImGuiStyle_TreeLinesRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TreeLinesRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TreeLinesRounding, 2);
	return 0;
}
int lua_ImGuiStyle_TreeLinesSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TreeLinesSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->TreeLinesSize);
	return 1;
}
int lua_ImGuiStyle_TreeLinesSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.TreeLinesSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->TreeLinesSize, 2);
	return 0;
}
int lua_ImGuiStyle_WindowBorderHoverPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowBorderHoverPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->WindowBorderHoverPadding);
	return 1;
}
int lua_ImGuiStyle_WindowBorderHoverPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowBorderHoverPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->WindowBorderHoverPadding, 2);
	return 0;
}
int lua_ImGuiStyle_WindowBorderSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowBorderSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->WindowBorderSize);
	return 1;
}
int lua_ImGuiStyle_WindowBorderSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowBorderSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->WindowBorderSize, 2);
	return 0;
}
int lua_ImGuiStyle_WindowMenuButtonPosition_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowMenuButtonPosition/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->WindowMenuButtonPosition);
	return 1;
}
int lua_ImGuiStyle_WindowMenuButtonPosition_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowMenuButtonPosition/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->WindowMenuButtonPosition, 2);
	return 0;
}
int lua_ImGuiStyle_WindowMinSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowMinSize/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->WindowMinSize);
	return 1;
}
int lua_ImGuiStyle_WindowMinSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowMinSize/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->WindowMinSize, 2);
	return 0;
}
int lua_ImGuiStyle_WindowPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowPadding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->WindowPadding);
	return 1;
}
int lua_ImGuiStyle_WindowPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowPadding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->WindowPadding, 2);
	return 0;
}
int lua_ImGuiStyle_WindowRounding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowRounding/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->WindowRounding);
	return 1;
}
int lua_ImGuiStyle_WindowRounding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowRounding/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->WindowRounding, 2);
	return 0;
}
int lua_ImGuiStyle_WindowTitleAlign_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowTitleAlign/getter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_PUSH_NATIVE(cobj->WindowTitleAlign);
	return 1;
}
int lua_ImGuiStyle_WindowTitleAlign_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImGuiStyle.WindowTitleAlign/setter");
	auto cobj = LUA_TO_COBJ(ImGuiStyle*, 1);
	LUA_NATIVE_SETTER(cobj->WindowTitleAlign, 2);
	return 0;
}
int lua_ImGuiStyle_scaleAllSizes(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImGuiStyle", "imgui.ImGuiStyle:scaleAllSizes");
	LUA_TRY_INVOKE(1, &ImGuiStyle::ScaleAllSizes);
	LUA_INVOKE_FOOTER("1");
}
int luaReg_imguiStyle_imguiImGuiStyle(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImGuiStyle, "ImGuiStyle", "imgui.ImGuiStyle",
		nullptr, nullptr);
	LUA_FIELD("Alpha", lua_ImGuiStyle_Alpha_getter, lua_ImGuiStyle_Alpha_setter);
	LUA_FIELD("AntiAliasedFill", lua_ImGuiStyle_AntiAliasedFill_getter, lua_ImGuiStyle_AntiAliasedFill_setter);
	LUA_FIELD("AntiAliasedLineEnds", lua_ImGuiStyle_AntiAliasedLineEnds_getter, lua_ImGuiStyle_AntiAliasedLineEnds_setter);
	LUA_FIELD("AntiAliasedLines", lua_ImGuiStyle_AntiAliasedLines_getter, lua_ImGuiStyle_AntiAliasedLines_setter);
	LUA_FIELD("ButtonTextAlign", lua_ImGuiStyle_ButtonTextAlign_getter, lua_ImGuiStyle_ButtonTextAlign_setter);
	LUA_FIELD("CellPadding", lua_ImGuiStyle_CellPadding_getter, lua_ImGuiStyle_CellPadding_setter);
	LUA_FIELD("ChildBorderSize", lua_ImGuiStyle_ChildBorderSize_getter, lua_ImGuiStyle_ChildBorderSize_setter);
	LUA_FIELD("ChildRounding", lua_ImGuiStyle_ChildRounding_getter, lua_ImGuiStyle_ChildRounding_setter);
	LUA_FIELD("CircleTessellationMaxError", lua_ImGuiStyle_CircleTessellationMaxError_getter, lua_ImGuiStyle_CircleTessellationMaxError_setter);
	LUA_FIELD("ColorButtonPosition", lua_ImGuiStyle_ColorButtonPosition_getter, lua_ImGuiStyle_ColorButtonPosition_setter);
	LUA_FIELD("ColorMarkerSize", lua_ImGuiStyle_ColorMarkerSize_getter, lua_ImGuiStyle_ColorMarkerSize_setter);
	LUA_FIELD("ColumnsMinSpacing", lua_ImGuiStyle_ColumnsMinSpacing_getter, lua_ImGuiStyle_ColumnsMinSpacing_setter);
	LUA_FIELD("CurveTessellationMaxError", lua_ImGuiStyle_CurveTessellationMaxError_getter, lua_ImGuiStyle_CurveTessellationMaxError_setter);
	LUA_FIELD("DisabledAlpha", lua_ImGuiStyle_DisabledAlpha_getter, lua_ImGuiStyle_DisabledAlpha_setter);
	LUA_FIELD("DisplaySafeAreaPadding", lua_ImGuiStyle_DisplaySafeAreaPadding_getter, lua_ImGuiStyle_DisplaySafeAreaPadding_setter);
	LUA_FIELD("DisplayWindowPadding", lua_ImGuiStyle_DisplayWindowPadding_getter, lua_ImGuiStyle_DisplayWindowPadding_setter);
	LUA_FIELD("DockingNodeHasCloseButton", lua_ImGuiStyle_DockingNodeHasCloseButton_getter, lua_ImGuiStyle_DockingNodeHasCloseButton_setter);
	LUA_FIELD("DockingSeparatorSize", lua_ImGuiStyle_DockingSeparatorSize_getter, lua_ImGuiStyle_DockingSeparatorSize_setter);
	LUA_FIELD("DragDropTargetBorderSize", lua_ImGuiStyle_DragDropTargetBorderSize_getter, lua_ImGuiStyle_DragDropTargetBorderSize_setter);
	LUA_FIELD("DragDropTargetPadding", lua_ImGuiStyle_DragDropTargetPadding_getter, lua_ImGuiStyle_DragDropTargetPadding_setter);
	LUA_FIELD("DragDropTargetRounding", lua_ImGuiStyle_DragDropTargetRounding_getter, lua_ImGuiStyle_DragDropTargetRounding_setter);
	LUA_FIELD("FontScaleDpi", lua_ImGuiStyle_FontScaleDpi_getter, lua_ImGuiStyle_FontScaleDpi_setter);
	LUA_FIELD("FontScaleMain", lua_ImGuiStyle_FontScaleMain_getter, lua_ImGuiStyle_FontScaleMain_setter);
	LUA_FIELD("FontSizeBase", lua_ImGuiStyle_FontSizeBase_getter, lua_ImGuiStyle_FontSizeBase_setter);
	LUA_FIELD("FrameBorderSize", lua_ImGuiStyle_FrameBorderSize_getter, lua_ImGuiStyle_FrameBorderSize_setter);
	LUA_FIELD("FramePadding", lua_ImGuiStyle_FramePadding_getter, lua_ImGuiStyle_FramePadding_setter);
	LUA_FIELD("FrameRounding", lua_ImGuiStyle_FrameRounding_getter, lua_ImGuiStyle_FrameRounding_setter);
	LUA_FIELD("GrabMinSize", lua_ImGuiStyle_GrabMinSize_getter, lua_ImGuiStyle_GrabMinSize_setter);
	LUA_FIELD("GrabRounding", lua_ImGuiStyle_GrabRounding_getter, lua_ImGuiStyle_GrabRounding_setter);
	LUA_FIELD("HoverDelayNormal", lua_ImGuiStyle_HoverDelayNormal_getter, lua_ImGuiStyle_HoverDelayNormal_setter);
	LUA_FIELD("HoverDelayShort", lua_ImGuiStyle_HoverDelayShort_getter, lua_ImGuiStyle_HoverDelayShort_setter);
	LUA_FIELD("HoverFlagsForTooltipMouse", lua_ImGuiStyle_HoverFlagsForTooltipMouse_getter, lua_ImGuiStyle_HoverFlagsForTooltipMouse_setter);
	LUA_FIELD("HoverFlagsForTooltipNav", lua_ImGuiStyle_HoverFlagsForTooltipNav_getter, lua_ImGuiStyle_HoverFlagsForTooltipNav_setter);
	LUA_FIELD("HoverStationaryDelay", lua_ImGuiStyle_HoverStationaryDelay_getter, lua_ImGuiStyle_HoverStationaryDelay_setter);
	LUA_FIELD("ImageBorderSize", lua_ImGuiStyle_ImageBorderSize_getter, lua_ImGuiStyle_ImageBorderSize_setter);
	LUA_FIELD("ImageRounding", lua_ImGuiStyle_ImageRounding_getter, lua_ImGuiStyle_ImageRounding_setter);
	LUA_FIELD("IndentSpacing", lua_ImGuiStyle_IndentSpacing_getter, lua_ImGuiStyle_IndentSpacing_setter);
	LUA_FIELD("InputTextCursorSize", lua_ImGuiStyle_InputTextCursorSize_getter, lua_ImGuiStyle_InputTextCursorSize_setter);
	LUA_FIELD("ItemInnerSpacing", lua_ImGuiStyle_ItemInnerSpacing_getter, lua_ImGuiStyle_ItemInnerSpacing_setter);
	LUA_FIELD("ItemSpacing", lua_ImGuiStyle_ItemSpacing_getter, lua_ImGuiStyle_ItemSpacing_setter);
	LUA_FIELD("LogSliderDeadzone", lua_ImGuiStyle_LogSliderDeadzone_getter, lua_ImGuiStyle_LogSliderDeadzone_setter);
	LUA_FIELD("MenuItemRounding", lua_ImGuiStyle_MenuItemRounding_getter, lua_ImGuiStyle_MenuItemRounding_setter);
	LUA_FIELD("MouseCursorScale", lua_ImGuiStyle_MouseCursorScale_getter, lua_ImGuiStyle_MouseCursorScale_setter);
	LUA_FIELD("PopupBorderSize", lua_ImGuiStyle_PopupBorderSize_getter, lua_ImGuiStyle_PopupBorderSize_setter);
	LUA_FIELD("PopupRounding", lua_ImGuiStyle_PopupRounding_getter, lua_ImGuiStyle_PopupRounding_setter);
	LUA_FIELD("ScrollbarPadding", lua_ImGuiStyle_ScrollbarPadding_getter, lua_ImGuiStyle_ScrollbarPadding_setter);
	LUA_FIELD("ScrollbarRounding", lua_ImGuiStyle_ScrollbarRounding_getter, lua_ImGuiStyle_ScrollbarRounding_setter);
	LUA_FIELD("ScrollbarSize", lua_ImGuiStyle_ScrollbarSize_getter, lua_ImGuiStyle_ScrollbarSize_setter);
	LUA_FIELD("SelectableRounding", lua_ImGuiStyle_SelectableRounding_getter, lua_ImGuiStyle_SelectableRounding_setter);
	LUA_FIELD("SelectableTextAlign", lua_ImGuiStyle_SelectableTextAlign_getter, lua_ImGuiStyle_SelectableTextAlign_setter);
	LUA_FIELD("SeparatorSize", lua_ImGuiStyle_SeparatorSize_getter, lua_ImGuiStyle_SeparatorSize_setter);
	LUA_FIELD("SeparatorTextAlign", lua_ImGuiStyle_SeparatorTextAlign_getter, lua_ImGuiStyle_SeparatorTextAlign_setter);
	LUA_FIELD("SeparatorTextBorderSize", lua_ImGuiStyle_SeparatorTextBorderSize_getter, lua_ImGuiStyle_SeparatorTextBorderSize_setter);
	LUA_FIELD("SeparatorTextPadding", lua_ImGuiStyle_SeparatorTextPadding_getter, lua_ImGuiStyle_SeparatorTextPadding_setter);
	LUA_FIELD("TabBarBorderSize", lua_ImGuiStyle_TabBarBorderSize_getter, lua_ImGuiStyle_TabBarBorderSize_setter);
	LUA_FIELD("TabBarOverlineSize", lua_ImGuiStyle_TabBarOverlineSize_getter, lua_ImGuiStyle_TabBarOverlineSize_setter);
	LUA_FIELD("TabBorderSize", lua_ImGuiStyle_TabBorderSize_getter, lua_ImGuiStyle_TabBorderSize_setter);
	LUA_FIELD("TabCloseButtonMinWidthSelected", lua_ImGuiStyle_TabCloseButtonMinWidthSelected_getter, lua_ImGuiStyle_TabCloseButtonMinWidthSelected_setter);
	LUA_FIELD("TabCloseButtonMinWidthUnselected", lua_ImGuiStyle_TabCloseButtonMinWidthUnselected_getter, lua_ImGuiStyle_TabCloseButtonMinWidthUnselected_setter);
	LUA_FIELD("TabMinWidthBase", lua_ImGuiStyle_TabMinWidthBase_getter, lua_ImGuiStyle_TabMinWidthBase_setter);
	LUA_FIELD("TabMinWidthShrink", lua_ImGuiStyle_TabMinWidthShrink_getter, lua_ImGuiStyle_TabMinWidthShrink_setter);
	LUA_FIELD("TabRounding", lua_ImGuiStyle_TabRounding_getter, lua_ImGuiStyle_TabRounding_setter);
	LUA_FIELD("TableAngledHeadersAngle", lua_ImGuiStyle_TableAngledHeadersAngle_getter, lua_ImGuiStyle_TableAngledHeadersAngle_setter);
	LUA_FIELD("TableAngledHeadersTextAlign", lua_ImGuiStyle_TableAngledHeadersTextAlign_getter, lua_ImGuiStyle_TableAngledHeadersTextAlign_setter);
	LUA_FIELD("TouchExtraPadding", lua_ImGuiStyle_TouchExtraPadding_getter, lua_ImGuiStyle_TouchExtraPadding_setter);
	LUA_FIELD("TreeLinesFlags", lua_ImGuiStyle_TreeLinesFlags_getter, lua_ImGuiStyle_TreeLinesFlags_setter);
	LUA_FIELD("TreeLinesRounding", lua_ImGuiStyle_TreeLinesRounding_getter, lua_ImGuiStyle_TreeLinesRounding_setter);
	LUA_FIELD("TreeLinesSize", lua_ImGuiStyle_TreeLinesSize_getter, lua_ImGuiStyle_TreeLinesSize_setter);
	LUA_FIELD("WindowBorderHoverPadding", lua_ImGuiStyle_WindowBorderHoverPadding_getter, lua_ImGuiStyle_WindowBorderHoverPadding_setter);
	LUA_FIELD("WindowBorderSize", lua_ImGuiStyle_WindowBorderSize_getter, lua_ImGuiStyle_WindowBorderSize_setter);
	LUA_FIELD("WindowMenuButtonPosition", lua_ImGuiStyle_WindowMenuButtonPosition_getter, lua_ImGuiStyle_WindowMenuButtonPosition_setter);
	LUA_FIELD("WindowMinSize", lua_ImGuiStyle_WindowMinSize_getter, lua_ImGuiStyle_WindowMinSize_setter);
	LUA_FIELD("WindowPadding", lua_ImGuiStyle_WindowPadding_getter, lua_ImGuiStyle_WindowPadding_setter);
	LUA_FIELD("WindowRounding", lua_ImGuiStyle_WindowRounding_getter, lua_ImGuiStyle_WindowRounding_setter);
	LUA_FIELD("WindowTitleAlign", lua_ImGuiStyle_WindowTitleAlign_getter, lua_ImGuiStyle_WindowTitleAlign_setter);
	LUA_METHOD("scaleAllSizes", lua_ImGuiStyle_scaleAllSizes);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}
