#include "Button.h"
#include "Exception.h"

Button::Button(int x, int y, int width, int height, unsigned char align, const WCHAR text[], ElementStyle style):
    Element(x, y, width, height, align), text(text), style(style), fontSize(20.0f)
{
}

void Button::Create(HINSTANCE hInstance, HWND hParent, ID2D1HwndRenderTarget* pRenderTarget)
{
    ppRenderTarget = &pRenderTarget;
    CreateResources(pRenderTarget);
}

void Button::Move(int x, int y)
{
    posX = x;
    posY = y;
    if (pLinearGradientBrush)
    {
        pLinearGradientBrush->SetStartPoint(D2D1::Point2(posX, posY));
        pLinearGradientBrush->SetEndPoint(D2D1::Point2(posX + width, posY + height));
    }
    roundRect = D2D1::RoundedRect(D2D1::Rect(posX, posY, posX + width, posY + height), style.getCornerRadius(), style.getCornerRadius());
}

HWND Button::Show(HWND hParent, HINSTANCE hInstance)
{
    return hwnd;
}

static int getTextSize(const WCHAR* text)
{
    int i = 0;
    for (; text[i] != '\0'; i++);
    return i;
}

void Button::OnPaint(ID2D1HwndRenderTarget* pRenderTarget)
{
    if(style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarget->FillRoundedRectangle(roundRect, pSolidColorBrush);
    else
        pRenderTarget->FillRoundedRectangle(roundRect, pLinearGradientBrush);
    pRenderTarget->DrawTextA(text, getTextSize(text), pWriteFormat, D2D1::Rect(posX, posY, width + posX, height + posY),
       pForegroundBrush);
}

void Button::SetColorSolidColor(D2D1::ColorF color)
{
    if (pSolidColorBrush)
    {
        pSolidColorBrush->SetColor(color);
    }
}


void Button::CreateResources(ID2D1HwndRenderTarget* pRenderTarget)
{
    if (style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarget->CreateSolidColorBrush(style.getBackgroundColors()[0], &pSolidColorBrush);
    else if (style.getTypeOfBrush() == LINEAR_GRADIENT)
    {
        auto& vec = style.getBackgroundColors();
        ID2D1GradientStopCollection* pGradientStops = nullptr;
        D2D1_GRADIENT_STOP* gradientStop = (D2D1_GRADIENT_STOP*)malloc(sizeof(D2D1_GRADIENT_STOP) * vec.size());
        if (gradientStop)
        {
            for (int i = 0; i < vec.size(); i++)
            {
                gradientStop[i].color = vec[i];
                gradientStop[i].position = i / (vec.size() - 1);
            }
            pRenderTarget->CreateGradientStopCollection(gradientStop, vec.size(), D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pGradientStops);

            HRESULT hr = pRenderTarget->CreateLinearGradientBrush(
                D2D1::LinearGradientBrushProperties(
                    D2D1::Point2(posX, posY),
                    D2D1::Point2(posX + width, posY + height)
                ),
                pGradientStops,
                &pLinearGradientBrush
            );

            if (!SUCCEEDED(hr))
            {
                ERROR_MESSAGE_LAST_ERROR();
            }
        }
    }
    roundRect = D2D1::RoundedRect(D2D1::Rect(posX, posY, posX + width, posY + height), style.getCornerRadius(), style.getCornerRadius());

    pRenderTarget->CreateSolidColorBrush(style.getForegroundColor(), &pForegroundBrush);


    const WCHAR fontName[] = L"Arial";
    HRESULT hr;

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pWriteFactory), (IUnknown**)&pWriteFactory);

    hr = pWriteFactory->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &pWriteFormat);

    pWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    pWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

}
