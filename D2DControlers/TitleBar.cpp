#include "TitleBar.h"


TitleBar::TitleBar(int x, int y, int width, int height, unsigned char align, ElementStyle style):
    Element(x, y, width, height, align), style(style), 
    text(5, 2, AUTO, AUTO, ALIGN_NONE, L"Title", 20, ElementStyle(0x0, 0x000000, 0, 0, 0))
{
}

void TitleBar::Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget * pRenderTarget)
{
    CreateResources(pRenderTarget);
    text.Create(NULL, NULL, pRenderTarget);
}

void TitleBar::SetOpacity(float opacity)
{
}

float TitleBar::GetOpacity()
{
    return 0.0f;
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

void TitleBar::ExOnPaint(ID2D1DeviceContext5* pRenderTarger)
{
    pRenderTarger->FillRoundedRectangle(&roundRect, pSolidColorBrush);

    pRenderTarger->FillRectangle(D2D1::RectF(width - 55, 0, width, 30), closeButtonBrush);
    pRenderTarger->FillRectangle(D2D1::RectF(width - 101, 0, width - 56, 30), maxButtonBrush);
    pRenderTarger->FillRectangle(D2D1::RectF(width - 148, 0, width-102, 30), minButtonBrush);

    D2D1_RECT_F dest = D2D1::RectF(width - 35.f, 7.5f, width - 20.f, 22.5f);

    pRenderTarger->DrawBitmap(pBitmap, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, pBitmap->GetSize().width, pBitmap->GetSize().height));

    D2D1_MATRIX_3X2_F transform;

    transform = D2D1::Matrix3x2F::Translation(width - (101 - 12.5f), 5);
    pRenderTarger->SetTransform(transform);
    pRenderTarger->DrawSvgDocument(pSvgMax);
    pRenderTarger->SetTransform(D2D1::Matrix3x2F::Translation(width - (148 - 12.5f), 5));
    pRenderTarger->DrawSvgDocument(pSvgMin);
    pRenderTarger->SetTransform(D2D1::Matrix3x2F::Translation(0,0));

    text.OnPaint(pRenderTarger);
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

    hr = pImagingFactory->CreateStream(&pStream);
    hr = pStream->InitializeFromFilename(L"browsers-outline.svg", GENERIC_READ);
    hr = reinterpret_cast<ID2D1DeviceContext5*>(pRenderTarger)->CreateSvgDocument(pStream, D2D1::SizeF(20, 20), &pSvgMax);

    pStream->Release();
    hr = pImagingFactory->CreateStream(&pStream);
    hr = pStream->InitializeFromFilename(L"remove-outline.svg", GENERIC_READ);
    reinterpret_cast<ID2D1DeviceContext5*>(pRenderTarger)->CreateSvgDocument(pStream, D2D1::SizeF(20, 20), &pSvgMin);


    if (style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarger->CreateSolidColorBrush(style.getBackgroundColors()[0], &pSolidColorBrush);

    roundRect = D2D1::RoundedRect(D2D1::RectF(posX, posY, posX + width, posY + height), 0, 0);

    pRenderTarger->CreateSolidColorBrush(D2D1::ColorF(0xFF0000), &closeButtonBrush);
    pRenderTarger->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF, 0.5f), &maxButtonBrush);
    pRenderTarger->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF, 0.5f), &minButtonBrush);
    ResetHover();


    pDecoder->Release();
    pSource->Release();
    pConverter->Release();
    pStream->Release();
    
    CoUninitialize();
}
