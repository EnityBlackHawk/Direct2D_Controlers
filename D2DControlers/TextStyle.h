#pragma once

enum FontWeight
{
	THIN = 100,
	EXTRA_LIGHT = 200,
	LIGHT = 300,
	SEMI_LIGHT = 350,
	REGULAR = 400,
	MEDIUM = 500,
	SEMI_BOLD = 600,
	BOLD = 700,
	ULTRA_BOLD = 800,
	HEAVY = 900,
	ULTRA_HEAVY = 950
};

enum FontStyle
{
	NORMAL = 0,
	OBLIQUE = 1,
	ITALIC = 2
};

class TextStyle
{
public:

	TextStyle();
	TextStyle(float fontSize, const wchar_t* fontFamily, FontWeight fontWeight, FontStyle fontStyle);


	float GetFontSize();
	const wchar_t* GetFontFamily();
	int GetFontWeight();
	int GetFontStyle();



private:
	float fontSize;
	const wchar_t* fontFamily;
	FontWeight fontWeight;
	FontStyle fontStyle;

};

