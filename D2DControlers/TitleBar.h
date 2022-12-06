#pragma once
#include "Element.h"
#include <d2d1.h>
#include "ElementStyle.h"

class TitleBar : public Element
{
public:
	TitleBar(int x, int y, int width, int height, unsigned char align, ElementStyle style);

	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1HwndRenderTarget* pRenderTarget) override;

	virtual void SetOpacity(float opacity) override;

	virtual void Move(int x, int y) override;

	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;

	virtual void OnPaint(ID2D1HwndRenderTarget* pRenderTarget) override;

private:

	void CreateResources(ID2D1HwndRenderTarget* pRenderTarger);

	D2D1_ROUNDED_RECT roundRect;
	ID2D1SolidColorBrush* pSolidColorBrush;
	ElementStyle style;
};

