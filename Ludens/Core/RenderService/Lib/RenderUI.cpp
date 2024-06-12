#pragma once

#include "Core/Math/Include/Vec4.h"
#include "Core/Math/Include/Hex.h"
#include "Core/UI/Include/UI.h"
#include "Core/RenderService/Include/RenderService.h"
#include "Core/RenderService/Lib/RenderContext.h"
#include "Core/RenderService/Lib/RenderUI.h"

namespace LD
{

static void RenderUIWidget(RenderContext* ctx, const Rect2D& rect, UIWidget* widget);
static void RenderUILabel(RenderContext* ctx, const Rect2D& rect, UILabel* label);
static void RenderUIPanel(RenderContext* ctx, const Rect2D& rect, UIPanel* panel);
static void RenderUITexture(RenderContext* ctx, const Rect2D& rect, UITexture* texture);

void RenderUI(RenderContext* ctx, UIContext* ui)
{
    View<UIWindow*> windows = ui->GetWindows();
    RectBatch& batch = ctx->DefaultRectBatch;

    for (UIWindow* window : windows)
    {
        Rect2D windowRect = window->GetRect();
        batch.AddRectFilled(windowRect, window->GetColor());

        UIWidget* widget;
        for (widget = window->GetChildren(); widget; widget = widget->GetNext())
        {
            Rect2D rect = widget->GetRect();
            rect.x += windowRect.x;
            rect.y += windowRect.y;
            batch.AddRectOutline(rect, { 0.8f, 0.0f, 0.0f, 1.0f }, 2.0f);

            RenderUIWidget(ctx, rect, widget);
        }
    }
}

static void RenderUIWidget(RenderContext* ctx, const Rect2D& rect, UIWidget* widget)
{
    UIType type = widget->GetType();
    switch (type)
    {
    case UIType::Label:
        RenderUILabel(ctx, rect, (UILabel*)widget);
        break;
    case UIType::Texture:
        RenderUITexture(ctx, rect, (UITexture*)widget);
        break;
    case UIType::Panel:
        RenderUIPanel(ctx, rect, (UIPanel*)widget);
        break;
    }
}

static void RenderUILabel(RenderContext* ctx, const Rect2D& rect, UILabel* label)
{
    View<FontGlyphExt> glyphsExts = label->GetTextGlyphs();
    float scale = label->GetGlyphScale();
    UIFont* uiFont = label->GetFont();
    Ref<FontTTF> ttf = uiFont->GetTTF();

    int ascent;
    ttf->GetVerticalMetrics(&ascent, nullptr, nullptr, nullptr);

    // put cursor at baseline, left most point
    Vec2 cursor = Vec2{ rect.x, rect.y + ascent * scale };

    for (const FontGlyphExt& glyphExt : glyphsExts)
    {
        ctx->DefaultRectBatch.AddGlyph(cursor + glyphExt.Offset, glyphExt, scale, label->GetTextColor(), 1);
    }
}

static void RenderUIPanel(RenderContext* ctx, const Rect2D& rect, UIPanel* panel)
{
    for (UIWidget* widget = panel->GetChildren(); widget; widget = widget->GetNext())
    {
        Vec2 widgetPos = widget->GetPos();
        Rect2D widgetRect = rect;
        widgetRect.x += widgetPos.x;
        widgetRect.y += widgetPos.y;
        RenderUIWidget(ctx, widgetRect, widget);
    }
}

static void RenderUITexture(RenderContext* ctx, const Rect2D& rect, UITexture* texture)
{
    Vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
    Vec2 size(rect.w, rect.h);
    ctx->DefaultRectBatch.AddTexture(rect, { 0.0f, 0.0f, rect.w, rect.h }, size, white, texture->GetTextureID());
}

} // namespace LD