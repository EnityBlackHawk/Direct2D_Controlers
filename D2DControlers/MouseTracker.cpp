#include "MouseTracker.h"

void MouseTracker::AddElement(Element* e)
{
	elements.push_back(e);
}

bool MouseTracker::IsInBound(Geometry g)
{
	if (mouseX > g.posX && mouseX < g.posX + g.width && mouseY > g.posY && mouseY < g.posY + g.heght)
		return true;
	return false;
}

void MouseTracker::Procedure(MouseTracker& mt)
{
	int lastX = 0;
	int lastY = 0;

	static Element* elementOnHover = nullptr;
	static Geometry onHoverElementGometry = {};

	if (elementOnHover && !mt.IsInBound(onHoverElementGometry))
	{
		elementOnHover->Raise(ON_MOUSE_HOVER_OUT, nullptr);
		elementOnHover = nullptr;
	}
	else if (elementOnHover && mt.IsInBound(onHoverElementGometry))
		return;


	if (lastX != mt.mouseX && lastY != mt.mouseY)
	{
		for (Element* e : mt.elements)
		{
			if (mt.IsInBound(e->GetGeometry()))
			{
				elementOnHover = e;
				onHoverElementGometry = e->GetGeometry();

				e->Raise(ON_MOUSE_HOVER, nullptr);
				break;
			}
		}
		lastX = mt.mouseX;
		lastY = mt.mouseY;
	}
	
}

void MouseTracker::SetMousePosition(int x, int y)
{
	mouseX = x;
	mouseY = y;
}
