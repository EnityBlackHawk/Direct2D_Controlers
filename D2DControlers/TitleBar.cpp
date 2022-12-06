#include "TitleBar.h"


TitleBar::TitleBar(int x, int y, int width, int height, unsigned char align, ElementStyle style):
    Element(x, y, width, height, align), style(style)
{
}

void TitleBar::Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget * pRenderTarget)
{
    CreateResources(pRenderTarget);
}

void TitleBar::SetOpacity(float opacity)
{
}

void TitleBar::Move(int x, int y)
{
    roundRect = D2D1::RoundedRect(D2D1::RectF(posX, posY, posX + width, posY + height), 0, 0);
}

HWND TitleBar::Show(HWND hParent, HINSTANCE hInstance)
{
    return HWND();
}

void TitleBar::OnPaint(ID2D1RenderTarget* pRenderTarget)
{
    pRenderTarget->FillRoundedRectangle(&roundRect, pSolidColorBrush);
}

void TitleBar::ExOnPaint(ID2D1RenderTarget* pRenderTarger)
{
    pRenderTarger->FillRoundedRectangle(&roundRect, pSolidColorBrush);
}


void TitleBar::CreateResources(ID2D1RenderTarget* pRenderTarger)
{
    if (style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarger->CreateSolidColorBrush(style.getBackgroundColors()[0], &pSolidColorBrush);

    roundRect = D2D1::RoundedRect(D2D1::RectF(posX, posY, posX + width, posY + height), 0, 0);
}
