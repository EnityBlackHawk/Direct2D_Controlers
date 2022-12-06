#pragma once
#include "Element.h"
#include "ElementStyle.h"
#include "BaseWindow.h"

class Frame : public Element, public BaseWindow
{
public:

	Frame(int x, int y, int width, int height, unsigned char align, ElementStyle style);


	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget) override;

	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;

	virtual void SetOpacity(float) override {}

	void OnPaint(ID2D1RenderTarget* pRenderTarget) override;
	void CreateResources(ID2D1RenderTarget* pRenderTarget);

	void Move(int x, int y) override;
	void SetColor(D2D1::ColorF color);

private:

	// Inherited via BaseWindow
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	ElementStyle style;
	D2D1_ROUNDED_RECT roundRect;
	ID2D1SolidColorBrush* pSolidColorBrush;
	ID2D1LinearGradientBrush* pLinearGradientBrush;
	HCURSOR handCursor;
};

