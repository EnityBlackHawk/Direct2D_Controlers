#pragma once
#include <vector>
#include "Element.h"


class MouseTracker
{

public:

	MouseTracker() = default;

	void AddElement(Element* e);
	
	bool IsInBound(Geometry g);

	void Procedure();

	void SetMousePosition(int x, int y);



	std::vector<Element*> elements;

	int mouseX = 0;
	int mouseY = 0;

};

