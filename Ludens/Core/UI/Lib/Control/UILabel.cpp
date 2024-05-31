#include "Core/UI/Include/Control/UILabel.h"
#include "Core/UI/Include/UI.h"

namespace LD {

UILabel::UILabel() : UIWidget(UIType::Label), mFont(nullptr)
{
}

UILabel::~UILabel()
{
    UIWidget::Cleanup();
}

void UILabel::Startup(const UILabelInfo& info)
{
    UIWidget::Startup(info);

    LD_DEBUG_ASSERT(info.Font);

    mLimitWidth = info.Width;
    mFont = info.Font;
    mTextSize = info.TextSize;
    mTextColor = info.TextColor;
    SetText(info.Text);
}

void UILabel::Cleanup()
{
    UIWidget::Cleanup();

    mFont = nullptr;
}

std::string UILabel::GetText()
{
    return mText;
}

void UILabel::SetText(const std::string& text)
{
    mText = text;
    mTextGlyphs.Resize(mText.size());

    Vec2 textSize;
    float scale = mTextSize / mFont->GetTTF()->GetPixelSize();

    if (mLimitWidth > 0.0f)
    {
        textSize.x = mLimitWidth;
        mFont->DeriveTextSizeLimitWidth(mText, scale, mLimitWidth, textSize.y, mTextGlyphs.Data());
    }
    else
        mFont->DeriveTextSize(mText, scale, textSize, mTextGlyphs.Data());

    mLayout.SetSize(textSize);
}

float UILabel::GetTextSize()
{
    return mTextSize;
}

void UILabel::SetTextSize(float size)
{
    mTextSize = size;
}

Vec4 UILabel::GetTextColor()
{
    return mTextColor;
}

void UILabel::SetTextColor(const Vec4& color)
{
    mTextColor = color;
}

View<FontGlyphExt> UILabel::GetTextGlyphs()
{
    return mTextGlyphs.GetView();
}

float UILabel::GetGlyphScale()
{
    LD_DEBUG_ASSERT(mFont);
    return mTextSize / mFont->GetTTF()->GetPixelSize();
}

UIFont* UILabel::GetFont()
{
    LD_DEBUG_ASSERT(mFont);
    return mFont;
}

} // namespace LD