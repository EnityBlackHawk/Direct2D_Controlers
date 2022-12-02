#include "Frame.h"

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
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    pRenderTarget->FillRoundedRectangle(roundRect, pSolidColorBrush);
    EndPaint(hwnd, &ps);
    
}

void Frame::CreateResources(ID2D1HwndRenderTarget* pRenderTarget)
{
    pRenderTarget->CreateSolidColorBrush(style.getBackgroundColors()[0], &pSolidColorBrush);
    roundRect = D2D1::RoundedRect(D2D1::Rect(posX, posY, posX + width, posY + height), style.getCornerRadius(), style.getCornerRadius());
}

void Frame::Move(int x, int y)
{
    posX = x;
    posY = y;
    roundRect = D2D1::RoundedRect(D2D1::Rect(posX, posY, posX + width, posY + height), style.getCornerRadius(), style.getCornerRadius());
}

void Frame::SetColor(D2D1::ColorF color)
{
    pSolidColorBrush->SetColor(color);
}

