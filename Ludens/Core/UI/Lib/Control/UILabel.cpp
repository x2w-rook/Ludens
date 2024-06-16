#include "Core/UI/Include/Control/UILabel.h"
#include "Core/UI/Include/UI.h"

namespace LD {

UILabel::UILabel() : UIWidget(UIType::Label), mLimitWidth(0.0f)
{
}

UILabel::~UILabel()
{
    UIWidget::Cleanup();
}

void UILabel::Startup(const UILabelInfo& info)
{
    UIWidget::Startup(info.Widget);

    LD_DEBUG_ASSERT(info.Text.Font);

    mText.Color = info.Text.Color;
    mText.Font = info.Text.Font;
    mText.Size = info.Text.Size;
    SetText(info.Text.Content);

    mLimitWidth = info.Widget.Width;
}

void UILabel::Cleanup()
{
    UIWidget::Cleanup();

    mText.Font = nullptr;
}

UIString UILabel::GetText()
{
    return mText.Content;
}

void UILabel::SetText(const UIString& text)
{
    mText.Content = text;
    mTextGlyphs.Resize(mText.Content.Size());

    Vec2 textSize;
    float scale = GetGlyphScale();

    if (mLimitWidth > 0.0f)
    {
        textSize.x = mLimitWidth;
        mText.Font->DeriveTextSizeLimitWidth(mText.Content, scale, mLimitWidth, textSize.y, mTextGlyphs.Data());
    }
    else
        mText.Font->DeriveTextSize(mText.Content, scale, textSize, mTextGlyphs.Data());

    mLayout.SetSize(textSize);
}

float UILabel::GetTextSize()
{
    return mText.Size;
}

void UILabel::SetTextSize(float size)
{
    mText.Size = size;
}

View<FontGlyphExt> UILabel::GetTextGlyphs()
{
    return mTextGlyphs.GetView();
}

float UILabel::GetGlyphScale()
{
    return mText.GetGlyphScale();
}

UIFont* UILabel::GetFont()
{
    LD_DEBUG_ASSERT(mText.Font);
    return mText.Font;
}

} // namespace LD