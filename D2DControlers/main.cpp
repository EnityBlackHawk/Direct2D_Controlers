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
#include "TextBox.h"

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
	
	TextBox tb(0, 0, 120, 30, ALIGN_CENTER, ElementStyle(0x444444, 0xFFFFFF, 10.0f, 0, 0));

	window->AddElement(tb, true);

	window->Show();

}