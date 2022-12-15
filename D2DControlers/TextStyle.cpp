#include "TextStyle.h"

TextStyle::TextStyle():
	fontSize(0), fontFamily(L"Segoe UI"), fontWeight(REGULAR), fontStyle(NORMAL)
{
}

TextStyle::TextStyle(float fontSize, const wchar_t* fontFamily, FontWeight fontWeight, FontStyle fontStyle):
	fontSize(fontSize), fontFamily(fontFamily), fontWeight(fontWeight), fontStyle(fontStyle)
{
}

float TextStyle::GetFontSize()
{
	return fontSize;
}

const wchar_t* TextStyle::GetFontFamily()
{
	return fontFamily;
}

int TextStyle::GetFontWeight()
{
	return fontWeight;
}

int TextStyle::GetFontStyle()
{
	return fontStyle;
}
