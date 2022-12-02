#pragma once
#include <vector>
#include "Element.h"

class IParentable
{
public:
	/// <summary>
	/// Draw all the child windows
	/// </summary>
	/// <param name="hInstance">Application HINSTANCE</param>
	virtual void LoadChilds(HINSTANCE hInstance) = 0;
	/// <summary>
	/// Add a child
	/// </summary>
	/// <param name="rElement">Element reference</param>
	virtual void AddChild(Element& rElement) = 0;
	/// <summary>
	/// Remove child from parent
	/// </summary>
	/// <param name="target">Element pointer</param>
	virtual void RemoveChild(Element* ptarget) = 0;
	/// <summary>
	/// Remove child from parent
	/// </summary>
	/// <param name="index">index of the Element</param>
	virtual void RemoveChild(int index) = 0;

	virtual std::vector<Element*>GetChildElements()
	{
		return elements;
	}
protected:
	std::vector<Element*> elements;
};