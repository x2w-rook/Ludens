#include "Core/Document/Include/Document.h"
#include "Core/Document/Include/DocumentLink.h"

#define LD_DOCUMENT_PADDING 60

namespace LD
{

Document::Document(const DocumentInfo& info)
{
    LD_DEBUG_ASSERT(info.Parent);

    mID = CUID<Document>::Get();
    mContext = info.Parent->GetContext();

    Rect2D windowRect;
    windowRect.x = 0.0f;
    windowRect.y = 0.0f;
    windowRect.w = info.Width;
    windowRect.h = info.Height;

    UIWindowInfo windowI;
    windowI.Parent = info.Parent;
    windowI.Rect = windowRect;
    mWindow.Startup(windowI, mContext);
    mWindow.SetPadding(LD_DOCUMENT_PADDING, UIEdge::Top);
    mWindow.SetHPadding(LD_DOCUMENT_PADDING);

    UILabelInfo labelI;
    labelI.FlexGrow = 0.0f;
    labelI.Parent = &mWindow;
    labelI.Font = info.Font;
    labelI.Text = info.Title;
    labelI.TextColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    labelI.TextSize = 60.0f;
    mTitle.Startup(labelI);
}

Document::~Document()
{
    mWindow.Cleanup();

    for (StackAllocator& page : mPages)
        page.Cleanup();
}

void Document::SetTitle(const char* title)
{
    mTitle.SetText(title);
}

UIWindow* Document::GetWindow()
{
    return &mWindow;
}

UID Document::GetID() const
{
    return mID;
}

} // namespace LD