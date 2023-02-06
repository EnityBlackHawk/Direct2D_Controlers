#pragma once
#include <vector>
#include "Element.h"

#define INFINITY 0
#define ONCE_ONLY 1

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
	char repeatBehavior;
};

class Animator
{
public:

	int AddAnimation(ANIMATION animation, float initialValue, float targetValue, float duration, Element* pElement, char repeatBehavior = ONCE_ONLY);

	void StartAnimation(int index);
	void StartAllAnimations();

	static DWORD WINAPI AnimationThread(_animation*);


private:

	std::vector<_animation> animations;
	

};

