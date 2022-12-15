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
Frame* rf;
Frame* pFrameBlack;
Button* pB;
Button* pY;
constexpr int hearthsCount = 50;
std::vector<Image> hearths = std::vector<Image>();

HANDLE shakeThread = NULL;
HANDLE mouseMoveThread = NULL;

// Animations:
int Index_onNoButtonsAnimation;
int Index_onNoBackgroundAnimation;
int Index_onYesBackgroundAnimation;


std::default_random_engine generator;

int delta = 0;
bool loaded = false;


void frameOut(float delta)
{
	rf->SetOpacity(delta);
}

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
	window->GetAnimator().StartAnimation(2);
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

// Shake animation:
DWORD WINAPI ShackWindow()
{
	RECT originalPos = window->GetRect();
	int intensity = 0;

	while (true)
	{
		std::uniform_int_distribution<int> distributionX(originalPos.left - intensity, originalPos.left + intensity);
		std::uniform_int_distribution<int> distributionY(originalPos.top - intensity, originalPos.top + intensity);

		SetWindowPos(window->GetHwnd(), NULL, distributionX(generator), distributionY(generator), 0, 0, SWP_NOSIZE);
		intensity = intensity < 5 ? intensity + 1 : intensity;
		::Sleep(10);
	}
}


DWORD WINAPI mousePointerMove()
{
	::Sleep(250);
	int deltaX = 0;
	while (true)
	{
		int targertX = (pY->GetPosX() + pY->GetWidth() / 2) + window->GetRect().left;
		int targetY = (pY->GetPosY() + pY->GetHeight() / 2) + window->GetRect().top;

		POINT p;
		GetCursorPos(&p);
		
		if (p.x < targertX)
		{
			deltaX += 1;
			SetCursorPos(p.x + deltaX, targetY);
		}
		else
		{
			deltaX = 0;
			while (true)
			{
				SetCursorPos(targertX, targetY);
				::Sleep(50);
			}
		}
		::Sleep(15);
	}
}

void WINAPI imageTeste(Image* imageC)
{

	std::uniform_int_distribution<int> distX(-3, 3);
	std::uniform_int_distribution<int> distY(-3, 3);
	float direcX = distX(generator);
	float direcY = distY(generator);
	const float targetRight = window->GetActualWidth() - (imageC->GetWidth());
	const float targetBottom = window->GetActualHeight() - (imageC->GetHeight());
	while (true)
	{
		imageC->Translate(direcX, direcY);
		window->RequestRedraw();

		if (imageC->GetPosX() >= targetRight)
		{
			direcX = -abs(distX(generator));
			direcY = distY(generator);
		}
		else if (imageC->GetPosY() >= targetBottom)
		{
			direcX = distX(generator);
			direcY = -abs(distY(generator));
		}
		else if (imageC->GetPosX() <= 0)
		{
			direcX = abs(distX(generator));
			direcY = distY(generator);
		}
		else if (imageC->GetPosY() <= 0)
		{
			direcX = distX(generator);
			direcY = abs(distY(generator));
		}
		::Sleep(2.5);
	}
}

// EVENTS
void noOnHover_once(void* sender, void* args)
{
	pB->RemoveEvent(ON_MOUSE_HOVER);
	window->GetAnimator().StartAnimation(Index_onNoButtonsAnimation);
	window->GetAnimator().StartAnimation(Index_onNoBackgroundAnimation);

	shakeThread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)ShackWindow, nullptr, NULL, nullptr);
	mouseMoveThread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)mousePointerMove, nullptr, NULL, nullptr);
	
}

void onYesClick(void* sender, void* args)
{
	const int x = pY->GetPosX() + pY->GetWidth() / 2;
	const int y = pY->GetPosY() + pY->GetHeight() / 2;

	for (int i = 0; i < hearthsCount; i++)
		hearths[i].SetPos(x, y);



	if (shakeThread)
		TerminateThread(shakeThread, 0);
	if (mouseMoveThread)
		TerminateThread(mouseMoveThread, 0);
	window->GetAnimator().StartAnimation(Index_onYesBackgroundAnimation);
	
	for(int i = 0; i < hearthsCount; i++)
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)imageTeste, &hearths[i], NULL, nullptr);
}

