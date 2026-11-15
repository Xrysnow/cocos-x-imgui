------------------------------
--- Tag: imguiFont
------------------------------


---@class imgui.ImFontConfig
local ImFontConfig = {}
imgui.ImFontConfig = ImFontConfig

---@type ImFont (ImFont*)
ImFontConfig.DstFont = nil

---@type ImWchar (ImWchar)
ImFontConfig.EllipsisChar = nil

---@type number (float)
ImFontConfig.ExtraSizeScale = nil

---@type ImFontFlags (ImFontFlags)
ImFontConfig.Flags = nil

---@type boolean (bool)
ImFontConfig.FontDataOwnedByAtlas = nil

---@type number (int)
ImFontConfig.FontDataSize = nil

---@type number (unsigned int)
ImFontConfig.FontLoaderFlags = nil

---@type ImU32 (ImU32)
ImFontConfig.FontNo = nil

---@type number (float)
ImFontConfig.GlyphExtraAdvanceX = nil

---@type number (float)
ImFontConfig.GlyphMaxAdvanceX = nil

---@type number (float)
ImFontConfig.GlyphMinAdvanceX = nil

---@type ImVec2 (ImVec2)
ImFontConfig.GlyphOffset = nil

---@type boolean (bool)
ImFontConfig.MergeMode = nil

---@type number (ImS8)
ImFontConfig.OversampleH = nil

---@type number (ImS8)
ImFontConfig.OversampleV = nil

---@type boolean (bool)
ImFontConfig.PixelSnapH = nil

---@type number (float)
ImFontConfig.RasterizerDensity = nil

---@type number (float)
ImFontConfig.RasterizerMultiply = nil

---@type number (float)
ImFontConfig.SizePixels = nil


---@class imgui.ImFontGlyph
local ImFontGlyph = {}
imgui.ImFontGlyph = ImFontGlyph

---@type number (float)
ImFontGlyph.AdvanceX = nil

---@type number (unsigned int)
ImFontGlyph.Codepoint = nil

---@type number (unsigned int)
ImFontGlyph.Colored = nil

---@type number (int)
ImFontGlyph.PackId = nil

---@type number (unsigned int)
ImFontGlyph.SourceIdx = nil

---@type number (float)
ImFontGlyph.U0 = nil

---@type number (float)
ImFontGlyph.U1 = nil

---@type number (float)
ImFontGlyph.V0 = nil

---@type number (float)
ImFontGlyph.V1 = nil

---@type number (unsigned int)
ImFontGlyph.Visible = nil

---@type number (float)
ImFontGlyph.X0 = nil

---@type number (float)
ImFontGlyph.X1 = nil

---@type number (float)
ImFontGlyph.Y0 = nil

---@type number (float)
ImFontGlyph.Y1 = nil

local ImFontAtlasFlags = {
	None = 0,
	NoPowerOfTwoHeight = 1,
	NoMouseCursors = 2,
	NoBakedLines = 4,
	NoBakedRoundCorners = 8,
}
---
---@class imgui.ImFontAtlasFlags
imgui.ImFontAtlasFlags = ImFontAtlasFlags


---@class imgui.ImFontAtlas
local ImFontAtlas = {}
imgui.ImFontAtlas = ImFontAtlas

---@type ImFontAtlasFlags (ImFontAtlasFlags)
ImFontAtlas.Flags = nil

---@type number (unsigned int)
ImFontAtlas.FontLoaderFlags = nil

---@type string (const char*)
ImFontAtlas.FontLoaderName = nil

---@type number (int)
ImFontAtlas.FontNextUniqueID = nil

---@type boolean (bool)
ImFontAtlas.Locked = nil

---@type number (int)
ImFontAtlas.RefCount = nil

---@type boolean (bool)
ImFontAtlas.RendererHasTextures = nil

---@type ImTextureData (ImTextureData*)
ImFontAtlas.TexData = nil

---@type ImTextureFormat (ImTextureFormat)
ImFontAtlas.TexDesiredFormat = nil

---@type number (int)
ImFontAtlas.TexGlyphPadding = nil

---@type boolean (bool)
ImFontAtlas.TexIsBuilt = nil

---@type number (int)
ImFontAtlas.TexMaxHeight = nil

---@type number (int)
ImFontAtlas.TexMaxWidth = nil

---@type number (int)
ImFontAtlas.TexMinHeight = nil

---@type number (int)
ImFontAtlas.TexMinWidth = nil

---@type number (int)
ImFontAtlas.TexNextUniqueID = nil

---@type boolean (bool)
ImFontAtlas.TexPixelsUseColors = nil

---@type ImVec2 (ImVec2)
ImFontAtlas.TexUvScale = nil

---@type ImVec2 (ImVec2)
ImFontAtlas.TexUvWhitePixel = nil

---@param font_cfg ImFontConfig @(const ImFontConfig*)
---@return ImFont @(ImFont*)
function ImFontAtlas:addFont(font_cfg)
end

---@param font_cfg ImFontConfig @(const ImFontConfig*)
---@return ImFont @(ImFont*)
function ImFontAtlas:addFontDefault(font_cfg)
end

---@return ImFont @(ImFont*)
function ImFontAtlas:addFontDefault()
end

