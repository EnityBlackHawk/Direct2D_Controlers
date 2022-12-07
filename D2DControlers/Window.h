#pragma once
#include <Windows.h>
#include "Element.h"
#include <vector>
#include "Exception.h"
#include <d2d1_3.h>
#include "MouseTracker.h"
#include "Animator.h"
#include "TitleBar.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <dxgi.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11.lib")

struct MainWindowSyle
{
	COLORREF background;
	COLORREF borderColor;
	COLORREF captionColor;
	COLORREF titleColor;
};

class Window
{

public:
	Window(const char* name, HINSTANCE hInstance, WNDPROC windowProcedure, MainWindowSyle colorStyle, DWORD style = 0, DWORD styleEx = 0);

	LRESULT CALLBACK InternalWindowProc(HWND hwnd, UINT uMgs, WPARAM wParam, LPARAM lParam);

	void Redraw();
	void Show();
	void AddElement(Element& rElement, bool enableMouseEvents = false);
	int GetActualWidth() const;
	int GetActualHeight() const;
	Element* operator[](int index);
	HWND GetHwnd() const;
	Animator& GetAnimator();
	MouseTracker& GetMouseTracker();

	void RequestRedraw();
	void ClearRedrawRequest();
	bool IsRedrawRequested();


	void ChangeCursor(LPCSTR cursor) const;

	static DWORD WINAPI DrawThread(Window* pWindow);

private:

	void CreateDirectXResources();
	void ReleaseSwapChainDependencies();
	void CreateBuffers();

	HWND hwnd;
	HINSTANCE hInstance;
	const char* name;
	MainWindowSyle style;
	std::vector<Element*> elements;


	//ID2D1HwndRenderTarget* pRenderTarget;

	ID2D1Factory7* pFactory;
	ID2D1Device5* pDevice;
	ID2D1DeviceContext5* pContext;
	IDXGISwapChain1* pSwapChain;
	ID2D1Bitmap1* pBitmap;


	// D3D11:
	Microsoft::WRL::ComPtr<ID3D11Device> pD3DDevice;
	ID3D11DeviceContext* pD3Dcontext;
	ID3D11Texture2D* pBackBuffer;
	IDXGISurface* pDxgiBackBuffer;

	MouseTracker mouseTracker;
	Animator animator;

	TitleBar titleBar;

	HANDLE hDrawThread;

	bool isRedrawRequested = false;

	mutable HCURSOR hCursor;
	mutable bool isBusy = false;
	mutable bool isSysNavHover = false;
};

