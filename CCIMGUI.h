#pragma once
#include "cocos2d.h"
#include "imgui/imgui.h"
#include "implot/implot.h"
#include <functional>
#include <string>
#include <tuple>

class CCIMGUI
{
	void init();
public:
	static CCIMGUI* getInstance();
	static void destroyInstance();
	static void setOnInit(const std::function<void(CCIMGUI*)>& callBack);
	void update();
	void addCallback(const std::function<void()>& callBack, const std::string& name);
	void removeCallback(const std::string& name);

    // imgui helper

    void image(
		cc::Texture2D* tex,
		const ImVec2& size,
		const ImVec2& uv0 = ImVec2(0, 0),
		const ImVec2& uv1 = ImVec2(1, 1),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
		const ImVec4& border_col = ImVec4(0, 0, 0, 0));
    void image(
		cc::Sprite* sprite,
		const ImVec2& size,
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
		const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	void imageWithBg(
		cc::Texture2D* tex,
		const ImVec2& size,
		const ImVec2& uv0 = ImVec2(0, 0),
		const ImVec2& uv1 = ImVec2(1, 1),
		const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	void imageWithBg(
		cc::Sprite* sprite,
		const ImVec2& size,
		const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool imageButton(
		const char* str_id,
		cc::Texture2D* tex,
		const ImVec2& size,
		const ImVec2& uv0 = ImVec2(0, 0),
		const ImVec2& uv1 = ImVec2(1, 1),
		const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool imageButton(
		const char* str_id,
		cc::Sprite* sprite,
		const ImVec2& size,
		const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

	void node(
		cc::Node* node,
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
		const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	bool nodeButton(
		const char* str_id,
		cc::Node* node,
		const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

	std::tuple<ImTextureID, int> useTexture(cc::Texture2D* texture);
	std::tuple<ImTextureID, ImVec2, ImVec2, int> useSprite(cc::Sprite* sprite);
	std::tuple<ImTextureID, ImVec2, ImVec2, int> useNode(cc::Node* node, const ImVec2& pos);

	static void setNodeColor(cc::Node* node, const ImVec4& col);
	static void setNodeColor(cc::Node* node, ImGuiCol col);
	static void setLabelColor(cc::Label* label, const ImVec4& col);
	static void setLabelColor(cc::Label* label, bool disabled = false);
	static void setLabelColor(cc::Label* label, ImGuiCol col);

	int getCCRefId(cc::Ref* p);

	// implot

	void plotImage(
		const char* label_id,
		cc::Texture2D* tex,
		const ImPlotPoint& bounds_min,
		const ImPlotPoint& bounds_max,
		const ImVec2& uv0 = ImVec2(0, 0),
		const ImVec2& uv1 = ImVec2(1, 1),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
		const ImPlotSpec& spec = ImPlotSpec());
	void plotImage(
		const char* label_id,
		cc::Sprite* sprite,
		const ImPlotPoint& bounds_min,
		const ImPlotPoint& bounds_max,
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
		const ImPlotSpec& spec = ImPlotSpec());

	// markdown

	using MdLinkCallback = std::function<void(const std::string&, const std::string&, bool)>;
	using MdTooltipCallback = std::function<void(const std::string&, const std::string&, bool, const std::string&)>;
	using MdImageCallback = std::function<std::tuple<cc::Sprite*, ImVec2, ImVec4, ImVec4>(const std::string&, const std::string&)>;
	using MdFormatCallback = std::function<void(int, int32_t, bool, bool)>;

	void setMarkdownLinkCallback(const MdLinkCallback& f);
	void setMarkdownTooltipCallback(const MdTooltipCallback& f);
	void setMarkdownImageCallback(const MdImageCallback& f);
	void setMarkdownFormatCallback(const MdFormatCallback& f);
	void setMarkdownFont(int index, ImFont* font, bool seperator);
	void setMarkdownLinkIcon(const std::string& icon);
	void markdown(const std::string& content);

private:
	static std::function<void(CCIMGUI*)> _onInit;
	ImPlotContext* _implotContext = nullptr;
	
    std::unordered_map<std::string, std::function<void()>> _callPiplines;
    std::unordered_map<cc::Ref*, int> usedCCRefIdMap;
	// cocos objects should be retained until next frame
    cc::Vector<cc::Ref*> usedCCRef;
};
