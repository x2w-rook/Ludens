#pragma once

#include "Core/UI/Include/UIWidget.h"

namespace LD {

struct UILabelInfo : UIWidgetInfo
{
    UIFont* Font;
    std::string Text;
    float TextSize = 16.0f;
    Vec4 TextColor;
};

class UILabel : public UIWidget
{
public:
    UILabel();
    UILabel(const UILabel&) = delete;
    ~UILabel();

    UILabel& operator=(const UILabel&) = delete;

    void Startup(const UILabelInfo& info);
    void Cleanup();

    /// get the text content
    std::string GetText();

    /// set the text content
    void SetText(const std::string& text);

    /// get the text size
    float GetTextSize();

    /// set the text size
    void SetTextSize(float size);

    /// get the text color
    Vec4 GetTextColor();

    /// set the text color
    void SetTextColor(const Vec4& color);

    /// get a view to the glyphs for each character in text,
    /// the view is invalidated between calls to SetText().
    View<FontGlyphExt> GetTextGlyphs();

    /// get the ratio of displayed pixel size to the actual glyph size
    float GetGlyphScale();

    UIFont* GetFont();

private:
    Vector<FontGlyphExt> mTextGlyphs;
    UIFont* mFont;
    Vec4 mTextColor;
    float mTextSize;
    std::string mText;
    float mLimitWidth;
};

} // namespace LD