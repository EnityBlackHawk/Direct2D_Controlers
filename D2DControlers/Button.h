#pragma once
#include "Element.h"
#include "ElementStyle.h"
#include <d2d1effects.h>
#include <dwrite.h>

#pragma comment(lib, "Dwrite.lib")

class Button : public Element
{

public:

	Button(int x, int y, int width, int height, unsigned char align, const WCHAR text[], ElementStyle style);


	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget) override;

	virtual void Move(int x, int y) override;

	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;

	virtual void OnPaint(ID2D1RenderTarget* pRenderTarget) override;

	virtual void SetOpacity(float opacity) override;

	void SetColorSolidColor(D2D1::ColorF color);

	void SetActivate(bool value);
	bool isActivated() const;

	virtual void Raise(int eventId, void* args) override;


private:

	void CreateResources(ID2D1RenderTarget* pRenderTarget);

	ElementStyle style;
	D2D1_ROUNDED_RECT roundRect;
	ID2D1SolidColorBrush* pSolidColorBrush;
	ID2D1SolidColorBrush* pForegroundBrush;
	ID2D1LinearGradientBrush* pLinearGradientBrush;

	IDWriteFactory* pWriteFactory;
	IDWriteTextFormat* pWriteFormat;
	IDWriteTextLayout* pTextLayout;
	const WCHAR* text;
	float fontSize;
	bool isActivate = true;

};

