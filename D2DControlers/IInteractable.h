#pragma once
#include <unordered_map>

#define MOUSE_HOVER_EVENT 1;
#define MOUSE_HOVER_OUT_EVENT 2;



typedef void(*EVENT)(void* sender, void* args);


class Interactable
{
public:

	void AddEvent(int eventId, EVENT action)
	{
		events[eventId] = action;
	}

	void Raise(int eventId, void* args)
	{
		if (events.contains(eventId))
			events[eventId](this, args);
	}


protected:

	bool isInBounds;

	std::unordered_map<int, EVENT> events;

};

