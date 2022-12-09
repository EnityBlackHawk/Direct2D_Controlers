#pragma once
#include "Element.h"
#include <wincodec.h>


class Image : public Element
{

public:

	Image(int x, int y, int width,int height, unsigned char align, const WCHAR* filePath);

	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget) override;

	virtual void SetOpacity(float opacity) override;

	virtual void Move(int x, int y) override;

	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;

	virtual void OnPaint(ID2D1RenderTarget* pRenderTarget) override;


private:
	void CreateResources(ID2D1RenderTarget* pRenderTaget);
	float opacity = 1;
	const WCHAR* imagePath;

	IWICImagingFactory2* pImagingFactory;
	ID2D1Bitmap* pBitmap;


};

