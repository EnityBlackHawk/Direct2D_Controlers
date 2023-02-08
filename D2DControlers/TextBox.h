#pragma once
#include "Element.h"
#include "ElementStyle.h"
#include <sstream>

#include <dwrite.h>
#pragma comment(lib, "Dwrite.lib")

class TextBox : public Element
{
public:

	TextBox(int x, int y, int width, int height, unsigned char align, ElementStyle style);

	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget) override;
	virtual void SetOpacity(float opacity) override;
	virtual float GetOpacity() override;
	virtual void Move(int x, int y) override;
	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;
	virtual void OnPaint(ID2D1RenderTarget* pRenderTarget) override;

	void SetText(const WCHAR* text);
	void InputChar(const wchar_t* character);
	void RemoveChar();

	static void OnTypingEvent(void* sender, void* args);
	static void OnFocusEvent(void* sender, void* args);
	static void OnLostFocusEvent(void* sender, void* args);

private:


	void CreateResouces(ID2D1RenderTarget* pRenderTarget);

	ElementStyle style;
	D2D1_ROUNDED_RECT roundRect;
	ID2D1SolidColorBrush* pSolidColorBrush;
	ID2D1SolidColorBrush* pForegroundBrush;

	IDWriteFactory* pWriteFactory;
	IDWriteTextFormat* pWriteFormat;
	IDWriteTextLayout* pTextLayout;

	int size = 0;
	std::wstring text;

	float fontSize;
};

