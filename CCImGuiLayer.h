#pragma once
#include "cocos2d.h"

class ImGuiLayer : public cc::Layer
{
public:
    static cc::IntrusivePtr<ImGuiLayer> create();
protected:
    virtual bool init() override;
    virtual void visit(cc::Renderer *renderer, const cc::Mat4& parentTransform, uint32_t parentFlags) override;
    void onDraw();
};
