#pragma once

#include "Core/UI/Include/UIWidget.h"

namespace LD {

struct UIPanelInfo : UIWidgetInfo
{
    Vec4 Color;
};

class UIPanel : public UIWidget
{
public:
    UIPanel();
    UIPanel(const UIPanel&) = delete;
    ~UIPanel();

    UIPanel& operator=(const UIPanel&) = delete;

    void Startup(const UIPanelInfo& info);
    void Cleanup();

private:
    Vec4 mColor;
};

} // namespace LD