// Animations
void onNoButtonsAnimation(float delta)
{
	pB->Move(pB->GetPosX() - delta, pB->GetPosY());
	pY->Move(pY->GetPosX() + delta, pY->GetPosY());
}

void onNoBackgroundAnimation(float delta)
{
	rf->SetOpacity(delta);
	window->RequestRedraw();
}



void onYesBackgroundAnimation(float delta)
{
	pFrameBlack->SetOpacity(delta);
	window->RequestRedraw();
}

BOOL WinMain(HINSTANCE hInstance, HINSTANCE hIgnore, PSTR lpCmdLine, INT nCmdShow)
{
	window = new Window("Teste", hInstance, WinProc, { RGB(0,0,0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) });
	
	
	Image i(AUTO, AUTO, 500, AUTO, ALIGN_CENTER, L"E:\\cpp\\D2DControlers\\D2DControlers\\img\\bg1.jpg");
	for (int i = 0; i < hearthsCount; i++)
	{
		hearths.push_back(Image(500, 500, 60, AUTO, ALIGN_NONE, L"E:\\cpp\\D2DControlers\\D2DControlers\\img\\cora.png"));
	}
	//Image iC(50, 50, 60, AUTO, ALIGN_NONE, L"E:\\cpp\\D2DControlers\\D2DControlers\\img\\cora.png");
	

	Grid g(AUTO, AUTO, AUTO, AUTO, ALIGN_STREACH);
	g.DefineColumn(500, 500);
	g.DefineRow(100, 100);
	g.AddElement(0, 0, i);
	
	ElementStyle styleFrame(LINEAR_GRADIENT, { 0x451057, 0xa36110 }, 0, 0, 0);
	
	Frame f(20, 20, 100, 100, ALIGN_STREACH, styleFrame);
	rf = &f;

	Frame frameBlack(AUTO, AUTO, AUTO, AUTO, ALIGN_STREACH, ElementStyle(0, 0, 0, 0, 0));
	pFrameBlack = &frameBlack;


	ElementStyle s2(SOLID_COLOR, {D2D1::ColorF(0xFFFFFF, 0.75f)}, NULL, 10, NULL, 0 );
	Button b(AUTO, AUTO, AUTO, 50, ALIGN_CENTER, L"Não", s2);
	b.margin = { 0, 0, b.GetWidth() / 2 + 5, 0 };

	b.AddEvent(ON_MOUSE_HOVER, noOnHover_once);

	pB = &b;

	

	Button b2(AUTO, AUTO, AUTO, 50, ALIGN_CENTER, L"Sim", s2);
	b2.margin = { b.GetWidth() / 2 + 5, 0, 0, 0 };
	b2.AddEvent(ON_CLICK, onYesClick);
	pY = &b2;

	Label l(0, 0, AUTO, AUTO, ALIGN_CENTER, L"Label", 30, ElementStyle(0, 0xFFFFFF, 0, 0, 0));
	l.margin = { 0, 0, 0, b2.GetHeight() + 10 };

	window->AddElement(g);
	for (int i = 0; i < hearthsCount; i++)
		window->AddElement(hearths[i]);
	window->AddElement(frameBlack);
	window->AddElement(f);
	window->AddElement(l);
	window->AddElement(b2, true);
	window->AddElement(b, true);


	/*window->GetAnimator().AddAnimation(moveAni, 1, 0, 250, &b);
	window->GetAnimator().AddAnimation(opacityIn, 0, 1, 500, &b);
	window->GetAnimator().AddAnimation(frameOut, 1, 0, 500, &f);*/

	auto& animator = window->GetAnimator();
	Index_onNoButtonsAnimation = animator.AddAnimation(onNoButtonsAnimation, 0, 20, 500, &b);
	Index_onNoBackgroundAnimation = animator.AddAnimation(onNoBackgroundAnimation, 1, 0, 1000, &f);
	Index_onYesBackgroundAnimation = animator.AddAnimation(onYesBackgroundAnimation, 1, 0, 1000, nullptr);

	window->Show();

}