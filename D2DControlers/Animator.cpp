#include "Animator.h"

std::vector<Element*> Animator::elementsOnAnimation;

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
    if (an.pElement)
    {
        if (std::find(elementsOnAnimation.begin(), elementsOnAnimation.end(), an.pElement) != elementsOnAnimation.end())
            return;
        else
        {
            elementsOnAnimation.push_back(an.pElement);
            an._index = elementsOnAnimation.size() - 1;
        }
    }
    

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
        delta += an->changeRate;

        if (an->changeRate < 0 && delta < an->targetValue)
            break;
        if (an->changeRate > 0 && delta > an->targetValue)
            break;

        Sleep(1);
    }
    callCounter--;
    //std::remove(Animator::elementsOnAnimation.begin(), Animator::elementsOnAnimation.end(), an->pElement);
    Animator::elementsOnAnimation.erase(Animator::elementsOnAnimation.begin() + an->_index);
    return 0;
}
