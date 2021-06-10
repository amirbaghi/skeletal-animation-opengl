#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <algorithm>

#include "./Headers/Scene.h"

void Scene::addBone(std::string parent, std::string boneName, int length, float angle)
{
    Bone *parentBone = this->getBone(parent);

    Bone *bone = new Bone(length, boneName);

    this->skeleton->addBone(parentBone, bone, glm::vec3(), glm::vec3(0, 0, angle));
}

Bone *Scene::getBone(std::string boneName)
{
    return this->skeleton->getBone(boneName);
}

Bone *Scene::getBone(int boneIndex)
{
    auto boneNames = this->skeleton->getBoneNames();
    return this->getBone(boneNames[boneIndex]);
}

int Scene::getCount()
{
    return this->skeleton->getBoneNames().size();
}

void Scene::selectBone(std::string boneName)
{
    auto bone = this->getBone(boneName);
    if (bone != nullptr)
    {
        this->selectedBone = bone;
    }
}

void Scene::selectBone(int boneIndex)
{
    auto bone = this->getBone(boneIndex);
    if (bone != nullptr)
    {
        if (this->selectedBone == bone)
        {
            this->selectedBone = nullptr;
        }
        else
        {
            this->selectedBone = bone;
        }
    }
}

Bone *Scene::getSelectedBone()
{
    return this->selectedBone;
}

void Scene::rotateSelectedBone(float delta_theta)
{
    if (selectedBone != nullptr)
    {
        selectedBone->rotate(glm::vec3(0, 0, delta_theta));
    }
}

void Scene::setSkin(std::vector<Vertex> skin)
{
    this->skin = skin;
}

void Scene::inverseKinematic(glm::vec2 pos)
{
    auto target = glm::vec3(pos.x, pos.y, 1);

    Bone *endEffectorBone = this->getBone(this->getCount() - 1);

    auto boneNames = skeleton->getBoneNames();
    std::reverse(boneNames.begin(), boneNames.end());
    for (auto boneName : boneNames)
    {
        Bone *bone = this->getBone(boneName);
        glm::vec3 animatedStartPos, animatedEndEffector;
        animatedStartPos = bone->transform_from_bonespace_animated(glm::vec3(0, 0, 0));
        animatedEndEffector = endEffectorBone->transform_from_bonespace_animated(glm::vec3(endEffectorBone->getLength(), 0, 0));

        auto u = glm::length(animatedEndEffector - animatedStartPos);
        auto f = glm::length(target - animatedStartPos);
        auto g = glm::length(target - animatedEndEffector);

        auto alpha = glm::acos((u * u + f * f - g * g) / (2 * u * f));

        auto prev_theta = bone->getTheta();

        auto v1 = animatedEndEffector - animatedStartPos;
        auto v2 = target - animatedStartPos;

        auto crossprod = glm::cross(v1, v2);

        if (alpha < 0.01)
        {
            break;
        }

        if (crossprod.z > 0)
        {
            alpha += prev_theta.z;

            if (alpha > 2 * glm::pi<double>())
            {
                alpha -= 2 * glm::pi<double>();
            }
        }
        else
        {
            alpha = prev_theta.z - alpha;
            if (alpha < 0)
            {
                alpha += 2 * glm::pi<double>();
            }
        }

        bone->rotate(glm::vec3(0, 0, alpha));

        bone->calculate_mi_a();
    }
}

void Scene::init()
{
    this->skeleton->calculate_mi_d();
}

void Scene::update()
{
    this->skeleton->calculate_mi_a();

    for (Vertex &vert : this->skin)
    {
        Bone *bone1 = this->getBone(vert.bone_id1);
        Bone *bone2 = this->getBone(vert.bone_id2);

        glm::vec3 default_pos = glm::vec3(vert.default_x, vert.default_y, 1);

        glm::vec3 pos1 = bone1->transform_forward_kinematics(default_pos);
        glm::vec3 pos2 = bone2->transform_forward_kinematics(default_pos);

        vert.animated_x = pos1.x * vert.bone_weight1 + pos2.x * vert.bone_weight2;
        vert.animated_y = pos1.y * vert.bone_weight1 + pos2.y * vert.bone_weight2;
    }
}

void Scene::render()
{
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(-20.0, 0.0);
    glVertex2f(20.0, 0.0);
    glVertex2f(0.0, 20.0);
    glVertex2f(0.0, -20.0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    auto boneNames = this->skeleton->getBoneNames();
    for (auto boneName : boneNames)
    {
        glm::vec3 startPoint, endPoint, topPoint(2, 2, 0), bottomPoint(2, -2, 0);

        auto bone = this->getBone(boneName);

        bone->calculate_bone_endpoints(startPoint, endPoint);

        topPoint = bone->transform_from_bonespace_animated(topPoint);
        bottomPoint = bone->transform_from_bonespace_animated(bottomPoint);

        if (bone == selectedBone)
        {
            glLineWidth(4);
        }

        glBegin(GL_LINE_LOOP);
        glVertex2f(startPoint.x, startPoint.y);
        glVertex2f(topPoint.x, topPoint.y);
        glVertex2f(endPoint.x, endPoint.y);
        glVertex2f(bottomPoint.x, bottomPoint.y);
        glEnd();

        glLineWidth(1.0);
    }

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (auto vert : this->skin)
    {
        glVertex2d(vert.animated_x, vert.animated_y);
    }
    glEnd();
}