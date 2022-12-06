#pragma once
#include <Windows.h>
#include "Element.h"
#include <vector>
#include "Exception.h"
#include <d2d1.h>
#include "MouseTracker.h"
#include "Animator.h"
#include "TitleBar.h"

#pragma comment(lib, "d2d1")

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


	HWND hwnd;
	HINSTANCE hInstance;
	const char* name;
	MainWindowSyle style;
	std::vector<Element*> elements;


	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1Factory* pFactory;
	MouseTracker mouseTracker;
	Animator animator;

	TitleBar titleBar;

	HANDLE hDrawThread;

	bool isRedrawRequested = false;

	mutable HCURSOR hCursor;
	mutable bool isBusy = false;
};

