#include "Label.h"

Label::Label(int x, int y, int width, int height, unsigned char align, const WCHAR text[], float fontSize, ElementStyle style):
    Element(x, y, width, height, align), text(text), fontSize(fontSize), style(style)
{
}

void Label::SetText(const WCHAR text[])
{
    Label::text = text;
}

void Label::Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget * pRenderTarget)
{
    CreateResources(pRenderTarget);
}

void Label::SetOpacity(float opacity)
{
    pSolidColorBrush->SetOpacity(opacity);
}

void Label::Move(int x, int y)
{
    posX = x;
    posY = y;
}

HWND Label::Show(HWND hParent, HINSTANCE hInstance)
{
    return HWND();
}

void Label::OnPaint(ID2D1RenderTarget* pRenderTarget)
{
    pRenderTarget->DrawTextA(text, getTextSize(text), pWriteFormat, D2D1::Rect(posX, posY, width + posX, height + posY),
        pSolidColorBrush);
}

int Label::getTextSize(const WCHAR* text)
{
    int i = 0;
    for (; text[i] != '\0'; i++);
    return i;
}

void Label::CreateResources(ID2D1RenderTarget* pRenderTarget)
{
    pRenderTarget->CreateSolidColorBrush(style.getForegroundColor(), &pSolidColorBrush);

    const WCHAR fontName[] = L"Arial";
    HRESULT hr;

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pWriteFactory), (IUnknown**)&pWriteFactory);

    hr = pWriteFactory->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &pWriteFormat);

    if (height == AUTO || width == AUTO)
    {
        hr = pWriteFactory->CreateTextLayout(text, getTextSize(text), pWriteFormat, 999, 999, &pTextLayout);
        
        DWRITE_TEXT_METRICS tm;
        pTextLayout->GetMetrics(&tm);

        if (width == AUTO) width = tm.width + 1;
        if (height == AUTO) height = tm.height + 1;

        pTextLayout->Release();

    }

    pWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    pWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

}
