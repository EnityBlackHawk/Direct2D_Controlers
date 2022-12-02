#pragma once
#include <d2d1.h>

#pragma comment (lib, "d2d1")


class BaseWindow
{
protected:

	static LRESULT CALLBACK DummyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			BaseWindow* pT = reinterpret_cast<BaseWindow*>(cs->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&BaseWindow::ProcStart);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pT);
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	HWND CreateWnd(int x, int y, int width, int height, const char* className, HINSTANCE hInstance, DWORD styles, HWND hParent, BaseWindow* pBaseWindow)
	{
		WNDCLASS wc = {};

		wc.lpszClassName = className;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = BaseWindow::DummyProc;

		RegisterClass(&wc);

		HWND hwnd = CreateWindowEx(
			0,
			className,
			NULL,
			styles,
			x,
			y,
			width,
			height,
			hParent,
			NULL,
			hInstance,
			pBaseWindow
		);

		return hwnd;
	}

	static LRESULT ProcStart(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		BaseWindow* pT = reinterpret_cast<BaseWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pT->HandleMessage(uMsg, wParam, lParam);
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

};

