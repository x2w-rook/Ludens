#include "Core/UI/Include/UI.h"

namespace LD {

UIContext::~UIContext()
{
    LD_DEBUG_ASSERT(mWindowStack.Size() == 0);
}

void UIContext::Startup(const UIContextInfo& info)
{
    mTheme = new UITheme();
    mHoverWidget = nullptr;

    UIWindowInfo rootInfo;
    rootInfo.Context = this;
    rootInfo.Parent = nullptr;
    rootInfo.Rect = { 0.0f, 0.0f, info.Width, info.Height };
    rootInfo.DebugName = "root";
    mRoot.Startup(rootInfo);
    mRoot.SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
}

void UIContext::Cleanup()
{
    mRoot.Cleanup();

    delete mTheme;
    mTheme = nullptr;
}

UITheme* UIContext::GetTheme()
{
    return mTheme;
}

UIWindow* UIContext::GetRoot()
{
    return &mRoot;
}

Vec2 UIContext::GetSize()
{
    return mRoot.GetWindowSize();
}

void UIContext::BeginFrame(DeltaTime dt)
{
    mIsWithinFrame = true;

    mRoot.CalculateLayout();

    for (int i = 0; i < (int)mWindowStack.Size(); i++)
    {
        UIWindow* window = mWindowStack[i];
        window->CalculateLayout();
    }
}

void UIContext::EndFrame()
{
    mIsWithinFrame = false;
}

void UIContext::RaiseWindow(UIWindow* window)
{
    mWindowStack.Remove(window);
    mWindowStack.PushTop(window);
}

void UIContext::InputMousePosition(Vec2 pos)
{
    mMousePos = pos;

    UIWindow* window = GetTopWindow(pos);

    if (!window)
        return;

    Vec2 windowPos = window->GetWindowPos();
    UIWidget* hover = window->GetTopWidget(mMousePos - windowPos, UIWidget::IS_HOVERABLE_BIT);

    if (hover != mHoverWidget)
    {
        if (hover)
            hover->OnEnter();
        else if (mHoverWidget)
            mHoverWidget->OnLeave();

        mHoverWidget = hover;
    }
}

void UIContext::InputMouseScroll(float& deltaX, float& deltaY)
{
    // TODO: focus widget takes the scroll
}

void UIContext::InputMouseButtonPressed(MouseButton button)
{
    UIWindow* window = GetTopWindow(mMousePos);

    if (!window)
        return;

    Vec2 windowPos = window->GetWindowPos();
    window->InputMouseButtonPressed(button, mMousePos - windowPos);
}

void UIContext::InputMouseButtonReleased(MouseButton button)
{
    UIWindow* window = GetTopWindow(mMousePos);

    if (!window)
        return;

    Vec2 windowPos = window->GetWindowPos();
    window->InputMouseButtonReleased(button, mMousePos - windowPos);
}

void UIContext::InputKeyPress(KeyCode key)
{
}

void UIContext::InputKeyRelease(KeyCode key)
{
}

UIWindow* UIContext::GetTopWindow(const Vec2& pos)
{
    for (int i = (int)mWindowStack.Size() - 1; i >= 0; i--)
    {
        UIWindow* window = mWindowStack[i];
        Rect2D rect = window->GetWindowRect();

        if (rect.Contains(pos))
            return window;
    }

    return nullptr;
}

} // namespace LD