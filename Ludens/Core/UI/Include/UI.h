#pragma once

#include "Core/OS/Include/Time.h"
#include "Core/Math/Include/Rect2D.h"
#include "Core/Math/Include/Vec2.h"
#include "Core/Math/Include/Vec4.h"
#include "Core/DSA/Include/Vector.h"
#include "Core/DSA/Include/View.h"
#include "Core/Application/Include/Input.h"
#include "Core/UI/Include/UILayout.h"
#include "Core/UI/Include/UIWidget.h"
#include "Core/UI/Include/Control/UILabel.h"
#include "Core/UI/Include/Control/UIPanel.h"
#include "Core/UI/Include/Control/UIButton.h"
#include "Core/UI/Include/Control/UITexture.h"

namespace LD {

class UIContext;
class UIWindow;
class UIWidget;
class UIPanel;
class UIScroll;

struct UIFontInfo
{
    Ref<FontTTF> TTF;
    Ref<FontGlyphTable> GlyphTable;
};

/// font data required by the UI framework
class UIFont
{
public:
    UIFont();
    UIFont(const UIFont&) = delete;
    ~UIFont();

    void Startup(const UIFontInfo& info);
    void Cleanup();

    Ref<FontTTF> GetTTF();
    Ref<FontGlyphTable> GetGlyphTable();

    /// @brief calculate width and height from text when rendered on a single line
    /// @param text the text to calculate size from
    /// @param ratio the desired display size divided by actual font/glyph size
    /// @param size the width and height that the text occupies
    /// @param glyphs a pointer to an array of glyphs, one glyph per character in text
    /// @return true of all glyphs in text are found, false otherwise
    bool DeriveTextSize(const std::string& text, float ratio, Vec2& size, FontGlyphExt* glyphs);

    /// @brief calculate width and height from text when rendered on a single line
    /// @param text the text to calculate size from
    /// @param ratio the desired display size divided by actual font/glyph size
    /// @param width the limiting width per line of text
    /// @param height the resulting height, likely an integer multiple of font line space
    /// @param glyphs a pointer to an array of glyphs, one glyph per character in text
    /// @return true of all glyphs in text are found, false otherwise
    bool DeriveTextSizeLimitWidth(const std::string& text, float ratio, float width, float& height,
                                  FontGlyphExt* glyphs);

private:
    Ref<FontTTF> mTTF;
    Ref<FontGlyphTable> mGlyphTable;
};

/// stack of pointers for logic ordering,
/// input events are passed from stack top to bottom
/// while items are rendered from stack bottom to top
template <typename T>
class UILogicStack
{
public:
    void PushTop(T* item)
    {
        mItems.PushBack(item);
    }

    void Remove(T* item)
    {
        size_t i;
        size_t count = mItems.Size();

        for (i = 0; i < count && mItems[i] != item; i++)
            ;

        if (i >= count)
            return;

        for (; i + 1 < count; i++)
            mItems[i] = mItems[i + 1];

        mItems.Resize(count - 1);
    }

    inline const T* operator[](int idx) const
    {
        LD_DEBUG_ASSERT(0 <= idx && idx < mItems.Size());
        return mItems[idx];
    }

    inline T* operator[](int idx)
    {
        LD_DEBUG_ASSERT(0 <= idx && idx < mItems.Size());
        return mItems[idx];
    }

    View<T*> GetView() const
    {
        return { mItems.Size(), mItems.Begin() };
    }

    size_t Size() const
    {
        return mItems.Size();
    }

private:
    Vector<T*> mItems;
};

struct UIWindowInfo
{
    UIWindow* Parent;

    /// position and size of the window. the position is in root coordinates.
    Rect2D Rect;
};

class UIWindow : public UIWidget
{
    friend class UIWidget;

public:
    UIWindow();
    UIWindow(const UIWindow&) = delete;
    ~UIWindow();

    UIWindow& operator=(const UIWindow&) = delete;

    void Startup(const UIWindowInfo& info, UIContext* context);
    void Cleanup();

