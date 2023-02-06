#pragma once
#include <vector>
#include "Element.h"

struct coord
{
	int x;
	int y;
};

class MouseTracker
{

public:

	MouseTracker() = default;

	void AddElement(Element* e);

	void RemoveElement(const Element* e);
	
	bool IsInBound(Geometry g);

	void Procedure();

	void LeftClickProcedure();

	void SetMousePosition(int x, int y);

	void ClearElementHover();

	Element* getElementOnHover();
	

private:


	std::vector<Element*> elements;

	int mouseX = 0;
	int mouseY = 0;

	Element* elementOnHover = nullptr;
	Geometry onHoverGeometry = {};

};

