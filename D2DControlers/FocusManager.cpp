#include "FocusManager.h"

Element* FocusManager::getElementOnFocus()
{
	return elementOnFocus;
}

void FocusManager::SetFocus(Element* pElement)
{
	if (elementOnFocus == pElement)
		return;
	if (elementOnFocus != nullptr)
		RemoveFocus();
	elementOnFocus = pElement;
	
	if (pElement)
	{
		pElement->Raise(ON_FOCUS, nullptr);
	}
}

void FocusManager::RemoveFocus()
{
	elementOnFocus->Raise(LOST_FOCUS, nullptr);
	elementOnFocus = nullptr;
}
