#include "./Headers/Skeleton.h"

void Skeleton::addBone(Bone *parent, Bone *bone, glm::vec3 translation, glm::vec3 rotation)
{
    if (parent == nullptr)
    {
        this->root = bone;
    }
    else
    {
        parent->children.push_back(bone);

        bone->mi_p = glm::identity<glm::mat4>();
        bone->mi_p = glm::translate(bone->mi_p, glm::vec3(parent->length, 0, 0));
        bone->mi_p = glm::translate(bone->mi_p, translation);
        bone->mi_p = glm::rotate(bone->mi_p, rotation.z, glm::vec3(0, 0, 1));
        bone->mi_p = glm::rotate(bone->mi_p, rotation.y, glm::vec3(0, 1, 0));
        bone->mi_p = glm::rotate(bone->mi_p, rotation.x, glm::vec3(1, 0, 0));
    }

    this->boneNames.push_back(bone->name);
    this->bones[bone->name] = bone;
}

std::vector<std::string> Skeleton::getBoneNames()
{
    return this->boneNames;
}

Bone *Skeleton::getBone(std::string boneName)
{
    if (this->bones.count(boneName) != 0)
    {
        return this->bones[boneName];
    }
    else
    {
        return nullptr;
    }
}

void Skeleton::calculate_mi_d()
{
    this->root->calculate_mi_d();
}

void Skeleton::calculate_mi_a()
{
    this->root->calculate_mi_a();
}