#include "lua_imguiFont_auto.hpp"
#include "imgui.h"
#include "imgui_lua.hpp"

int lua_ImFontConfig_DstFont_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.DstFont/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->DstFont);
	return 1;
}
int lua_ImFontConfig_DstFont_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.DstFont/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->DstFont, 2);
	return 0;
}
int lua_ImFontConfig_EllipsisChar_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.EllipsisChar/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->EllipsisChar);
	return 1;
}
int lua_ImFontConfig_EllipsisChar_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.EllipsisChar/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->EllipsisChar, 2);
	return 0;
}
int lua_ImFontConfig_ExtraSizeScale_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.ExtraSizeScale/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->ExtraSizeScale);
	return 1;
}
int lua_ImFontConfig_ExtraSizeScale_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.ExtraSizeScale/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->ExtraSizeScale, 2);
	return 0;
}
int lua_ImFontConfig_Flags_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.Flags/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->Flags);
	return 1;
}
int lua_ImFontConfig_Flags_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.Flags/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->Flags, 2);
	return 0;
}
int lua_ImFontConfig_FontDataOwnedByAtlas_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontDataOwnedByAtlas/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->FontDataOwnedByAtlas);
	return 1;
}
int lua_ImFontConfig_FontDataOwnedByAtlas_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontDataOwnedByAtlas/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->FontDataOwnedByAtlas, 2);
	return 0;
}
int lua_ImFontConfig_FontDataSize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontDataSize/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->FontDataSize);
	return 1;
}
int lua_ImFontConfig_FontDataSize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontDataSize/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->FontDataSize, 2);
	return 0;
}
int lua_ImFontConfig_FontLoaderFlags_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontLoaderFlags/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->FontLoaderFlags);
	return 1;
}
int lua_ImFontConfig_FontLoaderFlags_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontLoaderFlags/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->FontLoaderFlags, 2);
	return 0;
}
int lua_ImFontConfig_FontNo_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontNo/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->FontNo);
	return 1;
}
int lua_ImFontConfig_FontNo_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.FontNo/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->FontNo, 2);
	return 0;
}
int lua_ImFontConfig_GlyphExtraAdvanceX_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphExtraAdvanceX/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->GlyphExtraAdvanceX);
	return 1;
}
int lua_ImFontConfig_GlyphExtraAdvanceX_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphExtraAdvanceX/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->GlyphExtraAdvanceX, 2);
	return 0;
}
int lua_ImFontConfig_GlyphMaxAdvanceX_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphMaxAdvanceX/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->GlyphMaxAdvanceX);
	return 1;
}
int lua_ImFontConfig_GlyphMaxAdvanceX_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphMaxAdvanceX/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->GlyphMaxAdvanceX, 2);
	return 0;
}
int lua_ImFontConfig_GlyphMinAdvanceX_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphMinAdvanceX/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->GlyphMinAdvanceX);
	return 1;
}
int lua_ImFontConfig_GlyphMinAdvanceX_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphMinAdvanceX/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->GlyphMinAdvanceX, 2);
	return 0;
}
int lua_ImFontConfig_GlyphOffset_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphOffset/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->GlyphOffset);
	return 1;
}
int lua_ImFontConfig_GlyphOffset_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.GlyphOffset/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->GlyphOffset, 2);
	return 0;
}
int lua_ImFontConfig_MergeMode_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.MergeMode/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->MergeMode);
	return 1;
}
int lua_ImFontConfig_MergeMode_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.MergeMode/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->MergeMode, 2);
	return 0;
}
int lua_ImFontConfig_OversampleH_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.OversampleH/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->OversampleH);
	return 1;
}
int lua_ImFontConfig_OversampleH_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.OversampleH/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->OversampleH, 2);
	return 0;
}
int lua_ImFontConfig_OversampleV_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.OversampleV/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->OversampleV);
	return 1;
}
int lua_ImFontConfig_OversampleV_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.OversampleV/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->OversampleV, 2);
	return 0;
}
int lua_ImFontConfig_PixelSnapH_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.PixelSnapH/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->PixelSnapH);
	return 1;
}
int lua_ImFontConfig_PixelSnapH_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.PixelSnapH/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->PixelSnapH, 2);
	return 0;
}
int lua_ImFontConfig_RasterizerDensity_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.RasterizerDensity/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->RasterizerDensity);
	return 1;
}
int lua_ImFontConfig_RasterizerDensity_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.RasterizerDensity/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->RasterizerDensity, 2);
	return 0;
}
int lua_ImFontConfig_RasterizerMultiply_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.RasterizerMultiply/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->RasterizerMultiply);
	return 1;
}
int lua_ImFontConfig_RasterizerMultiply_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.RasterizerMultiply/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->RasterizerMultiply, 2);
	return 0;
}
int lua_ImFontConfig_SizePixels_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.SizePixels/getter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_PUSH_NATIVE(cobj->SizePixels);
	return 1;
}
int lua_ImFontConfig_SizePixels_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontConfig.SizePixels/setter");
	auto cobj = LUA_TO_COBJ(ImFontConfig*, 1);
	LUA_NATIVE_SETTER(cobj->SizePixels, 2);
	return 0;
}
int luaReg_imguiFont_imguiImFontConfig(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImFontConfig, "ImFontConfig", "imgui.ImFontConfig",
		nullptr, nullptr);
	LUA_FIELD("DstFont", lua_ImFontConfig_DstFont_getter, lua_ImFontConfig_DstFont_setter);
	LUA_FIELD("EllipsisChar", lua_ImFontConfig_EllipsisChar_getter, lua_ImFontConfig_EllipsisChar_setter);
	LUA_FIELD("ExtraSizeScale", lua_ImFontConfig_ExtraSizeScale_getter, lua_ImFontConfig_ExtraSizeScale_setter);
	LUA_FIELD("Flags", lua_ImFontConfig_Flags_getter, lua_ImFontConfig_Flags_setter);
	LUA_FIELD("FontDataOwnedByAtlas", lua_ImFontConfig_FontDataOwnedByAtlas_getter, lua_ImFontConfig_FontDataOwnedByAtlas_setter);
	LUA_FIELD("FontDataSize", lua_ImFontConfig_FontDataSize_getter, lua_ImFontConfig_FontDataSize_setter);
	LUA_FIELD("FontLoaderFlags", lua_ImFontConfig_FontLoaderFlags_getter, lua_ImFontConfig_FontLoaderFlags_setter);
	LUA_FIELD("FontNo", lua_ImFontConfig_FontNo_getter, lua_ImFontConfig_FontNo_setter);
	LUA_FIELD("GlyphExtraAdvanceX", lua_ImFontConfig_GlyphExtraAdvanceX_getter, lua_ImFontConfig_GlyphExtraAdvanceX_setter);
	LUA_FIELD("GlyphMaxAdvanceX", lua_ImFontConfig_GlyphMaxAdvanceX_getter, lua_ImFontConfig_GlyphMaxAdvanceX_setter);
	LUA_FIELD("GlyphMinAdvanceX", lua_ImFontConfig_GlyphMinAdvanceX_getter, lua_ImFontConfig_GlyphMinAdvanceX_setter);
	LUA_FIELD("GlyphOffset", lua_ImFontConfig_GlyphOffset_getter, lua_ImFontConfig_GlyphOffset_setter);
	LUA_FIELD("MergeMode", lua_ImFontConfig_MergeMode_getter, lua_ImFontConfig_MergeMode_setter);
	LUA_FIELD("OversampleH", lua_ImFontConfig_OversampleH_getter, lua_ImFontConfig_OversampleH_setter);
	LUA_FIELD("OversampleV", lua_ImFontConfig_OversampleV_getter, lua_ImFontConfig_OversampleV_setter);
	LUA_FIELD("PixelSnapH", lua_ImFontConfig_PixelSnapH_getter, lua_ImFontConfig_PixelSnapH_setter);
	LUA_FIELD("RasterizerDensity", lua_ImFontConfig_RasterizerDensity_getter, lua_ImFontConfig_RasterizerDensity_setter);
	LUA_FIELD("RasterizerMultiply", lua_ImFontConfig_RasterizerMultiply_getter, lua_ImFontConfig_RasterizerMultiply_setter);
	LUA_FIELD("SizePixels", lua_ImFontConfig_SizePixels_getter, lua_ImFontConfig_SizePixels_setter);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}
