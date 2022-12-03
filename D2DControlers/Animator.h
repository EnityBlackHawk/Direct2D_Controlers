#pragma once
#include <vector>
#include "Element.h"


typedef void(*ANIMATION)(float delta);

struct _animation
{
	ANIMATION animation;
	float initialValue;
	float targetValue;
	float duration;
	float changeRate;
};

class Animator
{
public:

	int AddAnimation(ANIMATION animation, float initialValue, float targetValue, float duration);
	void StartAllAnimations() const;

	static DWORD WINAPI AnimationThread(_animation*);

private:


	std::vector<_animation> animations;

};

