#include "Image.h"

Image::Image(int x, int y, int width, int height, unsigned char align, const WCHAR* filePath):
    Element(x, y, width, height, align), imagePath(filePath)
{
}

void Image::Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget)
{
    CreateResources(pRenderTarget);
}

void Image::SetOpacity(float opacity)
{
    Image::opacity = opacity;
}

void Image::Move(int x, int y)
{
}

HWND Image::Show(HWND hParent, HINSTANCE hInstance)
{
    return HWND();
}

void Image::OnPaint(ID2D1RenderTarget* pRenderTarget)
{


    pRenderTarget->DrawBitmap(pBitmap, D2D1::RectF(posX, posY, width + posX, (width * imageAspectRatio) + posY), opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF(0, 0, pBitmap->GetSize().width, pBitmap->GetSize().height)
    );

}

void Image::CreateResources(ID2D1RenderTarget* pRenderTaget)
{
    CoInitialize(NULL);
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory2,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory2,
        (LPVOID*)&pImagingFactory
    );
    
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pSource = nullptr;
    IWICStream* pStrem = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    pImagingFactory->CreateDecoderFromFilename(
        imagePath,
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
        0,
        WICBitmapPaletteTypeMedianCut
    );

   
    hr = pRenderTaget->CreateBitmapFromWicBitmap(pConverter, NULL, &pBitmap);

    float w = pBitmap->GetSize().width;
    float h = pBitmap->GetSize().height;
    imageAspectRatio = h / w;
    height = height == AUTO ? width * imageAspectRatio : height;

    pDecoder->Release();
    pSource->Release();
    pConverter->Release();
    CoUninitialize();

}
