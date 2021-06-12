#include "./Headers/Animation.h"

#include "gtc/quaternion.hpp"

void Animation::addKeyframe(Keyframe *keyframe)
{
    this->keyframes.push_back(keyframe);
}

std::map<std::string, glm::quat> Animation::getCurrentFrameOrientations()
{
    if (this->currentFrame > this->inBetweenFramesCount)
    {
        this->currentFrame = 1;
        this->currentKeyframeIndex = (this->currentKeyframeIndex + 1) % (this->keyframes.size() - 1);
    }

    float u = (1.0 / (float)this->inBetweenFramesCount) * this->currentFrame;
    std::map<std::string, glm::quat> new_orientations;
    auto boneNames = this->skeleton->getBoneNames();
    for (auto boneName : boneNames)
    {
        glm::quat q1 = this->keyframes[this->currentKeyframeIndex]->getOrientation(boneName);
        glm::quat q2 = this->keyframes[this->currentKeyframeIndex + 1]->getOrientation(boneName);

        glm::quat q = glm::slerp(q1, q2, u);

        new_orientations[boneName] = q;
    }

    this->currentFrame += 1;

    return new_orientations;
}

int Animation::getKeyframeCount()
{
    return this->keyframes.size();
}

void Animation::setShouldPlay(bool value)
{
    this->shouldPlay = value;

    if (this->shouldPlay)
    {
        currentFrame = 0;
        currentKeyframeIndex = 0;
    }
}

bool Animation::getShouldPlay()
{
    return this->shouldPlay;
}
