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

    pRenderTarger->FillRectangle(D2D1::RectF(width - 55, 0, width, 30), closeButtonBrush);
    pRenderTarger->FillRectangle(D2D1::RectF(width - 101, 0, width - 56, 30), maxButtonBrush);
    pRenderTarger->FillRectangle(D2D1::RectF(width - 148, 0, width-102, 30), minButtonBrush);

    D2D1_RECT_F dest = D2D1::RectF(width - 35, 7.5f, width - 20, 22.5f);
    pRenderTarger->DrawBitmap(pBitmap, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, pBitmap->GetSize().width, pBitmap->GetSize().height));
}

void TitleBar::hoverCloseButton()
{
    closeButtonBrush->SetOpacity(1.0f);
}

void TitleBar::hoverMaxButton()
{
    maxButtonBrush->SetOpacity(1);
}

void TitleBar::hoverMinButton()
{
    minButtonBrush->SetOpacity(1);
}

void TitleBar::ResetHover()
{
    closeButtonBrush->SetOpacity(0);
    maxButtonBrush->SetOpacity(0);
    minButtonBrush->SetOpacity(0);
}



void TitleBar::CreateResources(ID2D1RenderTarget* pRenderTarger)
{
    CoInitialize(NULL);
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        (LPVOID*)&pImagingFactory
    );

    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pSource = nullptr;
    IWICStream* pStream = nullptr;
    IWICFormatConverter* pConverter = nullptr;
    IWICBitmapScaler* pScaler = nullptr;

    pImagingFactory->CreateDecoderFromFilename(
        L"Xicon.png",
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );

    pDecoder->GetFrame(0, &pSource);
    pImagingFactory->CreateFormatConverter(&pConverter);

    pConverter->Initialize(
        pSource,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.f,
        WICBitmapPaletteTypeMedianCut
    );
        

    hr = pRenderTarger->CreateBitmapFromWicBitmap(pConverter, NULL, &pBitmap);

    if (style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarger->CreateSolidColorBrush(style.getBackgroundColors()[0], &pSolidColorBrush);

    roundRect = D2D1::RoundedRect(D2D1::RectF(posX, posY, posX + width, posY + height), 0, 0);

    pRenderTarger->CreateSolidColorBrush(D2D1::ColorF(0xFF0000), &closeButtonBrush);
    pRenderTarger->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF), &maxButtonBrush);
    pRenderTarger->CreateSolidColorBrush(D2D1::ColorF(0x0000FF), &minButtonBrush);
    ResetHover();
    
}
