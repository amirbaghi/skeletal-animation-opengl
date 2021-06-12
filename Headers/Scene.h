#pragma once

#include "Skeleton.h"
#include "Vertex.h"
#include "Animation.h"

class Scene
{
public:
    Scene() : selectedBone(nullptr), animation(nullptr)
    {
        this->skeleton = new Skeleton();
    }

    void createNewAnimation(int inBetweenFrameCounts);
    void addKeyframe();
    void playAnimation();
    void stopAnimation();
    bool isAnimationPlaying();

    void addBone(std::string parent, std::string boneName, int length, float angle);
    Bone *getBone(std::string boneName);
    Bone *getBone(int boneIndex);
    int getCount();

    Bone *getSelectedBone();
    void selectBone(std::string boneName);
    void selectBone(int boneIndex);
    void rotateSelectedBone(float delta_theta);

    void setSkin(std::vector<Vertex> skin);

    void inverseKinematic(glm::vec2 pos);

    void init();
    void update();
    void render();

private:
    std::vector<Vertex> skin;
    Skeleton *skeleton;
    Bone *selectedBone;
    Animation *animation;
};