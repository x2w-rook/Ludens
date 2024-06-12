#include "Core/UI/Include/UI.h"

namespace LD
{

UIWindow::UIWindow() : UIWidget(UIType::Window)
{
}

UIWindow::~UIWindow()
{
    LD_DEBUG_ASSERT(mContext == nullptr);
}

void UIWindow::Startup(const UIWindowInfo& info)
{
    mContext = info.Context;
    mDebugName = info.DebugName;
    mRect = info.Rect;
    mColor = info.Color;

    LD_DEBUG_ASSERT(mContext);
    LD_DEBUG_ASSERT(mRect.w > 0 && mRect.h > 0);

    Attach(info.Parent);

    UIWidgetInfo widgetInfo{};
    widgetInfo.Width = mRect.w;
    widgetInfo.Height = mRect.h;
    widgetInfo.Parent = nullptr;
    UIWidget::Startup(widgetInfo);
    // TODO: Set flex direction row vs column for root node
}

void UIWindow::Cleanup()
{
    UIWidget::Cleanup();

    Detach();
    mContext = nullptr;
}

Vec2 UIWindow::GetRootPos() const
{
    return mRect.Min();
}

void UIWindow::SetRootPos(const Vec2& pos)
{
    mRect.x = pos.x;
    mRect.y = pos.y;
}

Vec2 UIWindow::GetSize() const
{
    return { mRect.w, mRect.h };
}

Rect2D UIWindow::GetRect() const
{
    return mRect;
}

void UIWindow::SetColor(const Vec4& color)
{
    mColor = color;
}

Vec4 UIWindow::GetColor() const
{
    return mColor;
}

void UIWindow::Raise()
{
    LD_DEBUG_ASSERT(mContext);

    mContext->RaiseWindow(this);
}

void UIWindow::InputMouseButtonPressed(MouseButton button, const Vec2& pos)
{
    UIWidget* widget = GetTopWidget(pos);

    if (!widget)
        return;

    widget->OnPress();
}

void UIWindow::InputMouseButtonReleased(MouseButton button, const Vec2& pos)
{
    UIWidget* widget = GetTopWidget(pos);
    if (!widget)
        return;

    widget->OnRelease();
}

void UIWindow::InputKeyPress(KeyCode key)
{
    // TODO:
}

void UIWindow::InputKeyRelease(KeyCode key)
{
    // TODO:
}

void UIWindow::Attach(UIWindow* parent)
{
    mContext->mWindowStack.PushTop(this);

    if (!parent) // context root window
    {
        mParent = mNext = mChild = nullptr;
        return;
    }

    mParent = parent;
    mNext = parent->mChild;
    parent->mChild = this;
}

void UIWindow::Detach()
{
    mContext->mWindowStack.Remove(this);

    if (!mParent) // context root window
        return;

    UIWindow** w;
    for (w = &mParent->mChild; *w && *w != this; w = &((*w)->mNext))
        ;

    LD_DEBUG_ASSERT(*w != nullptr);
    *w = (*w)->mNext;
    mParent = nullptr;
}

UIWidget* UIWindow::GetTopWidget(const Vec2& pos)
{
    for (int i = (int)mWidgetStack.Size() - 1; i >= 0; i--)
    {
        UIWidget* widget = mWidgetStack[i];
        Rect2D rect = widget->GetRect();

        if (rect.Contains(pos))
            return widget;
    }

    return nullptr;
}

} // namespace LD