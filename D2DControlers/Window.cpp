#include "Window.h"
#include "Exception.h"
#include <dwmapi.h>
#include <Uxtheme.h>
#include <vssym32.h>
#include <sstream>
#include <windowsx.h>
#include "ElementStyle.h"

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "UxTheme.lib")

#define LEFTEXTENDWIDTH 8
#define RIGHTEXTENDWIDTH 8
#define BOTTOMEXTENDWIDTH 20
#define TOPEXTENDWIDTH 27


// Hit test the frame for resizing and moving.
LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    // Get the point coordinates for the hit test.
    POINT ptMouse = { LOWORD(lParam), HIWORD(lParam) };

    // Get the window rectangle.
    RECT rcWindow;
    GetWindowRect(hWnd, &rcWindow);

    // Get the frame rectangle, adjusted for the style without a caption.
    RECT rcFrame = { 0 };
    AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

    // Determine if the hit test is for resizing. Default middle (1,1).
    USHORT uRow = 1;
    USHORT uCol = 1;
    bool fOnResizeBorder = false;

    // Determine if the point is at the top or bottom of the window.
    if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + TOPEXTENDWIDTH)
    {
        fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
        uRow = 0;
    }
    else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - BOTTOMEXTENDWIDTH)
    {
        uRow = 2;
    }

    // Determine if the point is at the left or right of the window.
    if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + LEFTEXTENDWIDTH)
    {
        uCol = 0; // left side
    }
    else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - RIGHTEXTENDWIDTH)
    {
        uCol = 2; // right side
    }

    // Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
    LRESULT hitTests[3][3] =
    {
        { HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
        { HTLEFT,       HTNOWHERE,     HTRIGHT },
        { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
    };

    return hitTests[uRow][uCol];
}

Window::Window(const char* name, HINSTANCE hInstance, WNDPROC windowProcedure, MainWindowSyle colorStyle, DWORD style, DWORD styleEx) :
    name(name), style(colorStyle), hInstance(hInstance), hCursor(LoadCursor(NULL, IDC_ARROW)),
    titleBar(0, 0, AUTO, 30, ALIGN_HORIZONTAL_STREACH, ElementStyle(SOLID_COLOR, {D2D1::ColorF(0xFFFFFF, 0.5f)}, D2D1::ColorF(0x0), 0, D2D1::ColorF(0x0), 0))
{
    elements = {};
    
    WNDCLASS wc = {};
    wc.lpszClassName = name;
    wc.lpfnWndProc = windowProcedure;
    wc.hbrBackground = CreateSolidBrush(colorStyle.background);
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);

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

    /*
    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);*/

    //pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, size), &pRenderTarget);

    CreateDirectXResources();
}

