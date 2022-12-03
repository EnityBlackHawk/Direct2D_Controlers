#include <Windows.h>
#include "ElementStyle.h"
#include "Window.h"
#include "Frame.h"
#include "Button.h"

Window* window;
Frame* rf;

int delta = 0;
bool loaded = false;


void animation(float delta)
{
	rf->Move(delta, delta);
	window->Redraw();
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return window->InternalWindowProc(hwnd, uMsg, wParam, lParam);
}

void eventTest(void* sender, void* args)
{
	//SetCursor(((Button*)sender)->getCursor());
	window->ChangeCursor(IDC_HAND);
}

void eventTestOut(void* sender, void* args)
{
	window->ChangeCursor(IDC_ARROW);
}



BOOL WinMain(HINSTANCE hInstance, HINSTANCE hIgnore, PSTR lpCmdLine, INT nCmdShow)
{
	window = new Window("Teste", hInstance, WinProc, { RGB(255,255,255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) });

	ElementStyle style(LINEAR_GRADIENT, { 0x451057, 0xa36110 }, 0, 0, 0);

	Frame f(20, 20, 100, 100, ALIGN_STREACH, style);

	/*Frame f2(AUTO, AUTO, 50, 50, ALIGN_CENTER, ElementStyle(0xFF0000, 0x0, 0, 0, 0));
	f2.AddEvent(ON_MOUSE_HOVER, eventTest);
	f2.AddEvent(ON_MOUSE_HOVER_OUT, eventTestOut);
	rf = &f2;*/
	ElementStyle s2(LINEAR_GRADIENT, { 0x473dcc, 0x1b165c }, 0xFFFFFF, 10, 0, 0.0f);
	Button b(AUTO, AUTO, 255, 50, ALIGN_CENTER, L"Teste", s2);

	b.AddEvent(ON_MOUSE_HOVER, eventTest);
	b.AddEvent(ON_MOUSE_HOVER_OUT, eventTestOut);

	window->AddElement(f);
	window->AddElement(b, true);
	//window->AddElement(f2, true);
	//window->GetAnimator().AddAnimation(animation, 0, 300, 50 );
	window->Show();

}