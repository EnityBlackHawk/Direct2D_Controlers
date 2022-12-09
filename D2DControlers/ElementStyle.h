#pragma once
#include <vector>
#include <d2d1.h>

#define SOLID_COLOR 0
#define LINEAR_GRADIENT 1


class ElementStyle
{
public:
	ElementStyle(unsigned char typeOfBrush, const std::vector<D2D1::ColorF> backgroundColors, D2D1::ColorF foregroundColor, float cornerRadius, D2D1::ColorF borderColor, float borderThickness);
	ElementStyle(unsigned char typeOfBrush, const std::vector<unsigned int>& backgroundColors, unsigned int foregroundColor, float cornerRadius, unsigned int borderColor = 0, float borderThickness = 0);
	ElementStyle(unsigned int backgroundColors, unsigned int foregroundColor, float cornerRadius, unsigned int borderColor, float borderThickness);

	unsigned char getTypeOfBrush() const;
	const std::vector<D2D1::ColorF>& getBackgroundColors() const;
	const D2D1::ColorF& getForegroundColor() const;
	const D2D1::ColorF& getBorderColor() const;
	float getBorderThickness() const;
	float getCornerRadius() const;

private:

	unsigned char typeOfBrush;
	std::vector<D2D1::ColorF> backgroundColors;
	D2D1::ColorF foregroundColor;
	float cornerRadius;
	D2D1::ColorF borderColor;
	float borderThickness;
};

