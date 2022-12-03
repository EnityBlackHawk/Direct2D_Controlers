#include "Frame.h"
#include "Exception.h"

LRESULT Frame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SETCURSOR:
        SetCursor(handCursor);
        break;

    case WM_MOUSEMOVE:
        OutputDebugString("Clicked");
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Frame::Frame(int x, int y, int width, int height, unsigned char align, ElementStyle style)
    : Element(x, y, width, height, align), style(style)
{
    handCursor = LoadCursor(NULL, IDC_HAND);
}

void Frame::Create(HINSTANCE hInstance, HWND hParent, ID2D1HwndRenderTarget* pRenderTarget)
{
    //CreateWnd(posX, posY, width, height, "RoundRect", hInstance, WS_CHILD, hParent, this);
    CreateResources(pRenderTarget);
}

HWND Frame::Show(HWND hParent, HINSTANCE hInstance)
{
    ShowWindow(hwnd, 10);
    return hwnd;
}

void Frame::OnPaint(ID2D1HwndRenderTarget* pRenderTarget)
{

    if(style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarget->FillRoundedRectangle(roundRect, pSolidColorBrush);
    else
        pRenderTarget->FillRoundedRectangle(roundRect, pLinearGradientBrush);
    
}

void Frame::CreateResources(ID2D1HwndRenderTarget* pRenderTarget)
{
    if(style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarget->CreateSolidColorBrush(style.getBackgroundColors()[0], &pSolidColorBrush);
    else if (style.getTypeOfBrush() == LINEAR_GRADIENT)
    {
        auto& vec = style.getBackgroundColors();
        ID2D1GradientStopCollection* pGradientStops = nullptr;
        D2D1_GRADIENT_STOP* gradientStop = (D2D1_GRADIENT_STOP*) malloc(sizeof(D2D1_GRADIENT_STOP) * vec.size());
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
}

void Frame::Move(int x, int y)
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

void Frame::SetColor(D2D1::ColorF color)
{
    pSolidColorBrush->SetColor(color);
}