LRESULT Window::InternalWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lR = 0;
    bool bResult =  DwmDefWindowProc(hwnd, uMsg, wParam, lParam, &lR);
    
    if (bResult)
    {

        if (lR == 20)
        {
            if(isSysNavHover)
                titleBar.ResetHover();

            isSysNavHover = true;
            titleBar.hoverCloseButton();
            RequestRedraw();
        }

        else if (lR == 9)
        {
            if (isSysNavHover)
                titleBar.ResetHover();

            isSysNavHover = true;
            titleBar.hoverMaxButton();
            RequestRedraw();
        }

        else if (lR == 8)
        {
            if (isSysNavHover)
                titleBar.ResetHover();

            isSysNavHover = true;
            titleBar.hoverMinButton();
            RequestRedraw();
        }

        return lR;
    }


    switch (uMsg)
    {

    case WM_ACTIVATE:
    {

        MARGINS margins;

        margins.cxLeftWidth = 8;      // 8
        margins.cxRightWidth = 8;    // 8
        margins.cyBottomHeight = 20; // 20
        margins.cyTopHeight = 27;       // 27

        auto hr = DwmExtendFrameIntoClientArea(hwnd, &margins);
        break;
    }
    case WM_SETCURSOR:
        SetCursor(hCursor);

        break;

    case WM_NCCALCSIZE:
    {
        if (wParam == TRUE)
        {
            // Calculate new NCCALCSIZE_PARAMS based on custom NCA inset.
            NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

            pncsp->rgrc[0].left = pncsp->rgrc[0].left + 0;
            pncsp->rgrc[0].top = pncsp->rgrc[0].top + 0;
            pncsp->rgrc[0].right = pncsp->rgrc[0].right - 0;
            pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - 0;

            return 0;
        }
        break;
     
    }

    case WM_CREATE:
    {
        RECT rcClient;
        GetWindowRect(hwnd, &rcClient);

        // Inform the application of the frame change.
        BOOL b = SetWindowPos(hwnd,
            NULL,
            rcClient.left, rcClient.top,
            rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
            SWP_FRAMECHANGED);
        break;
    }

    case WM_SHOWWINDOW:

        for (auto e : elements)
        {
            e->Show(hwnd, hInstance);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(hwnd, &ps);


        while (isBusy);
        pContext->BeginDraw();
        pContext->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
        for (auto e : elements)
        {
            e->OnPaint(pContext);
        }
        titleBar.ExOnPaint(pContext);
        pContext->EndDraw();
        pSwapChain->Present(1, 0);
        EndPaint(hwnd, &ps);
        if (!hDrawThread)
            hDrawThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Window::DrawThread,
                (void*)this, NULL, nullptr);
        break;
    }


    case WM_SIZE:
    {

        if (wParam == SIZE_MAXIMIZED)
        {
            OutputDebugString("Maximazed");
        }

        RECT rc;
        GetClientRect(hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        if (pContext)
        {
            DXGI_MODE_DESC md = {};
            md.Width = rc.right - rc.left;
            md.Height = rc.bottom - rc.top;

            ReleaseSwapChainDependencies();
        
            HRESULT hr = pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
            CreateBuffers();

            for (auto e : elements)
            {
                e->OnSizeChanged(pContext);
            }
            titleBar.OnSizeChanged(pContext);

            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);

        }
        break;
    }
       
    case WM_ERASEBKGND:
    {
        break;
    }
    case WM_NCHITTEST:
    {
        LRESULT r = HitTestNCA(hwnd, wParam, lParam);
        if (r)
            return r;
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (isSysNavHover)
        {
            titleBar.ResetHover();
            isSysNavHover = false;
            RequestRedraw();
        }

        mouseTracker.SetMousePosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        mouseTracker.Procedure();
        break;
    }

    case WM_LBUTTONUP:
    {
        mouseTracker.LeftClickProcedure();
        break;
    }

    case WM_CHAR:
    {
        std::wstringstream os;
        os << (const wchar_t)wParam;
        os << "\n";
        
        auto wString = os.str();
        std::string s(wString.begin(), wString.end());

        OutputDebugString(s.c_str());
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::Redraw()
{
    while (isBusy);
    isBusy = true;
    pContext->BeginDraw();
    pContext->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
    for (auto e : elements)
    {
        e->OnPaint(pContext);
    }
    titleBar.ExOnPaint(pContext);
    pContext->EndDraw();
    pSwapChain->Present(1, 0);
    isBusy = false;
}

void Window::Show()
{
    for (auto e : elements)
    {
        e->Align(pContext);
        e->Create(hInstance, hwnd, pContext);
    }
    titleBar.Align(pContext);
    titleBar.Create(hInstance, hwnd, pContext);

    ShowWindow(hwnd, 10);
    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}

void Window::AddElement(Element& rElement, bool enableMouseEvents)
{
    rElement.SetParent(hwnd);
    elements.push_back(&rElement);
    if(enableMouseEvents)
        mouseTracker.AddElement(&rElement);
}

void Window::RemoveElement(const Element& rElement)
{
    std::remove(elements.begin(), elements.end(), &rElement);
    mouseTracker.RemoveElement(&rElement);
    mouseTracker.ClearElementHover();
}

void Window::RemoveElement(Element* pElement)
{
    std::remove(elements.begin(), elements.end(), pElement);
    mouseTracker.RemoveElement(pElement);
    mouseTracker.ClearElementHover();
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

Animator& Window::GetAnimator()
{
    return animator;
}

MouseTracker& Window::GetMouseTracker()
{
    return mouseTracker;
}

const RECT& Window::GetRect() const
{
    RECT r;
    GetWindowRect(hwnd, &r);
    return r;
}

const ID2D1DeviceContext5* Window::GetDeviceContext() const
{
    return pContext;
}

void Window::RequestRedraw()
{
    isRedrawRequested = true;
}

void Window::ClearRedrawRequest()
{
    isRedrawRequested = false;
}

bool Window::IsRedrawRequested()
{
    return isRedrawRequested;
}

void Window::ChangeCursor(LPCSTR cursor) const
{
    hCursor = LoadCursor(NULL, cursor);
    
}

DWORD __stdcall Window::DrawThread(Window* pWindow)
{
    while (true)
    {
        if (pWindow->IsRedrawRequested())
        {
            RedrawWindow(pWindow->GetHwnd(), NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
            pWindow->ClearRedrawRequest();
        }
        ::Sleep(0.01);
    }
}

void Window::CreateDirectXResources()
{
    // This flag adds support for surfaces with a different color channel ordering than the API default.
    // You need it for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    // This array defines the set of DirectX hardware feature levels this app  supports.
    // The ordering is important and you should  preserve it.
    // Don't forget to declare your app's minimum required feature level in its
    // description.  All apps are assumed to support 9.1 unless otherwise stated.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        creationFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &pD3DDevice,
        nullptr,
        &pD3Dcontext
    );

    Microsoft::WRL::ComPtr<IDXGIDevice4> dxgiDevice;
    pD3DDevice.As(&dxgiDevice);

    pFactory->CreateDevice(dxgiDevice.Get(), &pDevice);

    pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pContext);
    

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
    swapChainDesc.Width = 0;
    swapChainDesc.Height = 0;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.Flags = 0;

    Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(&dxgiAdapter);

    Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
    dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

    HRESULT hr =  dxgiFactory->CreateSwapChainForHwnd(
        pD3DDevice.Get(),
        hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &pSwapChain
    );

    dxgiDevice->SetMaximumFrameLatency(1);

    CreateBuffers();

}

void Window::ReleaseSwapChainDependencies()
{

    pContext->SetTarget(nullptr);
    pBackBuffer->Release();
    pDxgiBackBuffer->Release();
    pBitmap->Release();
}

void Window::CreateBuffers()
{
 
    
        pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

        D2D1_BITMAP_PROPERTIES1 bitmapProperties =
            D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
            );

        pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));

        pContext->CreateBitmapFromDxgiSurface(
            pDxgiBackBuffer,
            &bitmapProperties,
            &pBitmap
        );

        
        pContext->SetTarget(pBitmap);
        
}
