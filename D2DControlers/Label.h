#pragma once
#include "Element.h"
#include "ElementStyle.h"
#include <dwrite.h>

#pragma comment(lib, "Dwrite.lib")

class Label : public Element
{
public:
	Label(int x, int y, int width, int height, unsigned char align, const WCHAR text[], float fontSize, ElementStyle style);

	void SetText(const WCHAR text[]);



	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1HwndRenderTarget* pRenderTarget) override;

	virtual void SetOpacity(float opacity) override;

	virtual void Move(int x, int y) override;

	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;

	virtual void OnPaint(ID2D1HwndRenderTarget* pRenderTarget) override;

	static int getTextSize(const WCHAR* text);

private:

	void CreateResources(ID2D1HwndRenderTarget* pRenderTarget);

	ElementStyle style;
	ID2D1SolidColorBrush* pSolidColorBrush;
	IDWriteFactory* pWriteFactory;
	IDWriteTextFormat* pWriteFormat;
	IDWriteTextLayout* pTextLayout;
	const WCHAR* text;
	float fontSize;
};