int lua_ImFontGlyph_AdvanceX_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.AdvanceX/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->AdvanceX);
	return 1;
}
int lua_ImFontGlyph_AdvanceX_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.AdvanceX/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->AdvanceX, 2);
	return 0;
}
int lua_ImFontGlyph_Codepoint_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Codepoint/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	const auto cval = cobj->Codepoint;
	LUA_PUSH_NATIVE(cval);
	return 1;
}
int lua_ImFontGlyph_Codepoint_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Codepoint/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	auto cval = cobj->Codepoint;
	LUA_NATIVE_SETTER(cval, 2);
	cobj->Codepoint = cval;
	return 0;
}
int lua_ImFontGlyph_Colored_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Colored/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	const auto cval = cobj->Colored;
	LUA_PUSH_NATIVE(cval);
	return 1;
}
int lua_ImFontGlyph_Colored_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Colored/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	auto cval = cobj->Colored;
	LUA_NATIVE_SETTER(cval, 2);
	cobj->Colored = cval;
	return 0;
}
int lua_ImFontGlyph_PackId_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.PackId/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->PackId);
	return 1;
}
int lua_ImFontGlyph_PackId_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.PackId/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->PackId, 2);
	return 0;
}
int lua_ImFontGlyph_SourceIdx_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.SourceIdx/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	const auto cval = cobj->SourceIdx;
	LUA_PUSH_NATIVE(cval);
	return 1;
}
int lua_ImFontGlyph_SourceIdx_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.SourceIdx/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	auto cval = cobj->SourceIdx;
	LUA_NATIVE_SETTER(cval, 2);
	cobj->SourceIdx = cval;
	return 0;
}
int lua_ImFontGlyph_U0_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.U0/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->U0);
	return 1;
}
int lua_ImFontGlyph_U0_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.U0/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->U0, 2);
	return 0;
}
int lua_ImFontGlyph_U1_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.U1/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->U1);
	return 1;
}
int lua_ImFontGlyph_U1_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.U1/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->U1, 2);
	return 0;
}
int lua_ImFontGlyph_V0_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.V0/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->V0);
	return 1;
}
int lua_ImFontGlyph_V0_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.V0/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->V0, 2);
	return 0;
}
int lua_ImFontGlyph_V1_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.V1/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->V1);
	return 1;
}
int lua_ImFontGlyph_V1_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.V1/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->V1, 2);
	return 0;
}
int lua_ImFontGlyph_Visible_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Visible/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	const auto cval = cobj->Visible;
	LUA_PUSH_NATIVE(cval);
	return 1;
}
int lua_ImFontGlyph_Visible_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Visible/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	auto cval = cobj->Visible;
	LUA_NATIVE_SETTER(cval, 2);
	cobj->Visible = cval;
	return 0;
}
int lua_ImFontGlyph_X0_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.X0/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->X0);
	return 1;
}
int lua_ImFontGlyph_X0_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.X0/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->X0, 2);
	return 0;
}
int lua_ImFontGlyph_X1_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.X1/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->X1);
	return 1;
}
int lua_ImFontGlyph_X1_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.X1/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->X1, 2);
	return 0;
}
int lua_ImFontGlyph_Y0_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Y0/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->Y0);
	return 1;
}
int lua_ImFontGlyph_Y0_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Y0/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->Y0, 2);
	return 0;
}
int lua_ImFontGlyph_Y1_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Y1/getter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_PUSH_NATIVE(cobj->Y1);
	return 1;
}
int lua_ImFontGlyph_Y1_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontGlyph.Y1/setter");
	auto cobj = LUA_TO_COBJ(ImFontGlyph*, 1);
	LUA_NATIVE_SETTER(cobj->Y1, 2);
	return 0;
}
int luaReg_imguiFont_imguiImFontGlyph(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImFontGlyph, "ImFontGlyph", "imgui.ImFontGlyph",
		nullptr, nullptr);
	LUA_FIELD("AdvanceX", lua_ImFontGlyph_AdvanceX_getter, lua_ImFontGlyph_AdvanceX_setter);
	LUA_FIELD("Codepoint", lua_ImFontGlyph_Codepoint_getter, lua_ImFontGlyph_Codepoint_setter);
	LUA_FIELD("Colored", lua_ImFontGlyph_Colored_getter, lua_ImFontGlyph_Colored_setter);
	LUA_FIELD("PackId", lua_ImFontGlyph_PackId_getter, lua_ImFontGlyph_PackId_setter);
	LUA_FIELD("SourceIdx", lua_ImFontGlyph_SourceIdx_getter, lua_ImFontGlyph_SourceIdx_setter);
	LUA_FIELD("U0", lua_ImFontGlyph_U0_getter, lua_ImFontGlyph_U0_setter);
	LUA_FIELD("U1", lua_ImFontGlyph_U1_getter, lua_ImFontGlyph_U1_setter);
	LUA_FIELD("V0", lua_ImFontGlyph_V0_getter, lua_ImFontGlyph_V0_setter);
	LUA_FIELD("V1", lua_ImFontGlyph_V1_getter, lua_ImFontGlyph_V1_setter);
	LUA_FIELD("Visible", lua_ImFontGlyph_Visible_getter, lua_ImFontGlyph_Visible_setter);
	LUA_FIELD("X0", lua_ImFontGlyph_X0_getter, lua_ImFontGlyph_X0_setter);
	LUA_FIELD("X1", lua_ImFontGlyph_X1_getter, lua_ImFontGlyph_X1_setter);
	LUA_FIELD("Y0", lua_ImFontGlyph_Y0_getter, lua_ImFontGlyph_Y0_setter);
	LUA_FIELD("Y1", lua_ImFontGlyph_Y1_getter, lua_ImFontGlyph_Y1_setter);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}
