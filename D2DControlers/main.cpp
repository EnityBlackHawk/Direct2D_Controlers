#include <Windows.h>
#include <random>
#include "ElementStyle.h"
#include "Window.h"
#include "Frame.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"

Window* window;
Frame* rf;
Button* pB;
Button* pY;

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
	//if (delta > 0.9f)
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
		window->GetAnimator().StartAnimation(1);
	}
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(window)
		return window->InternalWindowProc(hwnd, uMsg, wParam, lParam);
}

void eventClick(void* sender, void* args)
{
	pB->SetAlign(ALIGN_NONE);
	pB->SetActivate(false);
	window->GetAnimator().StartAnimation(0);
	/*SetCursorPos(
		(pY->GetPosX() + pY->GetWidth() / 2) + window->GetRect().left,
		(pY->GetPosY() + pY->GetHeight() / 2) + window->GetRect().top
	);*/

}

void moveWithMouse(void* sender, void* args)
{
	coord* c = (coord*)args;
	pB->Move(c->x, c->y);
	window->RequestRedraw();
}

BOOL WinMain(HINSTANCE hInstance, HINSTANCE hIgnore, PSTR lpCmdLine, INT nCmdShow)
{
	window = new Window("Teste", hInstance, WinProc, { RGB(255,255,255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) });
	
	Image i(AUTO, AUTO, AUTO, AUTO, ALIGN_STREACH, L"E:\\Downloads\\e2272d6ba9565d84b624496d1d54b6ccf2b8d4df.jpg");


	ElementStyle style(LINEAR_GRADIENT, { 0x451057, 0xa36110 }, 0, 0, 0);
	
	Frame f(20, 20, 100, 100, ALIGN_STREACH, style);

	ElementStyle s2(SOLID_COLOR, {D2D1::ColorF(0xFFFFFF, 0.75f)}, NULL, 10, NULL, 0 );
	Button b(100, 100, AUTO, 50, ALIGN_NONE, L"Teste", s2);
	b.margin = { 0, 0, b.GetWidth() / 2 + 5, 0 };

	pB = &b;

	b.AddEvent(ON_MOUSE_HOVER, eventClick);

	Button b2(AUTO, AUTO, AUTO, 50, ALIGN_CENTER, L"Teste2", s2);
	b2.margin = { b.GetWidth() / 2 + 5, 0, 0, 0 };
	pY = &b2;

	Label l(0, 0, AUTO, AUTO, ALIGN_CENTER, L"Label", 30, ElementStyle(0, 0xFFFFFF, 0, 0, 0));
	l.margin = { 0, 0, 0, b2.GetHeight() + 10 };

	window->AddElement(i);
	window->AddElement(f);
	window->AddElement(l);
	window->AddElement(b2, true);
	window->AddElement(b, true);

	window->GetAnimator().AddAnimation(moveAni, 1, 0, 100, &b);
	window->GetAnimator().AddAnimation(opacityIn, 0, 1, 500, &b);


	window->Show();

}