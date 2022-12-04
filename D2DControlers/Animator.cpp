#include "Animator.h"


int callCounter = 0;

int Animator::AddAnimation(ANIMATION animation, float initialValue, float targetValue, float duration, Element* pElement)
{
    _animation stuc = {};
    stuc.animation = animation;
    stuc.initialValue = initialValue;
    stuc.targetValue = targetValue;
    stuc.duration = duration;
    stuc.changeRate = (targetValue - initialValue) / (duration * 0.06);
    stuc.pElement = pElement;

    animations.push_back(stuc);

    return animations.size() - 1;
}

void Animator::StartAnimation(int index)
{
    if (callCounter > callLimit)
        return;
    _animation& an = animations[index];

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Animator::AnimationThread, (void*)&an, NULL, nullptr);
    callCounter++;
}

void Animator::StartAllAnimations()
{
    for (int i = 0; i < animations.size(); i++)
    {
        if (callCounter > callLimit)
            return;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Animator::AnimationThread, (void*)&animations[i], NULL, nullptr);
        callCounter++;
    }
}

DWORD __stdcall Animator::AnimationThread(_animation* an)
{

    float delta = an->initialValue;
    while (true)
    {
        an->animation(delta);

        if (delta == an->targetValue)
            break;

        delta += an->changeRate;

        if (an->changeRate < 0 && delta < an->targetValue)
        {
            delta = an->targetValue;
        }
        if (an->changeRate > 0 && delta > an->targetValue)
        {
            delta = an->targetValue;
        }


        Sleep(1);
    }
    callCounter--;


    return 0;
}
