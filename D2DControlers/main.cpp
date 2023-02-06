#include <Windows.h>
#include <random>
#include <vector>
#include "ElementStyle.h"
#include "Window.h"
#include "Frame.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"
#include "Grid.h"

Window* window;


void focus(void* sender, void* args)
{
	OutputDebugString("Focus\n");
}

void lostFocus(void* sender, void* args)
{
	OutputDebugString("Lost Focus\n");
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(window)
		return window->InternalWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL WinMain(HINSTANCE hInstance, HINSTANCE hIgnore, PSTR lpCmdLine, INT nCmdShow)
{
	window = new Window("Teste", hInstance, WinProc, { RGB(0,0,0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) });
	
	Button b(0, 0, AUTO, 200, ALIGN_CENTER, L"Teste", ElementStyle(0x0, 0xFFFFFF, 10, 0, 0));
	b.AddEvent(ON_FOCUS, focus);
	b.AddEvent(LOST_FOCUS, lostFocus);

	window->AddElement(b, true);

	window->Show();

}