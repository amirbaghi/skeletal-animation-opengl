#include "./Headers/Keyframe.h"

void Keyframe::addOrientation(std::string boneName, glm::quat orientation)
{
    this->orientations[boneName] = orientation;
}

glm::quat Keyframe::getOrientation(std::string boneName)
{
    return this->orientations[boneName];
}

std::map<std::string, glm::quat> Keyframe::getOrientations()
{
    return this->orientations;
}