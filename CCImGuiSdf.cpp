#include "CCImGuiSdf.h"
#include "imgui_internal.h"
#include "imgui_impl_cc.h"
#include "platform/FileUtils.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>

namespace ImGuiCC
{
namespace
{
constexpr int imguiSdfPpem = 48;
constexpr int gridWidth = 256;
constexpr int maxCells = gridWidth * gridWidth;

struct Cell
{
    std::shared_ptr<cc::SdfFontSource> font;
    uint32_t glyph;
    cc::SdfGlyphBounds bounds;
};
struct Atlas
{
    std::vector<Cell> cells;
    std::vector<int> free;
    std::vector<int> retired;
    int sources{};
};
std::map<ImFontAtlas*, Atlas> atlases;

struct Source
{
    std::shared_ptr<cc::SdfFontSource> font;
    std::map<int, int> cells;
};
Source& source(ImFontConfig* cfg) { return *static_cast<Source*>(cfg->FontLoaderData); }

bool initSource(ImFontAtlas* atlas, ImFontConfig* cfg)
{
    auto font = cc::SdfFontSource::open({static_cast<const uint8_t*>(cfg->FontData), size_t(cfg->FontDataSize)},
                                        imguiSdfPpem);
    if (!font)
        return false;
    cfg->FontLoaderData = new Source{std::move(font), {}};
    ++atlases[atlas].sources;
    if (cfg->MergeMode && cfg->SizePixels == 0)
        cfg->SizePixels = cfg->DstFont->Sources[0]->SizePixels;
    return true;
}
void destroySource(ImFontAtlas* atlas, ImFontConfig* cfg)
{
    if (!cfg->FontLoaderData)
        return;
    auto& registry = atlases.at(atlas);
    for (const auto& [glyph, cell] : source(cfg).cells)
        registry.retired.push_back(cell);
    --registry.sources;
    delete &source(cfg);
    cfg->FontLoaderData = nullptr;
}
bool initBake(ImFontAtlas*, ImFontConfig* cfg, ImFontBaked* baked, void*)
{
    baked->LoadNoRenderOnLayout = true;
    if (!cfg->MergeMode)
    {
        const auto& font = *source(cfg).font;
        const float scale = baked->Size / (font.ascentUnits() - font.descentUnits());
        baked->Ascent = std::ceil(font.ascentUnits() * scale);
        baked->Descent = std::floor(font.descentUnits() * scale);
    }
    return true;
}
int registerCell(Atlas& atlas, Source& src, int glyph)
{
    if (auto it = src.cells.find(glyph); it != src.cells.end())
        return it->second;
    if (atlas.free.empty() && atlas.cells.size() == maxCells)
        return -1;
    cc::SdfGlyphBounds bounds;
    if (!src.font->bounds(glyph, bounds))
        return -1;
    const int id = atlas.free.empty() ? int(atlas.cells.size()) : atlas.free.back();
    if (atlas.free.empty())
        atlas.cells.emplace_back();
    else
        atlas.free.pop_back();
    atlas.cells[id] = {src.font, uint32_t(glyph), bounds};
    src.cells.emplace(glyph, id);
    return id;
}
ImVec2 cellOrigin(int id)
{
    return ImVec2(float(-2 * (id % gridWidth + 1)), float(-2 * (id / gridWidth + 1)));
}
bool loadGlyph(ImFontAtlas* atlas, ImFontConfig* cfg, ImFontBaked* baked, void*, int glyph,
               ImFontGlyph* out, float* onlyAdvance)
{
    auto& src = source(cfg);
    auto& font = *src.font;
    const float ref = baked->OwnerFont->Sources[0]->SizePixels;
    const float mergeScale = cfg->MergeMode && ref > 0 ? cfg->SizePixels / ref : 1.f;
    const float scale = baked->Size * mergeScale * cfg->ExtraSizeScale / (font.ascentUnits() - font.descentUnits());
    float advance;
    if (!font.advance(glyph, advance))
        return false;
    if (onlyAdvance)
    {
        *onlyAdvance = advance * scale;
        return true;
    }
    out->AdvanceX = advance * scale;
    const float em = font.unitsPerEm() * scale;
    float factor = em / font.rasterPpem();
    cc::SdfGlyphBounds box{};
    auto& registry = atlases.at(atlas);
    // ImGui synthesizes an ellipsis by copying the packed pixels of the dot.
    const bool packedDot = baked->OwnerFont->EllipsisAutoBake &&
        (out->Codepoint == '.' || out->Codepoint == 0xff0e);
    const int id = !packedDot && ImGui_ImplCC_EnsureSdfProgram() ? registerCell(registry, src, glyph) : -1;
    if (id >= 0)
    {
        box = registry.cells[id].bounds;
        auto uv = cellOrigin(id);
        out->U0 = uv.x;
        out->V0 = uv.y;
        out->U1 = uv.x + 1;
        out->V1 = uv.y + 1;
    }
    else
    {
        const double ppem = std::round(double(em) * cfg->RasterizerDensity * baked->RasterizerDensity * 64);
        if (!(ppem >= 1 && ppem <= std::numeric_limits<int>::max()))
            return false;
        std::vector<uint8_t> pixels;
        if (!font.bitmap(glyph, int(ppem), box, pixels))
            return false;
        factor = em * 64.f / float(ppem);
        if (box.width && box.height)
        {
            out->PackId = ImFontAtlasPackAddRect(atlas, box.width, box.height);
            if (out->PackId == ImFontAtlasRectId_Invalid)
                return false;
            auto* r = ImFontAtlasPackGetRect(atlas, out->PackId);
            ImFontAtlasBakedSetFontGlyphBitmap(atlas, baked, cfg, out, r, pixels.data(), ImTextureFormat_Alpha8, box.width);
        }
    }
    const float offsets = ref != 0.f ? baked->Size / ref : 1.f;
    out->X0 = std::floor(cfg->GlyphOffset.x * offsets + 0.5f) + box.left * factor;
    out->Y0 = IM_ROUND(baked->Ascent) + std::floor(cfg->GlyphOffset.y * offsets + 0.5f) - box.top * factor;
    out->X1 = out->X0 + box.width * factor;
    out->Y1 = out->Y0 + box.height * factor;
    out->Visible = box.width && box.height;
    return true;
}

const ImFontLoader& loader()
{
    static const ImFontLoader value = [] {
        ImFontLoader table;
        table.Name = "cocos SDF";
        table.FontSrcInit = initSource;
        table.FontSrcDestroy = destroySource;
        table.FontSrcGetGlyphIndexFromCodepoint = [](ImFontAtlas*, ImFontConfig* cfg, ImWchar c) {
            return int(source(cfg).font->glyphIndex(c));
        };
        table.FontSrcContainsGlyph = [](ImFontAtlas*, ImFontConfig* cfg, int id) {
            return id > 0 && uint32_t(id) < source(cfg).font->glyphCount();
        };
        table.FontBakedInit = initBake;
        table.FontBakedLoadGlyph = loadGlyph;
        return table;
    }();
    return value;
}

int coordinate(float uv)
{
    if (!(uv >= -2 * gridWidth && uv <= -1))
        return -1;
    const int n = int(std::ceil(-uv * 0.5f)) - 1;
    return uv <= -2 * (n + 1) + 1 ? n : -1;
}
int cellIndex(const ImVec2& uv)
{
    const int x = coordinate(uv.x), y = coordinate(uv.y);
    return x < 0 || y < 0 ? -1 : y * gridWidth + x;
}
struct TriangleSource
{
    const ImDrawList* list;
    const ImDrawCmd* command;
    std::vector<const Cell*> cells;
};
} // namespace

ImFont* AddSdfFont(ImFontAtlas* atlas, const char* path, float size, const ImFontConfig& config)
{
    if (!atlas || !path || !*path || atlas->Locked || config.FontNo || config.RasterizerMultiply != 1)
        return nullptr;
    ImFontConfig cfg = config;
    cfg.SizePixels = size > 0 ? size : cfg.SizePixels;
    cfg.FontLoader = &loader();
    auto bytes = cc::FileUtils::getInstance()->getDataFromFile(path);
    if (bytes.getSize() <= 100 || bytes.getSize() > std::numeric_limits<int>::max())
        return nullptr;
    auto* data = ImGui::MemAlloc(bytes.getSize());
    if (!data)
        return nullptr;
    std::memcpy(data, bytes.getBytes(), bytes.getSize());
    cfg.FontDataOwnedByAtlas = true;
    return atlas->AddFontFromMemoryTTF(data, int(bytes.getSize()), cfg.SizePixels, &cfg);
}

void CollectSdfSources()
{
    for (auto it = atlases.begin(); it != atlases.end();)
    {
        auto& atlas = it->second;
        if (!atlas.sources)
        {
            it = atlases.erase(it);
            continue;
        }
        for (int i : atlas.retired)
        {
            atlas.cells[i] = {};
            atlas.free.push_back(i);
        }
        atlas.retired.clear();
        ++it;
    }
}

SdfDrawData PrepareSdfDrawData(const ImDrawData& data)
{
    SdfDrawData output;
    if (atlases.empty())
        return output;
    std::map<const ImTextureData*, const Atlas*> textures;
    for (auto* atlas : ImGui::GetCurrentContext()->FontAtlases)
        if (auto it = atlases.find(atlas); it != atlases.end())
            for (auto* texture : atlas->TexList)
                textures.emplace(texture, &it->second);

    std::vector<TriangleSource> mixed;
    std::map<const Cell*, size_t> requested;
    std::vector<cc::SdfFontSource::Request> ids;
    const ImVec2 clipMax(data.DisplayPos.x + data.DisplaySize.x, data.DisplayPos.y + data.DisplaySize.y);
    for (auto* list : data.CmdLists)
        for (const auto& cmd : list->CmdBuffer)
        {
            auto tex = textures.find(cmd.TexRef._TexData);
            if (cmd.UserCallback || tex == textures.end() || cmd.ElemCount == 0 ||
                cmd.ClipRect.x >= std::min(clipMax.x, cmd.ClipRect.z) ||
                cmd.ClipRect.y >= std::min(clipMax.y, cmd.ClipRect.w) ||
                cmd.ClipRect.z <= data.DisplayPos.x || cmd.ClipRect.w <= data.DisplayPos.y)
                continue;
            const auto& entries = tex->second->cells;
            TriangleSource triangles{list, &cmd, {}};
            const auto* indices = list->IdxBuffer.Data + cmd.IdxOffset;
            const auto* vertices = list->VtxBuffer.Data + cmd.VtxOffset;
            for (unsigned i = 0; i + 2 < cmd.ElemCount; i += 3)
            {
                const int id = cellIndex(vertices[indices[i]].uv);
                if (id < 0 || size_t(id) >= entries.size() || !entries[id].font ||
                    cellIndex(vertices[indices[i + 1]].uv) != id || cellIndex(vertices[indices[i + 2]].uv) != id)
                    continue;
                if (triangles.cells.empty())
                    triangles.cells.resize(cmd.ElemCount / 3);
                const auto& cell = entries[id];
                triangles.cells[i / 3] = &cell;
                const auto& a = vertices[indices[i]].pos;
                const auto& b = vertices[indices[i + 1]].pos;
                const auto& c = vertices[indices[i + 2]].pos;
                if (std::max({a.x, b.x, c.x}) <= std::max(cmd.ClipRect.x, data.DisplayPos.x) ||
                    std::min({a.x, b.x, c.x}) >= std::min(cmd.ClipRect.z, clipMax.x) ||
                    std::max({a.y, b.y, c.y}) <= std::max(cmd.ClipRect.y, data.DisplayPos.y) ||
                    std::min({a.y, b.y, c.y}) >= std::min(cmd.ClipRect.w, clipMax.y) ||
                    !cell.bounds.width || !cell.bounds.height)
                    continue;
                if (requested.try_emplace(&cell, ids.size()).second)
                    ids.push_back({cell.font.get(), cell.glyph});
            }
            if (!triangles.cells.empty())
                mixed.push_back(std::move(triangles));
        }

    std::vector<cc::SdfGlyphView> views(ids.size());
    if (!ids.empty() && ImGui_ImplCC_EnsureSdfProgram())
        cc::SdfFontSource::prepare(ids, views);
    for (auto& draw : mixed)
    {
        auto& runs = output.runs[draw.command];
        const auto* indices = draw.list->IdxBuffer.Data + draw.command->IdxOffset;
        const auto* vertices = draw.list->VtxBuffer.Data + draw.command->VtxOffset;
        for (size_t i = 0; i < draw.cells.size(); ++i)
        {
            const auto* cell = draw.cells[i];
            const auto it = cell ? requested.find(cell) : requested.end();
            if (cell && it == requested.end())
                continue;
            const auto* view = cell ? &views[it->second] : nullptr;
            const auto mode = !view ? DrawMode::Atlas : view->texture ? DrawMode::Sdf : DrawMode::Missing;
            const auto page = view ? view->texture : nullptr;
            if (runs.empty() || runs.back().mode != mode || runs.back().page != page)
                runs.push_back({mode, page, output.vertices.size(), 0, {}});
            auto& run = runs.back();
            if (view && view->lease && (run.leases.empty() || run.leases.back() != view->lease))
                run.leases.push_back(view->lease);
            for (int j = 0; j < 3; ++j)
            {
                auto v = vertices[indices[3 * i + j]];
                if (view)
                {
                    const auto origin = cellOrigin(cellIndex(v.uv));
                    const float s = v.uv.x - origin.x, t = v.uv.y - origin.y;
                    v.uv = page ? ImVec2(view->u0 + s * (view->u1 - view->u0), view->v0 + t * (view->v1 - view->v0))
                                : ImVec2(s, t);
                }
                output.vertices.push_back(v);
            }
            run.count += 3;
        }
    }
    return output;
}
} // namespace ImGuiCC
