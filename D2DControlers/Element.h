#pragma once
#include <Windows.h>
#include <unordered_map>
#include <d2d1.h>

#pragma comment(lib, "d2d1")

#define ALIGN_NONE 0
#define ALIGN_CENTER 2
#define ALIGN_STREACH 4
#define ALIGN_VERTICAL_STREACH 5
#define ALIGN_HORIZONTAL_STREACH 6

#define AUTO -1
#define RESIDUAL_SPACE -2

// Events macros
#define ON_MOUSE_HOVER 1
#define ON_MOUSE_HOVER_OUT 2
#define ON_CLICK 3

typedef void(*EVENT)(void* sender, void* args);


struct Margin
{
	int left;
	int top;
	int right;
	int bottom;
};

struct Geometry
{
	int posX;
	int posY;
	int width;
	int heght;
};

class Element
{
public:

	Element(int x, int y, int width, int height, unsigned char align):
		posX(x), posY(y), width(width), height(height), align(align)
	{
	}

	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1HwndRenderTarget* pRenderTarget) = 0;

	Geometry GetGeometry()
	{
		Geometry g = {posX, posY, width, height};
		return g;
	}

	void AddEvent(int eventId, EVENT action)
	{
		events[eventId] = action;
	}

	void Raise(int eventId, void* args)
	{
		if (events.contains(eventId))
			events[eventId](this, args);
	}

	int GetActualWidth()
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		return rect.right - rect.left;
	}

	int GetActualHeight()
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		return rect.bottom - rect.top;
	}

	int GetPosX()
	{
		return posX;
	}
	int GetPosY()
	{
		return posY;
	}
	int GetWidth()
	{
		return width;
	}
	int GetHeight()
	{
		return height;
	}
	HWND GetHwnd()
	{
		return hwnd;
	}
	unsigned char GetAlign()
	{
		return align;
	}
	void SetAlign(unsigned char newAlign)
	{
		align = newAlign;
	}

	
	virtual void Move(int x, int y) = 0;

	/// <summary>
	/// Draw window
	/// </summary>
	/// <param name="hParent">Parent's HWND</param>
	/// <param name="hInstance">Application HINSTANCE</param>
	/// <returns>Window HWND</returns>
	virtual HWND Show(HWND hParent, HINSTANCE hInstance) = 0;


	virtual void OnSizeChanged(ID2D1HwndRenderTarget* pRenderTarget)
	{
		Align(pRenderTarget);
		Move(posX, posY);
	}


	void Align(ID2D1HwndRenderTarget* pRenderTarget)
	{
		if (align == ALIGN_NONE)
			return;

		else if (align == ALIGN_CENTER)
		{

			D2D1_SIZE_F size = pRenderTarget->GetSize();

			auto newX = (size.width / 2) - (width / 2);
			auto newY = ((size.height / 2) - (height / 2));

			posX = newX - margin.right + margin.left;
			posY = newY - margin.bottom + margin.top;
		}
		else if (align == ALIGN_STREACH)
		{
			D2D1_SIZE_F size = pRenderTarget->GetSize();

			width = size.width - (margin.left + margin.right);
			height = size.height - (margin.bottom + margin.top);

			posX = margin.left;
			posY = margin.top;

		}

		else if (align == ALIGN_VERTICAL_STREACH)
		{
			RECT rect;
			GetWindowRect(hParent, &rect);
			auto parentHeight = rect.bottom - rect.top;

			height = parentHeight - (margin.bottom + margin.top);
			posY = margin.top;

		}
		
	}

	void SetParent(HWND hParent)
	{
		Element::hParent = hParent;
	}

	virtual void OnPaint(ID2D1HwndRenderTarget* pRenderTarget) = 0;


	/// <summary>
	/// Redraw the element. This function will mainly call the WS_PAINT
	/// </summary>
	void Reload()
	{
		if (!hwnd) return;
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
	}

	HWND GetPatentHwnd()
	{
		return hParent;
	}

	Margin margin = { 0,0,0,0 };

protected:
	int posX;
	int posY;
	int width;
	int height;
	HWND hwnd;
	HWND hParent;
	unsigned char align;
	ID2D1HwndRenderTarget* pRenderTarget;
	std::unordered_map<int, EVENT> events;
};

