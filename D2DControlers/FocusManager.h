#pragma once

#include "Element.h"

class FocusManager
{
public:

	FocusManager() = default;

	Element* getElementOnFocus();
	void SetFocus(Element* pElement);
	void RemoveFocus();

private:

	Element* elementOnFocus = nullptr;
};

