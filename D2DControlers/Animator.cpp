#include "Animator.h"

int Animator::AddAnimation(ANIMATION animation, float initialValue, float targetValue, float duration)
{
    _animation stuc = {};
    stuc.animation = animation;
    stuc.initialValue = initialValue;
    stuc.targetValue = targetValue;
    stuc.duration = duration;
    stuc.changeRate = (targetValue - initialValue) / duration;

    animations.push_back(stuc);

    return animations.size() - 1;
}

void Animator::StartAllAnimations() const
{
    for (int i = 0; i < animations.size(); i++)
    {
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Animator::AnimationThread, (void*)&animations[i], NULL, nullptr);
    }
}

DWORD __stdcall Animator::AnimationThread(_animation* an)
{
    float delta = an->initialValue;
    while (delta != an->targetValue)
    {
        an->animation(delta);
        delta += an->changeRate;
    }
    return 0;
}
