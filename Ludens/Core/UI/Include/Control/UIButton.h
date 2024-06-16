#pragma once

#include "Core/UI/Include/UIWidget.h"
#include "Core/UI/Include/Control/UILabel.h"

namespace LD {

class UIButton;
using UIButtonOnClick = void (*)(UIContext* ctx, UIButton* btn);

enum class UIClickMode
{
    ClickOnPress,
    ClickOnRelease,
};

struct UIButtonInfo
{
    UIWidgetInfo Widget;
    UIText Text;
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

    float GetGlyphScale();
    UIFont* GetFont();
    View<FontGlyphExt> GetTextGlyphs();

    void SetText(const UIString& text);

private:
    static void OnPress(UIContext*, UIWidget*);
    static void OnRelease(UIContext*, UIWidget*);

    Vector<FontGlyphExt> mTextGlyphs;
    UIText mText;
    UIButtonOnClick mOnClick = nullptr;
    UIClickMode mClickMode;
};

} // namespace LD