int luaReg_imguiFont_imguiImFontAtlasFlags(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_ENUM_DEF("ImFontAtlasFlags");
	LUA_ENUM_ENTRY("None", ImFontAtlasFlags_::ImFontAtlasFlags_None);
	LUA_ENUM_ENTRY("NoPowerOfTwoHeight", ImFontAtlasFlags_::ImFontAtlasFlags_NoPowerOfTwoHeight);
	LUA_ENUM_ENTRY("NoMouseCursors", ImFontAtlasFlags_::ImFontAtlasFlags_NoMouseCursors);
	LUA_ENUM_ENTRY("NoBakedLines", ImFontAtlasFlags_::ImFontAtlasFlags_NoBakedLines);
	LUA_ENUM_ENTRY("NoBakedRoundCorners", ImFontAtlasFlags_::ImFontAtlasFlags_NoBakedRoundCorners);
	LUA_ENUM_END();
	LUA_ENTRY_END(1);
	return 0;
}
int lua_ImFontAtlas_Flags_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.Flags/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->Flags);
	return 1;
}
int lua_ImFontAtlas_Flags_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.Flags/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->Flags, 2);
	return 0;
}
int lua_ImFontAtlas_FontLoaderFlags_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.FontLoaderFlags/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->FontLoaderFlags);
	return 1;
}
int lua_ImFontAtlas_FontLoaderFlags_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.FontLoaderFlags/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->FontLoaderFlags, 2);
	return 0;
}
int lua_ImFontAtlas_FontLoaderName_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.FontLoaderName/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->FontLoaderName);
	return 1;
}
int lua_ImFontAtlas_FontLoaderName_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.FontLoaderName/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->FontLoaderName, 2);
	return 0;
}
int lua_ImFontAtlas_FontNextUniqueID_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.FontNextUniqueID/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->FontNextUniqueID);
	return 1;
}
int lua_ImFontAtlas_FontNextUniqueID_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.FontNextUniqueID/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->FontNextUniqueID, 2);
	return 0;
}
int lua_ImFontAtlas_Locked_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.Locked/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->Locked);
	return 1;
}
int lua_ImFontAtlas_Locked_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.Locked/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->Locked, 2);
	return 0;
}
int lua_ImFontAtlas_RefCount_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.RefCount/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->RefCount);
	return 1;
}
int lua_ImFontAtlas_RefCount_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.RefCount/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->RefCount, 2);
	return 0;
}
int lua_ImFontAtlas_RendererHasTextures_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.RendererHasTextures/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->RendererHasTextures);
	return 1;
}
int lua_ImFontAtlas_RendererHasTextures_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.RendererHasTextures/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->RendererHasTextures, 2);
	return 0;
}
int lua_ImFontAtlas_TexData_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexData/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexData);
	return 1;
}
int lua_ImFontAtlas_TexData_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexData/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexData, 2);
	return 0;
}
int lua_ImFontAtlas_TexDesiredFormat_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexDesiredFormat/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexDesiredFormat);
	return 1;
}
int lua_ImFontAtlas_TexDesiredFormat_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexDesiredFormat/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexDesiredFormat, 2);
	return 0;
}
int lua_ImFontAtlas_TexGlyphPadding_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexGlyphPadding/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexGlyphPadding);
	return 1;
}
int lua_ImFontAtlas_TexGlyphPadding_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexGlyphPadding/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexGlyphPadding, 2);
	return 0;
}
int lua_ImFontAtlas_TexIsBuilt_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexIsBuilt/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexIsBuilt);
	return 1;
}
int lua_ImFontAtlas_TexIsBuilt_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexIsBuilt/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexIsBuilt, 2);
	return 0;
}
int lua_ImFontAtlas_TexMaxHeight_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMaxHeight/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexMaxHeight);
	return 1;
}
int lua_ImFontAtlas_TexMaxHeight_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMaxHeight/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexMaxHeight, 2);
	return 0;
}
int lua_ImFontAtlas_TexMaxWidth_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMaxWidth/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexMaxWidth);
	return 1;
}
int lua_ImFontAtlas_TexMaxWidth_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMaxWidth/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexMaxWidth, 2);
	return 0;
}
int lua_ImFontAtlas_TexMinHeight_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMinHeight/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexMinHeight);
	return 1;
}
int lua_ImFontAtlas_TexMinHeight_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMinHeight/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexMinHeight, 2);
	return 0;
}
int lua_ImFontAtlas_TexMinWidth_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMinWidth/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexMinWidth);
	return 1;
}
int lua_ImFontAtlas_TexMinWidth_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexMinWidth/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexMinWidth, 2);
	return 0;
}
int lua_ImFontAtlas_TexNextUniqueID_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexNextUniqueID/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexNextUniqueID);
	return 1;
}
int lua_ImFontAtlas_TexNextUniqueID_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexNextUniqueID/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexNextUniqueID, 2);
	return 0;
}
int lua_ImFontAtlas_TexPixelsUseColors_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexPixelsUseColors/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexPixelsUseColors);
	return 1;
}
int lua_ImFontAtlas_TexPixelsUseColors_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexPixelsUseColors/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexPixelsUseColors, 2);
	return 0;
}
int lua_ImFontAtlas_TexUvScale_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexUvScale/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexUvScale);
	return 1;
}
int lua_ImFontAtlas_TexUvScale_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexUvScale/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexUvScale, 2);
	return 0;
}
int lua_ImFontAtlas_TexUvWhitePixel_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexUvWhitePixel/getter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_PUSH_NATIVE(cobj->TexUvWhitePixel);
	return 1;
}
int lua_ImFontAtlas_TexUvWhitePixel_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontAtlas.TexUvWhitePixel/setter");
	auto cobj = LUA_TO_COBJ(ImFontAtlas*, 1);
	LUA_NATIVE_SETTER(cobj->TexUvWhitePixel, 2);
	return 0;
}
int lua_ImFontAtlas_addCustomRect(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:addCustomRect");
	LUA_TRY_INVOKE_R(3, &ImFontAtlas::AddCustomRect);
	LUA_TRY_INVOKE_R(2, ([](ImFontAtlas* obj,int arg0,int arg1){{return obj->AddCustomRect(std::move(arg0),std::move(arg1));}}));
	LUA_INVOKE_FOOTER("2,3");
}
int lua_ImFontAtlas_addFont(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:addFont");
	LUA_TRY_INVOKE_R(1, &ImFontAtlas::AddFont);
	LUA_INVOKE_FOOTER("1");
}
int lua_ImFontAtlas_addFontDefault(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:addFontDefault");
	LUA_TRY_INVOKE_R(1, &ImFontAtlas::AddFontDefault);
	LUA_TRY_INVOKE_R(0, [](ImFontAtlas* obj){{return obj->AddFontDefault();}});
	LUA_INVOKE_FOOTER("0,1");
}
int lua_ImFontAtlas_addFontDefaultBitmap(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:addFontDefaultBitmap");
	LUA_TRY_INVOKE_R(1, &ImFontAtlas::AddFontDefaultBitmap);
	LUA_TRY_INVOKE_R(0, [](ImFontAtlas* obj){{return obj->AddFontDefaultBitmap();}});
	LUA_INVOKE_FOOTER("0,1");
}
int lua_ImFontAtlas_addFontDefaultVector(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:addFontDefaultVector");
	LUA_TRY_INVOKE_R(1, &ImFontAtlas::AddFontDefaultVector);
	LUA_TRY_INVOKE_R(0, [](ImFontAtlas* obj){{return obj->AddFontDefaultVector();}});
	LUA_INVOKE_FOOTER("0,1");
}
int lua_ImFontAtlas_clear(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:clear");
	LUA_TRY_INVOKE(0, &ImFontAtlas::Clear);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFontAtlas_clearFonts(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:clearFonts");
	LUA_TRY_INVOKE(0, &ImFontAtlas::ClearFonts);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFontAtlas_clearInputData(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:clearInputData");
	LUA_TRY_INVOKE(0, &ImFontAtlas::ClearInputData);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFontAtlas_clearTexData(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:clearTexData");
	LUA_TRY_INVOKE(0, &ImFontAtlas::ClearTexData);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFontAtlas_compactCache(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:compactCache");
	LUA_TRY_INVOKE(0, &ImFontAtlas::CompactCache);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFontAtlas_getCustomRect(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:getCustomRect");
	LUA_TRY_INVOKE_R(2, &ImFontAtlas::GetCustomRect);
	LUA_INVOKE_FOOTER("2");
}
int lua_ImFontAtlas_removeCustomRect(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:removeCustomRect");
	LUA_TRY_INVOKE(1, &ImFontAtlas::RemoveCustomRect);
	LUA_INVOKE_FOOTER("1");
}
int lua_ImFontAtlas_removeFont(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontAtlas", "imgui.ImFontAtlas:removeFont");
	LUA_TRY_INVOKE(1, &ImFontAtlas::RemoveFont);
	LUA_INVOKE_FOOTER("1");
}
int luaReg_imguiFont_imguiImFontAtlas(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImFontAtlas, "ImFontAtlas", "imgui.ImFontAtlas",
		nullptr, nullptr);
	LUA_FIELD("Flags", lua_ImFontAtlas_Flags_getter, lua_ImFontAtlas_Flags_setter);
	LUA_FIELD("FontLoaderFlags", lua_ImFontAtlas_FontLoaderFlags_getter, lua_ImFontAtlas_FontLoaderFlags_setter);
	LUA_FIELD("FontLoaderName", lua_ImFontAtlas_FontLoaderName_getter, lua_ImFontAtlas_FontLoaderName_setter);
	LUA_FIELD("FontNextUniqueID", lua_ImFontAtlas_FontNextUniqueID_getter, lua_ImFontAtlas_FontNextUniqueID_setter);
	LUA_FIELD("Locked", lua_ImFontAtlas_Locked_getter, lua_ImFontAtlas_Locked_setter);
	LUA_FIELD("RefCount", lua_ImFontAtlas_RefCount_getter, lua_ImFontAtlas_RefCount_setter);
	LUA_FIELD("RendererHasTextures", lua_ImFontAtlas_RendererHasTextures_getter, lua_ImFontAtlas_RendererHasTextures_setter);
	LUA_FIELD("TexData", lua_ImFontAtlas_TexData_getter, lua_ImFontAtlas_TexData_setter);
	LUA_FIELD("TexDesiredFormat", lua_ImFontAtlas_TexDesiredFormat_getter, lua_ImFontAtlas_TexDesiredFormat_setter);
	LUA_FIELD("TexGlyphPadding", lua_ImFontAtlas_TexGlyphPadding_getter, lua_ImFontAtlas_TexGlyphPadding_setter);
	LUA_FIELD("TexIsBuilt", lua_ImFontAtlas_TexIsBuilt_getter, lua_ImFontAtlas_TexIsBuilt_setter);
	LUA_FIELD("TexMaxHeight", lua_ImFontAtlas_TexMaxHeight_getter, lua_ImFontAtlas_TexMaxHeight_setter);
	LUA_FIELD("TexMaxWidth", lua_ImFontAtlas_TexMaxWidth_getter, lua_ImFontAtlas_TexMaxWidth_setter);
	LUA_FIELD("TexMinHeight", lua_ImFontAtlas_TexMinHeight_getter, lua_ImFontAtlas_TexMinHeight_setter);
	LUA_FIELD("TexMinWidth", lua_ImFontAtlas_TexMinWidth_getter, lua_ImFontAtlas_TexMinWidth_setter);
	LUA_FIELD("TexNextUniqueID", lua_ImFontAtlas_TexNextUniqueID_getter, lua_ImFontAtlas_TexNextUniqueID_setter);
	LUA_FIELD("TexPixelsUseColors", lua_ImFontAtlas_TexPixelsUseColors_getter, lua_ImFontAtlas_TexPixelsUseColors_setter);
	LUA_FIELD("TexUvScale", lua_ImFontAtlas_TexUvScale_getter, lua_ImFontAtlas_TexUvScale_setter);
	LUA_FIELD("TexUvWhitePixel", lua_ImFontAtlas_TexUvWhitePixel_getter, lua_ImFontAtlas_TexUvWhitePixel_setter);
	LUA_METHOD("addCustomRect", lua_ImFontAtlas_addCustomRect);
	LUA_METHOD("addFont", lua_ImFontAtlas_addFont);
	LUA_METHOD("addFontDefault", lua_ImFontAtlas_addFontDefault);
	LUA_METHOD("addFontDefaultBitmap", lua_ImFontAtlas_addFontDefaultBitmap);
	LUA_METHOD("addFontDefaultVector", lua_ImFontAtlas_addFontDefaultVector);
	LUA_METHOD("clear", lua_ImFontAtlas_clear);
	LUA_METHOD("clearFonts", lua_ImFontAtlas_clearFonts);
	LUA_METHOD("clearInputData", lua_ImFontAtlas_clearInputData);
	LUA_METHOD("clearTexData", lua_ImFontAtlas_clearTexData);
	LUA_METHOD("compactCache", lua_ImFontAtlas_compactCache);
	LUA_METHOD("getCustomRect", lua_ImFontAtlas_getCustomRect);
	LUA_METHOD("removeCustomRect", lua_ImFontAtlas_removeCustomRect);
	LUA_METHOD("removeFont", lua_ImFontAtlas_removeFont);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}
int lua_ImFontBaked_Ascent_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontBaked.Ascent/getter");
	auto cobj = LUA_TO_COBJ(ImFontBaked*, 1);
	LUA_PUSH_NATIVE(cobj->Ascent);
	return 1;
}
int lua_ImFontBaked_Descent_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontBaked.Descent/getter");
	auto cobj = LUA_TO_COBJ(ImFontBaked*, 1);
	LUA_PUSH_NATIVE(cobj->Descent);
	return 1;
}
int lua_ImFontBaked_FallbackAdvanceX_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontBaked.FallbackAdvanceX/getter");
	auto cobj = LUA_TO_COBJ(ImFontBaked*, 1);
	LUA_PUSH_NATIVE(cobj->FallbackAdvanceX);
	return 1;
}
int lua_ImFontBaked_MetricsTotalSurface_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontBaked.MetricsTotalSurface/getter");
	auto cobj = LUA_TO_COBJ(ImFontBaked*, 1);
	const auto cval = cobj->MetricsTotalSurface;
	LUA_PUSH_NATIVE(cval);
	return 1;
}
int lua_ImFontBaked_OwnerFont_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontBaked.OwnerFont/getter");
	auto cobj = LUA_TO_COBJ(ImFontBaked*, 1);
	LUA_PUSH_NATIVE(cobj->OwnerFont);
	return 1;
}
int lua_ImFontBaked_RasterizerDensity_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontBaked.RasterizerDensity/getter");
	auto cobj = LUA_TO_COBJ(ImFontBaked*, 1);
	LUA_PUSH_NATIVE(cobj->RasterizerDensity);
	return 1;
}
int lua_ImFontBaked_Size_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFontBaked.Size/getter");
	auto cobj = LUA_TO_COBJ(ImFontBaked*, 1);
	LUA_PUSH_NATIVE(cobj->Size);
	return 1;
}
int lua_ImFontBaked_getCharAdvance(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontBaked", "imgui.ImFontBaked:getCharAdvance");
	LUA_TRY_INVOKE_R(1, &ImFontBaked::GetCharAdvance);
	LUA_INVOKE_FOOTER("1");
}
int lua_ImFontBaked_isGlyphLoaded(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFontBaked", "imgui.ImFontBaked:isGlyphLoaded");
	LUA_TRY_INVOKE_R(1, &ImFontBaked::IsGlyphLoaded);
	LUA_INVOKE_FOOTER("1");
}
int luaReg_imguiFont_imguiImFontBaked(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImFontBaked, "ImFontBaked", "imgui.ImFontBaked",
		nullptr, nullptr);
	LUA_FIELD("Ascent", lua_ImFontBaked_Ascent_getter, nullptr);
	LUA_FIELD("Descent", lua_ImFontBaked_Descent_getter, nullptr);
	LUA_FIELD("FallbackAdvanceX", lua_ImFontBaked_FallbackAdvanceX_getter, nullptr);
	LUA_FIELD("MetricsTotalSurface", lua_ImFontBaked_MetricsTotalSurface_getter, nullptr);
	LUA_FIELD("OwnerFont", lua_ImFontBaked_OwnerFont_getter, nullptr);
	LUA_FIELD("RasterizerDensity", lua_ImFontBaked_RasterizerDensity_getter, nullptr);
	LUA_FIELD("Size", lua_ImFontBaked_Size_getter, nullptr);
	LUA_METHOD("getCharAdvance", lua_ImFontBaked_getCharAdvance);
	LUA_METHOD("isGlyphLoaded", lua_ImFontBaked_isGlyphLoaded);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}
