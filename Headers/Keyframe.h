#pragma once

#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include <map>
#include <string>

class Keyframe
{
public:
    Keyframe(int number) : keyFrameNumber(number) {}

    void addOrientation(std::string boneName, glm::quat orientation);
    std::map<std::string, glm::quat> getOrientations();
    glm::quat getOrientation(std::string boneName);

private:
    int keyFrameNumber;
    std::map<std::string, glm::quat> orientations;
};