#include <Windows.h>
#include <random>
#include "ElementStyle.h"
#include "Window.h"
#include "Frame.h"
#include "Button.h"
#include "Label.h"

Window* window;
Frame* rf;
Button* pB;

std::default_random_engine generator;

int delta = 0;
bool loaded = false;


void animation(float delta)
{
	//pB->SetColorSolidColor(D2D1::ColorF(0xFFFFFF, delta));
	pB->SetOpacity(delta);
	window->RequestRedraw();
}

void opacityIn(float delta)
{
	pB->SetOpacity(delta);
	window->RequestRedraw();
	if (delta > 0.9f)
		pB->SetActivate(true);
}

void moveAni(float delta)
{
	static int indexPOS = 0;

	pB->SetOpacity(delta);
	window->RequestRedraw();
	if (delta < 0.001f)
	{
		std::uniform_int_distribution<int> distributionX(0, window->GetActualWidth() - pB->GetWidth());
		std::uniform_int_distribution<int> distributionY(0, window->GetActualHeight() - pB->GetHeight());
		pB->Move(distributionX(generator), distributionY(generator));
		indexPOS++;
		window->GetMouseTracker().Procedure();
		window->GetAnimator().StartAnimation(3);
	}
	auto i = pB->getOpacity();
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return window->InternalWindowProc(hwnd, uMsg, wParam, lParam);
}

void eventTest(void* sender, void* args)
{
	//SetCursor(((Button*)sender)->getCursor());
	//window->ChangeCursor(IDC_HAND);
	//->GetAnimator().StartAnimation(0);
}

void eventTestOut(void* sender, void* args)
{
	//window->GetAnimator().StartAnimation(1);
	//window->ChangeCursor(IDC_ARROW);
}

void eventClick(void* sender, void* args)
{
	pB->SetAlign(ALIGN_NONE);
	pB->SetActivate(false);
	window->GetAnimator().StartAnimation(2);
}


BOOL WinMain(HINSTANCE hInstance, HINSTANCE hIgnore, PSTR lpCmdLine, INT nCmdShow)
{
	window = new Window("Teste", hInstance, WinProc, { RGB(255,255,255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) });

	ElementStyle style(LINEAR_GRADIENT, { 0x451057, 0xa36110 }, 0, 0, 0);

	Frame f(20, 20, 100, 100, ALIGN_STREACH, style);

	ElementStyle s2(SOLID_COLOR, {D2D1::ColorF(0xFFFFFF, 0.5f)}, NULL, 10, NULL, 0 );
	Button b(AUTO, AUTO, AUTO, 50, ALIGN_CENTER, L"Teste", s2);
	b.margin = { 0, 0, b.GetWidth() / 2 + 5, 0 };

	pB = &b;
	//b.AddEvent(ON_MOUSE_HOVER, eventTest);
	//b.AddEvent(ON_MOUSE_HOVER_OUT, eventTestOut);
	b.AddEvent(ON_MOUSE_HOVER, eventClick);

	Button b2(AUTO, AUTO, AUTO, 50, ALIGN_CENTER, L"Teste2", s2);
	b2.margin = { b.GetWidth() / 2 + 5, 0, 0, 0 };


	Label l(0, 0, AUTO, AUTO, ALIGN_NONE, L"Label", 30, ElementStyle(0, 0xFFFFFF, 0, 0, 0));

	window->AddElement(f);
	window->AddElement(b2, true);
	window->AddElement(b, true);
	window->AddElement(l);

	window->GetAnimator().AddAnimation(animation, 0.5f, 1.0f, 250, &b);
	window->GetAnimator().AddAnimation(animation, 1.0f, 0.5f, 250, &b);
	window->GetAnimator().AddAnimation(moveAni, 1, 0, 100, &b);
	window->GetAnimator().AddAnimation(opacityIn, 0, 1, 500, &b);
	window->Show();

}