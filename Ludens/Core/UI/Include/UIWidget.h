#pragma once

#include <string>
#include <yoga/Yoga.h>
#include "Core/Math/Include/Rect2D.h"
#include "Core/Math/Include/Vec4.h"
#include "Core/DSA/Include/Vector.h"
#include "Core/Media/Include/Font.h"
#include "Core/OS/Include/Memory.h"
#include "Core/OS/Include/UID.h"
#include "Core/UI/Include/UILayout.h"

namespace LD {

class UIWidget;
class UIWindow;
class UIContext;
class UIFont;
using UIWidgetFn = void (*)(UIContext*, UIWidget*);

enum class UIType
{
    Window = 0,
    Panel,
    Label,
    Button,
    Texture,
};

using UIWidgetOnEnter = void (*)(UIContext*, UIWidget*);
using UIWidgetOnLeave = void (*)(UIContext*, UIWidget*);
using UIWidgetOnPress = void (*)(UIContext*, UIWidget*);
using UIWidgetOnRelease = void (*)(UIContext*, UIWidget*);

struct UIWidgetCallback
{
    UIWidgetOnEnter OnEnter = nullptr;
    UIWidgetOnLeave OnLeave = nullptr;
    UIWidgetOnPress OnPress = nullptr;
    UIWidgetOnRelease OnRelease = nullptr;

    void Reset()
    {
        OnEnter = nullptr;
        OnLeave = nullptr;
        OnPress = nullptr;
        OnRelease = nullptr;
    }
};

struct UIWidgetInfo
{
    UIWidget* Parent = nullptr;
    UIWidgetCallback Callback;
    UIFlexDirection FlexDirection = UIFlexDirection::Column;
    float FlexGrow = 1.0f;
    float Width = 0.0f;
    float Height = 0.0f;
};

class UIWidget
{
public:
    UIWidget() = delete;
    UIWidget(UIType type);
    UIWidget(const UIWidget&) = delete;
    ~UIWidget();

    UIWidget& operator=(const UIWidget&) = delete;

    void Startup(const UIWidgetInfo& info);
    void Cleanup();

    void CalculateLayout();

    /// get widget type 
    UIType GetType() const;

    /// get position relative to parent window
    Vec2 GetPos() const;

    /// get position and size of this widget, position is relative to parent window
    Rect2D GetRect() const;

    /// get the window that the widget lives in
    inline UIWindow* GetWindow() const
    {
        LD_DEBUG_ASSERT(mWindow);
        return mWindow;
    }

    /// get first child widget
    inline UIWidget* GetChildren() const
    {
        return mChild;
    }

    /// get next sibling widget
    inline UIWidget* GetNext() const
    {
        return mNext;
    }

    /// set padding for all directions
    void SetPadding(float padding);

    /// set padding for one direction
    void SetPadding(float padding, UIEdge edge);

    /// set left and right padding
    inline void SetHPadding(float padding)
    {
        SetPadding(padding, UIEdge::Left);
        SetPadding(padding, UIEdge::Right);
    }

    /// set top and bottom padding
    inline void SetVPadding(float padding)
    {
        SetPadding(padding, UIEdge::Top);
        SetPadding(padding, UIEdge::Bottom);
    }

    /// set margin for all directions
    void SetMargin(float margin);

    /// set padding for one direction
    void SetMargin(float margin, UIEdge edge);

    /// set left and right padding
    inline void SetHMargin(float margin)
    {
        SetMargin(margin, UIEdge::Left);
        SetMargin(margin, UIEdge::Right);
    }

    /// set top and bottom margin
    inline void SetVMargin(float margin)
    {
        SetMargin(margin, UIEdge::Top);
        SetMargin(margin, UIEdge::Bottom);
    }

    void OnEnter();
    void OnLeave();
    void OnPress();
    void OnRelease();

protected:
    UIType mType;
    UILayoutNode mLayout;
    UIWidgetCallback mUserCallback;
    UIWidgetCallback mLibCallback;
    UIWindow* mWindow; // the window this widget lives in
    UIWidget* mParent; // parent widget
    UIWidget* mChild;  // first child widget
    UIWidget* mNext;   // next sibling widget

private:
    void CleanupRecursive(UIWidget* widget);
    void Attach(UIWidget* parent);
    void Detach();

    bool mIsAlive;
};

} // namespace LD
