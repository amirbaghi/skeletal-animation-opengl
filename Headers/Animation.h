#pragma once

#include <vector>
#include "Skeleton.h"
#include "Keyframe.h"

class Animation
{
public:
    Animation(Skeleton *skeleton, int inBetweenFrameCount) : shouldPlay(false), currentFrame(0), currentKeyframeIndex(0), inBetweenFramesCount(inBetweenFrameCount)
    {
        this->skeleton = skeleton;
    }

    ~Animation()
    {
        for (auto keyframe : keyframes)
        {
            delete keyframe;
        }
    }

    void addKeyframe(Keyframe *keyframe);
    int getKeyframeCount();

    std::map<std::string, glm::quat> getCurrentFrameOrientations();

    void setShouldPlay(bool value);
    bool getShouldPlay();

private:
    Skeleton *skeleton;

    std::vector<Keyframe *> keyframes;
    bool shouldPlay;

    int inBetweenFramesCount;
    int currentFrame;
    int currentKeyframeIndex;
};