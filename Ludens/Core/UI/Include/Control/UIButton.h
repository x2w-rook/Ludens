#pragma once

#include "Core/UI/Include/UIWidget.h"

namespace LD {

class UIButton;
using UIButtonOnClick = void (*)(UIContext* ctx, UIButton* btn);

enum class UIClickMode
{
    ClickOnPress,
    ClickOnRelease,
};

struct UIButtonInfo : UIWidgetInfo
{
    Vec4 Color;
    UIButtonOnClick OnClick = nullptr;
    UIClickMode ClickMode = UIClickMode::ClickOnRelease;
};

class UIButton : public UIWidget
{
public:
    UIButton();
    UIButton(const UIButton&) = delete;
    ~UIButton();

    UIButton& operator=(const UIButton&) = delete;

    void Startup(const UIButtonInfo& info);
    void Cleanup();

private:
    static void OnPress(UIContext*, UIWidget*);
    static void OnRelease(UIContext*, UIWidget*);

    UIButtonOnClick mOnClick = nullptr;
    UIClickMode mClickMode;
};

} // namespace LD