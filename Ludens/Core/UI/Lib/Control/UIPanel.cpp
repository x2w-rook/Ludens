#include "Core/UI/Include/Control/UIPanel.h"

namespace LD {

UIPanel::UIPanel() : UIWidget(UIType::Panel)
{
}

UIPanel::~UIPanel()
{
    UIWidget::Cleanup();
}

void UIPanel::Startup(const UIPanelInfo& info)
{
    UIWidget::Startup(info);
}

void UIPanel::Cleanup()
{
    UIWidget::Cleanup();
}

} // namespace LD