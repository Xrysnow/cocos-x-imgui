#pragma once

#include "imgui.h"
#include "2d/font/SdfFontSource.h"
#include <map>
#include <vector>

namespace ImGuiCC
{
ImFont* AddSdfFont(ImFontAtlas* atlas, const char* path, float size, const ImFontConfig& config);
void CollectSdfSources();

enum class DrawMode { Atlas, Sdf, Missing };
struct DrawRun
{
    DrawMode mode;
    cc::Texture2D* page;
    size_t first, count;
    std::vector<std::shared_ptr<void>> leases;
};
struct SdfDrawData
{
    std::vector<ImDrawVert> vertices;
    std::map<const ImDrawCmd*, std::vector<DrawRun>> runs;
};
SdfDrawData PrepareSdfDrawData(const ImDrawData& data);
}
