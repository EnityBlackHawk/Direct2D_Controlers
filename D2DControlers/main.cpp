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

DWORD WINAPI AnimationThread()
{
	while (delta <= 300)
	{
		rf->Move(delta, delta);
		window->Redraw();
		delta++;
	}
	return 0;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PAINT)
	{
		/*if(!loaded)
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AnimationThread, nullptr, NULL, nullptr);
		loaded = true;*/
	}

	


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

	ElementStyle style(LINEAR_GRADIENT, { 0x451057, 0xa36110 }, 0, 0, 0);

	Frame f(20, 20, 100, 100, ALIGN_STREACH, style);

	/*Frame f2(AUTO, AUTO, 50, 50, ALIGN_CENTER, ElementStyle(0xFF0000, 0x0, 0, 0, 0));
	f2.AddEvent(ON_MOUSE_HOVER, eventTest);
	f2.AddEvent(ON_MOUSE_HOVER_OUT, eventTestOut);
	rf = &f2;*/
	ElementStyle s2(LINEAR_GRADIENT, { 0x473dcc, 0x1b165c }, 0xFFFFFF, 10, 0, 0.0f);
	Button b(AUTO, AUTO, 255, 50, ALIGN_CENTER, L"Teste", s2);


	window->AddElement(f);
	window->AddElement(b);
	//window->AddElement(f2, true);
	//window->GetAnimator().AddAnimation(animation, 0, 300, 50 );
	window->Show();

}