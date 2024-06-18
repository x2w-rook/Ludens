#pragma once

#include "Core/Math/Include/Vec4.h"
#include "Core/Math/Include/Hex.h"
#include "Core/UI/Include/UI.h"
#include "Core/RenderService/Include/RenderService.h"
#include "Core/RenderService/Lib/RenderContext.h"
#include "Core/RenderService/Lib/RenderUI.h"
#include "Core/UI/Include/Control/UILabel.h"
#include "Core/UI/Include/Control/UIPanel.h"
#include "Core/UI/Include/Control/UIButton.h"
#include "Core/UI/Include/Control/UITexture.h"

namespace LD
{

static void RenderUIWindow(RenderContext* ctx, UIWindow* window);
static void RenderUIWidget(RenderContext* ctx, const Rect2D& rect, UIWidget* widget);
static void RenderUILabel(RenderContext* ctx, const Rect2D& rect, UILabel* label);
static void RenderUIPanel(RenderContext* ctx, const Rect2D& rect, UIPanel* panel);
static void RenderUITexture(RenderContext* ctx, const Rect2D& rect, UITexture* texture);
static void RenderUIButton(RenderContext* ctx, const Rect2D& rect, UIButton* button);

void RenderUI(RenderContext* ctx, UIContext* ui)
{
    for (UIWindow* window : ui->GetWindows())
    {
        RenderUIWindow(ctx, window);
    }
}

static void RenderUIWindow(RenderContext* ctx, UIWindow* window)
{
    Rect2D windowRect = window->GetWindowRect();
    Vec4 windowColor = window->GetColor();
    RectBatch& batch = ctx->DefaultRectBatch;
    batch.AddRectFilled(windowRect, windowColor);

    float border = window->GetBorder();

    Vec4 borderColor = Vec4::Lerp(windowColor, { 1.0f, 1.0f, 1.0f, windowColor.a }, 0.5f);
    batch.AddRectOutline(windowRect, borderColor, border);

    for (UIWidget* widget = window->GetChildren(); widget; widget = widget->GetNext())
    {
        Rect2D rect = widget->GetRect();
        rect.x += windowRect.x;
        rect.y += windowRect.y;
        RenderUIWidget(ctx, rect, widget);
    }
}

static void RenderUIWidget(RenderContext* ctx, const Rect2D& rect, UIWidget* widget)
{
    UIType type = widget->GetType();
    switch (type)
    {
    case UIType::Panel:
        RenderUIPanel(ctx, rect, (UIPanel*)widget);
        break;
    case UIType::Label:
        RenderUILabel(ctx, rect, (UILabel*)widget);
        break;
    case UIType::Button:
        RenderUIButton(ctx, rect, (UIButton*)widget);
        break;
    case UIType::Texture:
        RenderUITexture(ctx, rect, (UITexture*)widget);
        break;
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

static void RenderUILabel(RenderContext* ctx, const Rect2D& rect, UILabel* label)
{
    View<FontGlyphExt> glyphsExts = label->GetTextGlyphs();
    float scale = label->GetGlyphScale();
    UIFont* uiFont = label->GetFont();
    Ref<FontTTF> ttf = uiFont->GetTTF();
    RectBatch& batch = ctx->DefaultRectBatch;

    int ascent;
    ttf->GetVerticalMetrics(&ascent, nullptr, nullptr, nullptr);

    // put cursor at baseline, left most point
    Vec2 cursor = Vec2{ rect.x, rect.y + ascent * scale };

    Vec4 bgColor, fgColor;
    label->GetColors(bgColor, fgColor);

    if (bgColor.a != 0.0f)
        batch.AddRectFilled(rect, bgColor);

    for (const FontGlyphExt& glyphExt : glyphsExts)
    {
        ctx->DefaultRectBatch.AddGlyph(cursor + glyphExt.Offset, glyphExt, scale, fgColor, 1);
    }
}

static void RenderUIButton(RenderContext* ctx, const Rect2D& rect, UIButton* button)
{
    View<FontGlyphExt> glyphsExts = button->GetTextGlyphs();
    float scale = button->GetGlyphScale();
    UIFont* uiFont = button->GetFont();
    Ref<FontTTF> ttf = uiFont->GetTTF();
    RectBatch& batch = ctx->DefaultRectBatch;

    Vec4 bgColor, fgColor;
    button->GetColors(bgColor, fgColor);

    batch.AddRectFilled(rect, bgColor);

    int ascent;
    ttf->GetVerticalMetrics(&ascent, nullptr, nullptr, nullptr);

    // put cursor at baseline, left most point
    Vec2 cursor = Vec2{ rect.x, rect.y + ascent * scale };

    for (const FontGlyphExt& glyphExt : glyphsExts)
    {
        ctx->DefaultRectBatch.AddGlyph(cursor + glyphExt.Offset, glyphExt, scale, fgColor, 1);
    }
}

static void RenderUITexture(RenderContext* ctx, const Rect2D& rect, UITexture* texture)
{
    Vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
    Vec2 size(rect.w, rect.h);
    ctx->DefaultRectBatch.AddTexture(rect, { 0.0f, 0.0f, rect.w, rect.h }, size, white, texture->GetTextureID());
}

} // namespace LD