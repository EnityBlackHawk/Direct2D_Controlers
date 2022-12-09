#include "ElementStyle.h"

ElementStyle::ElementStyle(unsigned char typeOfBrush, const std::vector<D2D1::ColorF> backgroundColors, D2D1::ColorF foregroundColor, float cornerRadius, D2D1::ColorF borderColor, float borderThickeness):
    typeOfBrush(typeOfBrush), backgroundColors(backgroundColors), foregroundColor(foregroundColor), borderColor(borderColor), borderThickness(borderThickeness), cornerRadius(cornerRadius)
{
}

ElementStyle::ElementStyle(unsigned char typeOfBrush, const std::vector<unsigned int>& backgroundColors, unsigned int foregroundColor, float cornerRadius, unsigned int borderColor, float borderThickness):
    typeOfBrush(typeOfBrush), foregroundColor(foregroundColor), borderColor(borderColor), borderThickness(borderThickness), cornerRadius(cornerRadius)
{
    ElementStyle::backgroundColors.reserve(backgroundColors.size() * sizeof(D2D1::ColorF));
    for (const int color : backgroundColors)
    {
        ElementStyle::backgroundColors.push_back(color);
    }
}

ElementStyle::ElementStyle(unsigned int backgroundColors, unsigned int foregroundColor, float cornerRadius, unsigned int borderColor, float borderThickness):
    typeOfBrush(SOLID_COLOR), foregroundColor(foregroundColor), borderColor(borderColor), borderThickness(borderThickness), cornerRadius(cornerRadius)
{
    ElementStyle::backgroundColors.reserve(sizeof(D2D1::ColorF));
    ElementStyle::backgroundColors.push_back(backgroundColors);
}

unsigned char ElementStyle::getTypeOfBrush() const
{
    return typeOfBrush;
}

const std::vector<D2D1::ColorF>& ElementStyle::getBackgroundColors() const
{
    return backgroundColors;
}

const D2D1::ColorF& ElementStyle::getForegroundColor() const
{
    return foregroundColor;
}

const D2D1::ColorF& ElementStyle::getBorderColor() const
{
    return borderColor;
}

float ElementStyle::getBorderThickness() const
{
    return borderThickness;
}

float ElementStyle::getCornerRadius() const
{
    return cornerRadius;
}