---@param font_cfg ImFontConfig @(const ImFontConfig*)
---@return ImFont @(ImFont*)
function ImFontAtlas:addFontDefaultBitmap(font_cfg)
end

---@return ImFont @(ImFont*)
function ImFontAtlas:addFontDefaultBitmap()
end

---@param font_cfg ImFontConfig @(const ImFontConfig*)
---@return ImFont @(ImFont*)
function ImFontAtlas:addFontDefaultVector(font_cfg)
end

---@return ImFont @(ImFont*)
function ImFontAtlas:addFontDefaultVector()
end

---@return nil @(void)
function ImFontAtlas:clear()
end

---@return nil @(void)
function ImFontAtlas:clearFonts()
end

---@return nil @(void)
function ImFontAtlas:clearInputData()
end

---@return nil @(void)
function ImFontAtlas:clearTexData()
end

---@return nil @(void)
function ImFontAtlas:compactCache()
end

---@param id number @(ImFontAtlasRectId)
---@return nil @(void)
function ImFontAtlas:removeCustomRect(id)
end

---@param font ImFont @(ImFont*)
---@return nil @(void)
function ImFontAtlas:removeFont(font)
end


--- Borrowed font cache, valid only in the current frame and while its font/atlas remains valid.
--- Reacquire every frame; Lua does not construct, copy or destroy this cache.
---@class imgui.ImFontBaked
local ImFontBaked = {}
imgui.ImFontBaked = ImFontBaked

---@type number (float, readonly)
ImFontBaked.Ascent = nil

---@type number (float, readonly)
ImFontBaked.Descent = nil

---@type number (float, readonly)
ImFontBaked.FallbackAdvanceX = nil

---@type number (unsigned int, readonly)
ImFontBaked.MetricsTotalSurface = nil

---@type ImFont (ImFont*, readonly)
ImFontBaked.OwnerFont = nil

---@type number (float, readonly)
ImFontBaked.RasterizerDensity = nil

---@type number (float, readonly)
ImFontBaked.Size = nil

--- May load missing glyph advance data.
---@param c number @(ImWchar) Unicode code point.
---@return number @(float)
function ImFontBaked:getCharAdvance(c)
end

--- Query the baked cache without loading; false does not mean the font lacks this character.
---@param c number @(ImWchar) Unicode code point.
---@return boolean @(bool)
function ImFontBaked:isGlyphLoaded(c)
end


---@class imgui.ImFont
local ImFont = {}
imgui.ImFont = ImFont

---@type number (float)
ImFont.CurrentRasterizerDensity = nil

---@type boolean (bool)
ImFont.EllipsisAutoBake = nil

---@type ImWchar (ImWchar)
ImFont.EllipsisChar = nil

---@type ImWchar (ImWchar)
ImFont.FallbackChar = nil

---@type ImFontFlags (ImFontFlags)
ImFont.Flags = nil

---@type number (ImGuiID)
ImFont.FontId = nil

---@type number (float)
ImFont.LegacySize = nil

---@type ImFontAtlas (ImFontAtlas*)
ImFont.OwnerAtlas = nil

---@param from_codepoint number @(ImWchar)
---@param to_codepoint number @(ImWchar)
---@return nil @(void)
function ImFont:addRemapCodepoint(from_codepoint, to_codepoint)
end

---@param from_codepoint number @(ImWchar)
---@param font_src_idx number @(int)
---@param glyph_idx number @(int)
---@return nil @(void)
function ImFont:addRemapCodepointToGlyphIndex(from_codepoint, font_src_idx, glyph_idx)
end

---@return nil @(void)
function ImFont:clearOutputData()
end

---@return string @(const char*)
function ImFont:getDebugName()
end

---@param font_size number @(float)
---@param density number @(float) Defaults to -1 when omitted; negative values use the font's current density.
---@return imgui.ImFontBaked|nil @(ImFontBaked*)
function ImFont:getFontBaked(font_size, density)
end

---@param font_size number @(float)
---@return imgui.ImFontBaked|nil @(ImFontBaked*)
function ImFont:getFontBaked(font_size)
end

---@param c number @(ImWchar)
---@return boolean @(bool)
function ImFont:isGlyphInFont(c)
end

---@param c_begin number @(unsigned int)
---@param c_last number @(unsigned int)
---@return boolean @(bool)
function ImFont:isGlyphRangeUnused(c_begin, c_last)
end

---@return boolean @(bool)
function ImFont:isLoaded()
end

---@param draw_list ImDrawList @(ImDrawList*)
---@param size number @(float)
---@param pos ImVec2 @(ImVec2)
---@param col number @(ImU32)
---@param c number @(ImWchar)
---@param cpu_fine_clip ImVec4 @(const ImVec4*)
---@return nil @(void)
function ImFont:renderChar(draw_list, size, pos, col, c, cpu_fine_clip)
end

---@param draw_list ImDrawList @(ImDrawList*)
---@param size number @(float)
---@param pos ImVec2 @(ImVec2)
---@param col number @(ImU32)
---@param c number @(ImWchar)
---@return nil @(void)
function ImFont:renderChar(draw_list, size, pos, col, c)
end

