#pragma once
#include <vector>
#include "Element.h"


typedef void(*ANIMATION)(float delta);

constexpr int callLimit = 5;

struct _animation
{
	ANIMATION animation;
	float initialValue;
	float targetValue;
	float duration;
	float changeRate;
	Element* pElement;
	int _index;
};

class Animator
{
public:

	int AddAnimation(ANIMATION animation, float initialValue, float targetValue, float duration, Element* pElement);

	void StartAnimation(int index);
	void StartAllAnimations();

	static DWORD WINAPI AnimationThread(_animation*);

	static std::vector<Element*> elementsOnAnimation;

private:

	std::vector<_animation> animations;
	

};

