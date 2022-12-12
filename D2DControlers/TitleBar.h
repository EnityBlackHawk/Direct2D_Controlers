#pragma once
#include "Element.h"
#include <d2d1_3.h>
#include <wincodec.h>
#include "ElementStyle.h"
#include "Label.h"

#pragma comment(lib, "windowscodecs.lib")

class TitleBar : public Element
{
public:
	TitleBar(int x, int y, int width, int height, unsigned char align, ElementStyle style);

	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget) override;

	virtual void SetOpacity(float opacity) override;

	virtual void Move(int x, int y) override;

	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;

	virtual void OnPaint(ID2D1RenderTarget* pRenderTarget) override;

	void ExOnPaint(ID2D1DeviceContext5* pRenderTarger);

	void hoverCloseButton();
	void hoverMaxButton();
	void hoverMinButton();
	void ResetHover();

	void CreateResources(ID2D1RenderTarget* pRenderTarger);

	D2D1_ROUNDED_RECT roundRect;
	Label text;

	ID2D1SolidColorBrush* closeButtonBrush;
	ID2D1SolidColorBrush* maxButtonBrush;
	ID2D1SolidColorBrush* minButtonBrush;

	ID2D1SolidColorBrush* pSolidColorBrush;

	IWICImagingFactory* pImagingFactory;
	ID2D1Bitmap* pBitmap;
	ID2D1SvgDocument* pSvgMax;
	ID2D1SvgDocument* pSvgMin;


	ElementStyle style;
};

