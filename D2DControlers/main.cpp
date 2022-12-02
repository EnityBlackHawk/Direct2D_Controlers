#include <Windows.h>
#include "ElementStyle.h"
#include "Window.h"
#include "Frame.h"

Window* window;
Frame* rf;

int delta = 0;
bool loaded = false;

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return window->InternalWindowProc(hwnd, uMsg, wParam, lParam);
}

void eventTest(void* sender, void* args)
{
	((Frame*)sender)->SetColor(D2D1::ColorF(0x00FF00, 0.5f));
	window->Redraw();
}

void eventTestOut(void* sender, void* args)
{
	((Frame*)sender)->SetColor(D2D1::ColorF(0xFF0000, 1));
	window->Redraw();
}


BOOL WinMain(HINSTANCE hInstance, HINSTANCE hIgnore, PSTR lpCmdLine, INT nCmdShow)
{
	window = new Window("Teste", hInstance, WinProc, { RGB(255,255,255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) });

	ElementStyle style(0xFF0000, 0, 10, 0, 0);


	Frame f(20, 20, 100, 100, ALIGN_CENTER, style);
	rf = &f;
	f.AddEvent(ON_MOUSE_HOVER, eventTest);
	f.AddEvent(ON_MOUSE_HOVER_OUT, eventTestOut);
	window->AddElement(f);
	window->Show();

}