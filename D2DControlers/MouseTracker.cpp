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

void MouseTracker::Procedure()
{
	int lastX = 0;
	int lastY = 0;

	static Element* elementOnHover = nullptr;
	static Geometry onHoverElementGometry = {};

	if (elementOnHover && !IsInBound(onHoverElementGometry))
	{
		elementOnHover->Raise(ON_MOUSE_HOVER_OUT, nullptr);
		elementOnHover = nullptr;
	}
	else if (elementOnHover && IsInBound(onHoverElementGometry))
		return;
	
		


	if (lastX != mouseX && lastY != mouseY)
	{
		for (Element* e : elements)
		{
			if (IsInBound(e->GetGeometry()))
			{
				elementOnHover = e;
				onHoverElementGometry = e->GetGeometry();

				e->Raise(ON_MOUSE_HOVER, nullptr);
				break;
			}
		}
		lastX = mouseX;
		lastY = mouseY;
	}
	
}

void MouseTracker::SetMousePosition(int x, int y)
{
	mouseX = x;
	mouseY = y;
}