int lua_ImFont_CurrentRasterizerDensity_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.CurrentRasterizerDensity/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->CurrentRasterizerDensity);
	return 1;
}
int lua_ImFont_CurrentRasterizerDensity_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.CurrentRasterizerDensity/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->CurrentRasterizerDensity, 2);
	return 0;
}
int lua_ImFont_EllipsisAutoBake_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.EllipsisAutoBake/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->EllipsisAutoBake);
	return 1;
}
int lua_ImFont_EllipsisAutoBake_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.EllipsisAutoBake/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->EllipsisAutoBake, 2);
	return 0;
}
int lua_ImFont_EllipsisChar_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.EllipsisChar/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->EllipsisChar);
	return 1;
}
int lua_ImFont_EllipsisChar_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.EllipsisChar/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->EllipsisChar, 2);
	return 0;
}
int lua_ImFont_FallbackChar_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.FallbackChar/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->FallbackChar);
	return 1;
}
int lua_ImFont_FallbackChar_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.FallbackChar/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->FallbackChar, 2);
	return 0;
}
int lua_ImFont_Flags_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.Flags/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->Flags);
	return 1;
}
int lua_ImFont_Flags_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.Flags/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->Flags, 2);
	return 0;
}
int lua_ImFont_FontId_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.FontId/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->FontId);
	return 1;
}
int lua_ImFont_FontId_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.FontId/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->FontId, 2);
	return 0;
}
int lua_ImFont_LegacySize_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.LegacySize/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->LegacySize);
	return 1;
}
int lua_ImFont_LegacySize_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.LegacySize/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->LegacySize, 2);
	return 0;
}
int lua_ImFont_OwnerAtlas_getter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.OwnerAtlas/getter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_PUSH_NATIVE(cobj->OwnerAtlas);
	return 1;
}
int lua_ImFont_OwnerAtlas_setter(lua_State* lua_S)
{
	LUA_CUR_FNAME("imgui.ImFont.OwnerAtlas/setter");
	auto cobj = LUA_TO_COBJ(ImFont*, 1);
	LUA_NATIVE_SETTER(cobj->OwnerAtlas, 2);
	return 0;
}
int lua_ImFont_addRemapCodepoint(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:addRemapCodepoint");
	LUA_TRY_INVOKE(2, &ImFont::AddRemapCodepoint);
	LUA_INVOKE_FOOTER("2");
}
int lua_ImFont_addRemapCodepointToGlyphIndex(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:addRemapCodepointToGlyphIndex");
	LUA_TRY_INVOKE(3, &ImFont::AddRemapCodepointToGlyphIndex);
	LUA_INVOKE_FOOTER("3");
}
int lua_ImFont_clearOutputData(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:clearOutputData");
	LUA_TRY_INVOKE(0, &ImFont::ClearOutputData);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFont_getDebugName(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:getDebugName");
	LUA_TRY_INVOKE_R(0, &ImFont::GetDebugName);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFont_getFontBaked(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:getFontBaked");
	LUA_TRY_INVOKE_R(2, &ImFont::GetFontBaked);
	LUA_TRY_INVOKE_R(1, ([](ImFont* obj,float arg0){{return obj->GetFontBaked(std::move(arg0));}}));
	LUA_INVOKE_FOOTER("1,2");
}
int lua_ImFont_isGlyphInFont(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:isGlyphInFont");
	LUA_TRY_INVOKE_R(1, &ImFont::IsGlyphInFont);
	LUA_INVOKE_FOOTER("1");
}
int lua_ImFont_isGlyphRangeUnused(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:isGlyphRangeUnused");
	LUA_TRY_INVOKE_R(2, &ImFont::IsGlyphRangeUnused);
	LUA_INVOKE_FOOTER("2");
}
int lua_ImFont_isLoaded(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:isLoaded");
	LUA_TRY_INVOKE_R(0, &ImFont::IsLoaded);
	LUA_INVOKE_FOOTER("0");
}
int lua_ImFont_renderChar(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("imgui.ImFont", "imgui.ImFont:renderChar");
	LUA_TRY_INVOKE(6, &ImFont::RenderChar);
	LUA_TRY_INVOKE(5, ([](ImFont* obj,ImDrawList* arg0,float arg1,const ImVec2& arg2,ImU32 arg3,ImWchar arg4){{return obj->RenderChar(arg0,std::move(arg1),arg2,std::move(arg3),std::move(arg4));}}));
	LUA_INVOKE_FOOTER("5,6");
}
int luaReg_imguiFont_imguiImFont(lua_State* lua_S)
{
	LUA_ENTRY("imgui");
	LUA_CLS_DEF(ImFont, "ImFont", "imgui.ImFont",
		nullptr, nullptr);
	LUA_FIELD("CurrentRasterizerDensity", lua_ImFont_CurrentRasterizerDensity_getter, lua_ImFont_CurrentRasterizerDensity_setter);
	LUA_FIELD("EllipsisAutoBake", lua_ImFont_EllipsisAutoBake_getter, lua_ImFont_EllipsisAutoBake_setter);
	LUA_FIELD("EllipsisChar", lua_ImFont_EllipsisChar_getter, lua_ImFont_EllipsisChar_setter);
	LUA_FIELD("FallbackChar", lua_ImFont_FallbackChar_getter, lua_ImFont_FallbackChar_setter);
	LUA_FIELD("Flags", lua_ImFont_Flags_getter, lua_ImFont_Flags_setter);
	LUA_FIELD("FontId", lua_ImFont_FontId_getter, lua_ImFont_FontId_setter);
	LUA_FIELD("LegacySize", lua_ImFont_LegacySize_getter, lua_ImFont_LegacySize_setter);
	LUA_FIELD("OwnerAtlas", lua_ImFont_OwnerAtlas_getter, lua_ImFont_OwnerAtlas_setter);
	LUA_METHOD("addRemapCodepoint", lua_ImFont_addRemapCodepoint);
	LUA_METHOD("addRemapCodepointToGlyphIndex", lua_ImFont_addRemapCodepointToGlyphIndex);
	LUA_METHOD("clearOutputData", lua_ImFont_clearOutputData);
	LUA_METHOD("getDebugName", lua_ImFont_getDebugName);
	LUA_METHOD("getFontBaked", lua_ImFont_getFontBaked);
	LUA_METHOD("isGlyphInFont", lua_ImFont_isGlyphInFont);
	LUA_METHOD("isGlyphRangeUnused", lua_ImFont_isGlyphRangeUnused);
	LUA_METHOD("isLoaded", lua_ImFont_isLoaded);
	LUA_METHOD("renderChar", lua_ImFont_renderChar);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}
