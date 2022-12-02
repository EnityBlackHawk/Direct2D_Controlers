#include "Window.h"
#include <dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")

Window::Window(const char* name, HINSTANCE hInstance, WNDPROC windowProcedure, MainWindowSyle colorStyle, DWORD style, DWORD styleEx):
    name(name), style(colorStyle), hInstance(hInstance)
{
    elements = {};

    WNDCLASS wc = {};
    wc.lpszClassName = name;
    wc.lpfnWndProc = windowProcedure;
    wc.hbrBackground = CreateSolidBrush(colorStyle.background);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    hwnd = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW | styleEx,
        name,
        name,
        WS_TILEDWINDOW | style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd)
        ERROR_MESSAGE_LAST_ERROR();

    HRESULT h;
    if(colorStyle.captionColor)
        h = DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &colorStyle.captionColor, sizeof(colorStyle.captionColor));
    if (colorStyle.titleColor != NULL)
    {
        h = DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR, &colorStyle.titleColor, sizeof(colorStyle.titleColor));
    }
    if (colorStyle.borderColor != NULL)
    {
        h = DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &colorStyle.borderColor, sizeof(colorStyle.borderColor));
    }

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, size), &pRenderTarget);
    

}

LRESULT Window::InternalWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_CREATE:
        
        return 0;

    case WM_SHOWWINDOW:

        for (auto e : elements)
        {
            e->Show(hwnd, hInstance);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps = {};
        BeginPaint(hwnd, &ps);
        pRenderTarget->BeginDraw();
        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
        for (auto e : elements)
        {
            e->OnPaint(pRenderTarget);
        }
        pRenderTarget->EndDraw();
        EndPaint(hwnd, &ps);
        break;
    }


    case WM_SIZE:
    {
        RECT rc;
        GetClientRect(hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        pRenderTarget->Resize(size);
        for (auto e : elements)
        {
            e->OnSizeChanged(pRenderTarget);
        }
        Redraw();
        break;
    }
       
    case WM_ERASEBKGND:
    {
        break;
    }

    case WM_MOUSEMOVE:
        mouseTracker.SetMousePosition(LOWORD(lParam), HIWORD(lParam));
        mouseTracker.Procedure(mouseTracker);
        break;

    case WM_LBUTTONUP:
    {

        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::Redraw() const
{
    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
    for (auto e : elements)
    {
        e->OnPaint(pRenderTarget);
    }
    pRenderTarget->EndDraw();
}

void Window::Show() const
{
    for (auto e : elements)
    {
        e->Align(pRenderTarget);
        e->Create(hInstance, hwnd, pRenderTarget);
    }


    ShowWindow(hwnd, 10);
    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}

void Window::AddElement(Element& rElement)
{
    rElement.SetParent(hwnd);
    elements.push_back(&rElement);
    mouseTracker.AddElement(&rElement);
}

int Window::GetActualWidth() const
{
    RECT rect;
    GetWindowRect(hwnd, &rect);
    return rect.right - rect.left;
}

int Window::GetActualHeight() const
{
    RECT rect;
    GetWindowRect(hwnd, &rect);
    return rect.bottom - rect.top;
}

Element* Window::operator[](int index)
{
    return elements[index];
}

HWND Window::GetHwnd() const
{
    return hwnd;
}