    UIContext* GetContext()
    {
        return mContext;
    }

    /// get a view to all the widgets in the window, from bottom to top.
    /// the view is invalidated whenever widget ordering changes.
    View<UIWidget*> GetWidgets()
    {
        return mWidgetStack.GetView();
    }

    /// get the window's position in root coordinates
    Vec2 GetRootPos() const;

    /// set the window's position in root coordinates
    void SetRootPos(const Vec2& pos);

    /// get width and height of the window
    Vec2 GetSize() const;

    /// get window rect relative to parent
    Rect2D GetRect() const;

    void GetData(void** user_data);

    void SetSize(int w, int h);
    void SetColor(Vec4 color);
    void SetData(void* user_data);
    void Show();
    void Hide();

    /// raise the window to the top
    void Raise();

    // by default, the context forwards input events to destination window,
    // but the user can also directly inject input into a specific window.

    void InputMouseButtonPressed(MouseButton button, const Vec2& pos);
    void InputMouseButtonReleased(MouseButton button, const Vec2& pos);
    void InputKeyPress(KeyCode key);
    void InputKeyRelease(KeyCode key);

private:
    void Attach(UIWindow* parent);
    void Detach();

    UIWidget* GetTopWidget(const Vec2& pos);

    void* data; // user data handle
    Rect2D mRect;
    UILogicStack<UIWidget> mWidgetStack; // widget stack, one per window
    UIContext* mContext = nullptr;
    UIWindow* mParent;      // parent window
    UIWindow* mChild;       // first child window
    UIWindow* mNext;        // next sibling window
    UIWidget* mHoverWidget; // widget hovered by mouse cursor
};

struct UIContextInfo
{
    float Width;
    float Height;
};

/// UI elements live within a context, and can not interact with UI elements
/// residing in another context.
class UIContext
{
    friend class UIWindow;

public:
    UIContext() = default;
    UIContext(const UIContext&) = delete;
    ~UIContext();

    UIContext& operator=(const UIContext&) = delete;

    void Startup(const UIContextInfo& info);
    void Cleanup();

    UIWindow* GetRoot();

    /// get the width and height of this context, which
    /// is defined by the size of the root window.
    Vec2 GetSize();

    /// get a view to all the windows in the context, from bottom to top.
    /// the view is invalidated whenever window ordering changes.
    View<UIWindow*> GetWindows()
    {
        return mWindowStack.GetView();
    }

    void BeginFrame(DeltaTime dt);
    void EndFrame();

    /// raise window to the top
    void RaiseWindow(UIWindow* window);

    // UI context still requires a driver that continuosly updates the input state,
    // user can freeze or save some CPU by not updating contexts that are inactive.

    void InputMousePosition(Vec2 pos);
    void InputMouseScroll(float& deltaX, float& deltaY);
    void InputMouseButtonPressed(MouseButton button);
    void InputMouseButtonReleased(MouseButton button);
    void InputKeyPress(KeyCode key);
    void InputKeyRelease(KeyCode key);

private:
    UILogicStack<UIWindow> mWindowStack; // window stack, one per context
    UIWindow mRoot;                      // root window is provided by context
    UIWindow* mFocus;                    // window receiving key input
    UIWindow* mPress;                    // window last pressed and not released by mouse button
    UIWindow* mHoverWindow;              // window last entered and not leaved by mouse cursor
    UIWidget* mDragWidget;               // widget pressed and dragged by mouse cursor
    UIWidget* mLastAttach;               // last widget that is attached to a parent
    UIWidget* mLastDetach;               // last widget that is detached from a parent
    UIWidget* mTooltip;                  // the tooltip widget, rendered beside mouse cursor
    Vec2 mMousePos;                      // mouse cursor position in the viewport
    Vec2 mAnchorPos;                     // anchor position, transforms local position to viewport position
    bool mIsWithinFrame = false;
    bool mIsWithinDraw = false;
};

} // namespace LD
