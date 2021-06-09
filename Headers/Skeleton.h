#pragma once

#include "Bone.h"
#include <map>

class Skeleton
{
public:
    void addBone(Bone *parent, Bone *bone, glm::vec3 translation, glm::vec3 rotation);
    std::vector<std::string> getBoneNames();
    Bone* getBone(std::string boneName);

    void calculate_mi_d();
    void calculate_mi_a();


private:
    Bone *root;
    std::vector<std::string> boneNames;
    std::map<std::string, Bone *> bones;
};