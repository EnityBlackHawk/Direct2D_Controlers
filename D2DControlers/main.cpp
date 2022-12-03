#include <Windows.h>
#include "ElementStyle.h"
#include "Window.h"
#include "Frame.h"
#include "Button.h"

Window* window;
Frame* rf;
Button* pB;

int delta = 0;
bool loaded = false;


void animation(float delta)
{
	pB->SetColorSolidColor(D2D1::ColorF(0xFFFFFF, delta));
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
	window->GetAnimator().StartAnimation(0);
}

void eventTestOut(void* sender, void* args)
{
	window->GetAnimator().StartAnimation(1);
	window->ChangeCursor(IDC_ARROW);
}

void eventClick(void* sender, void* args)
{
	//window->GetAnimator().StartAnimation(0);
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
	//ElementStyle s2(LINEAR_GRADIENT, { 0x473dcc, 0x1b165c }, 0xFFFFFF, 10, 0, 0.0f);
	ElementStyle s2(SOLID_COLOR, {D2D1::ColorF(0xFFFFFF, 0.5f)}, NULL, 10, NULL, 0 );
	Button b(0, 0, 255, 50, ALIGN_NONE, L"Teste", s2);

	pB = &b;
	b.AddEvent(ON_MOUSE_HOVER, eventTest);
	b.AddEvent(ON_MOUSE_HOVER_OUT, eventTestOut);
	b.AddEvent(ON_CLICK, eventClick);

	window->AddElement(f);
	window->AddElement(b, true);

	window->GetAnimator().AddAnimation(animation, 0.5f, 1.0f, 250, &b);
	window->GetAnimator().AddAnimation(animation, 1.0f, 0.5f, 250, &b);
	window->Show();

}