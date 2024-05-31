#include "Core/UI/Include/Control/UIButton.h"

namespace LD {

UIButton::UIButton() : UIWidget(UIType::Button)
{
}

UIButton::~UIButton()
{
    UIWidget::Cleanup();
}

void UIButton::Startup(const UIButtonInfo& info)
{
    UIWidget::Startup(info);

    mLibCallback.OnPress = &UIButton::OnPress;
    mLibCallback.OnRelease = &UIButton::OnRelease;
    mOnClick = info.OnClick;
    mClickMode = info.ClickMode;
}

void UIButton::Cleanup()
{
    UIWidget::Cleanup();

    mOnClick = nullptr;
    mUserCallback.Reset();
    mLibCallback.Reset();
}

void UIButton::OnPress(UIContext* ctx, UIWidget* widget)
{
    UIButton* button = static_cast<UIButton*>(widget);

    if (button->mClickMode == UIClickMode::ClickOnPress && button->mOnClick)
    {
        button->mOnClick(ctx, button);
    }
}

void UIButton::OnRelease(UIContext* ctx, UIWidget* widget)
{
    UIButton* button = static_cast<UIButton*>(widget);

    if (button->mClickMode == UIClickMode::ClickOnRelease && button->mOnClick)
    {
        button->mOnClick(ctx, button);
    }
}

} // namespace LD