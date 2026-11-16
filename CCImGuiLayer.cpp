#include "CCImGuiLayer.h"
#include "imgui/imgui.h"
#include "imgui_impl_cc.h"
#include "CCIMGUI.h"

using namespace cocos2d;

cc::IntrusivePtr<ImGuiLayer> ImGuiLayer::create()
{
	cc::IntrusivePtr pRet{new(std::nothrow) ImGuiLayer()};
	if (pRet && pRet->init())
	{
		return pRet;
	}
	return nullptr;
}

bool ImGuiLayer::init()
{
	if (!Layer::init() || !CCIMGUI::getInstance())
		return false;
#ifdef CC_PLATFORM_PC
	// note: when at the first click to focus the window, this will not take effect
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* touch, Event*) -> bool {
		if (!_visible)
			return false;
		return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif
	// add an empty sprite to avoid render problem
	const auto sp = Sprite::create();
	sp->setGlobalZOrder(1);
	sp->setOpacity(0);
	addChild(sp, 1);
	return true;
}

void ImGuiLayer::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
	if (!_visible)
		return;
	onDraw();
	Layer::visit(renderer, parentTransform, parentFlags);
}

void ImGuiLayer::onDraw()
{
    // create frame
    ImGui_ImplCC_NewFrame();
    // draw all gui
    CCIMGUI::getInstance()->update();
    // render
    ImGui::Render();
    ImGui_ImplCC_RenderDrawData(ImGui::GetDrawData());
	ImGui_ImplCC_RenderPlatform();
}
