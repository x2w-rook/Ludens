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
    UIWidget::Startup(info.Widget);

    mLibCallback.OnPress = &UIButton::OnPress;
    mLibCallback.OnRelease = &UIButton::OnRelease;

    mText.Color = info.Text.Color;
    mText.Font = info.Text.Font;
    mText.Size = info.Text.Size;
    SetText(info.Text.Content);

    mOnClick = info.OnClick;
    mClickMode = info.ClickMode;
}

void UIButton::Cleanup()
{
    UIWidget::Cleanup();

    mText.Font = nullptr;
    mOnClick = nullptr;
    mUserCallback.Reset();
    mLibCallback.Reset();
}

float UIButton::GetGlyphScale()
{
    return mText.GetGlyphScale();
}

UIFont* UIButton::GetFont()
{
    return mText.Font;
}

View<FontGlyphExt> UIButton::GetTextGlyphs()
{
    return mTextGlyphs.GetView();
}

void UIButton::SetText(const UIString& text)
{
    mText.Content = text;
    mTextGlyphs.Resize(mText.Content.Size());

    Vec2 textSize;
    float scale = GetGlyphScale();

    mText.Font->DeriveTextSize(mText.Content, scale, textSize, mTextGlyphs.Data());

    mLayout.SetSize(textSize);
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