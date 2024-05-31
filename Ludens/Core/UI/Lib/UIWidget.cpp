#include "Core/UI/Include/UIWidget.h"
#include "Core/UI/Include/UI.h"
#include "Core/UI/Include/Control/UILabel.h"
#include "Core/UI/Include/Control/UIPanel.h"
#include "Core/UI/Include/Control/UIButton.h"
#include "Core/UI/Include/Control/UITexture.h"

namespace LD
{

UIWidget::UIWidget(UIType type) : mType(type), mNext(nullptr), mParent(nullptr), mChild(nullptr), mIsAlive(false)
{
}

UIWidget::~UIWidget()
{
}

void UIWidget::Startup(const UIWidgetInfo& info)
{
    Attach(info.Parent);

    mUserCallback = info.Callback;

    UILayoutNode* parentLayout = nullptr;


    if (info.Parent && mType != UIType::Window)
        parentLayout = &info.Parent->mLayout;

    UILayoutNodeInfo layoutInfo;
    layoutInfo.Parent = parentLayout;
    layoutInfo.Width = info.Width;
    layoutInfo.Height = info.Height;
    layoutInfo.FlexDirection = info.FlexDirection;
    layoutInfo.FlexGrow = info.FlexGrow;
    mLayout.Startup(layoutInfo);

    mIsAlive = true;
}

void UIWidget::Cleanup()
{
    if (!mIsAlive)
        return;

    // NOTE: child Detach() modifies the mChild linked list,
    //       hence the while loop instead of for loop iteration
    while (mChild)
    {
        CleanupRecursive(mChild);
    }

    mIsAlive = false;
    mLayout.Cleanup();

    Detach();
}

void UIWidget::CalculateLayout()
{
    mLayout.CalculateLayout();
}

UIType UIWidget::GetType() const
{
    return mType;
}

void UIWidget::SetPadding(float padding)
{
    mLayout.SetPadding(padding);
}

void UIWidget::SetPadding(float padding, UIEdge edge)
{
    mLayout.SetPadding(padding, edge);
}

void UIWidget::SetMargin(float margin)
{
    mLayout.SetMargin(margin);
}

void UIWidget::SetMargin(float margin, UIEdge edge)
{
    mLayout.SetMargin(margin, edge);
}

Vec2 UIWidget::GetPos() const
{
    Vec2 pos;

    mLayout.GetPos(pos);

    return pos;
}

Rect2D UIWidget::GetRect() const
{
    Vec2 pos;
    Vec2 size;

    mLayout.GetSize(size);
    mLayout.GetPos(pos);

    return { pos.x, pos.y, size.x, size.y };
}

void UIWidget::OnEnter()
{
    UIContext* ctx = mWindow->GetContext();

    // TODO: cursor hint
    // if (ctx->cursor_hint && widget->on_press)
    //	ctx->cursor_hint(ctx, IG_CURSOR_PRESS);

    if (mLibCallback.OnEnter)
        mLibCallback.OnEnter(ctx, this);

    if (mUserCallback.OnEnter)
        mUserCallback.OnEnter(ctx, this);
}

void UIWidget::OnLeave()
{
    UIContext* ctx = mWindow->GetContext();

    // TODO: cursor hint

    if (mLibCallback.OnLeave)
        mLibCallback.OnLeave(ctx, this);

    if (mUserCallback.OnLeave)
        mUserCallback.OnLeave(ctx, this);
}

void UIWidget::OnPress()
{
    UIContext* ctx = mWindow->GetContext();

    if (mLibCallback.OnPress)
        mLibCallback.OnPress(ctx, this);

    if (mUserCallback.OnPress)
        mUserCallback.OnPress(ctx, this);

    // if (widget->on_drag)
    //	widget_drag_begin(widget);
}

void UIWidget::OnRelease()
{
    UIContext* ctx = mWindow->GetContext();

    if (mLibCallback.OnRelease)
        mLibCallback.OnRelease(ctx, this);

    if (mUserCallback.OnRelease)
        mUserCallback.OnRelease(ctx, this);
}

void UIWidget::CleanupRecursive(UIWidget* widget)
{
    UIType type = widget->GetType();
    switch (type)
    {
    case UIType::Panel:
        static_cast<UIPanel*>(widget)->Cleanup();
        break;
    case UIType::Label:
        static_cast<UILabel*>(widget)->Cleanup();
        break;
    case UIType::Button:
        static_cast<UIButton*>(widget)->Cleanup();
        break;
    case UIType::Texture:
        static_cast<UITexture*>(widget)->Cleanup();
        break;
    default:
        LD_DEBUG_UNREACHABLE;
    }
}

void UIWidget::Attach(UIWidget* parent)
{
    if (!parent) // window widget
    {
        mWindow = (UIWindow*)this;
        mParent = mNext = mChild = nullptr;
        return;
    }

    mWindow = parent->mWindow;
    mWindow->mWidgetStack.PushTop(this);

    mParent = parent;
    mNext = parent->mChild;
    parent->mChild = this;
}

void UIWidget::Detach()
{
    if (!mParent) // window widget
    {
        mWindow = nullptr;
        return;
    }

    mWindow->mWidgetStack.Remove(this);

    UIWidget** w;
    for (w = &mParent->mChild; *w && *w != this; w = &((*w)->mNext))
        ;
    LD_DEBUG_ASSERT(*w != nullptr);

    *w = (*w)->mNext;
    mParent = nullptr;
}

} // namespace LD