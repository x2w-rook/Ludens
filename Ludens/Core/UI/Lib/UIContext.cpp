#include "Core/UI/Include/UI.h"

namespace LD {

UIContext::~UIContext()
{
    LD_DEBUG_ASSERT(mWindowStack.Size() == 0);
}

void UIContext::Startup(const UIContextInfo& info)
{
    UIWindowInfo rootInfo;
    rootInfo.Context = this;
    rootInfo.Parent = nullptr;
    rootInfo.Rect = { 0.0f, 0.0f, info.Width, info.Height };
    rootInfo.DebugName = "root";
    rootInfo.Color = { 0.0f, 0.0f, 0.0f, 0.0f };
    mRoot.Startup(rootInfo);
}

void UIContext::Cleanup()
{
    mRoot.Cleanup();
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

    // TODO: hot code path, widget hover
}

void UIContext::InputMouseScroll(float& deltaX, float& deltaY)
{
    // TODO: focus widget takes the scroll
}

void UIContext::InputMouseButtonPressed(MouseButton button)
{
    for (int i = (int)mWindowStack.Size() - 1; i >= 0; i--)
    {
        UIWindow* window = mWindowStack[i];
        Rect2D rect = window->GetRect();

        if (rect.Contains(mMousePos))
        {
            Vec2 windowPos = window->GetWindowPos();
            window->InputMouseButtonPressed(button, mMousePos - windowPos);
            break;
        }
    }
}

void UIContext::InputMouseButtonReleased(MouseButton button)
{
    for (int i = (int)mWindowStack.Size() - 1; i >= 0; i--)
    {
        UIWindow* window = mWindowStack[i];
        Rect2D rect = window->GetRect();

        if (rect.Contains(mMousePos))
        {
            Vec2 windowPos = window->GetWindowPos();
            window->InputMouseButtonReleased(button, mMousePos - windowPos);
            break;
        }
    }
}

void UIContext::InputKeyPress(KeyCode key)
{
}

void UIContext::InputKeyRelease(KeyCode key)
{
}

} // namespace LD