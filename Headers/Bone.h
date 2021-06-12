#pragma once
#include <string>
#include <vector>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

class Bone
{
    friend class Skeleton;

public:
    Bone(int length, std::string name) : name(name), length(length), parent(nullptr), mi_d(1), mi_p(1), mi_l(1), mi_a(1), t(0), quat(1, 0, 0, 0), theta(0, 0, 0)
    {
    }

    void rotate(glm::vec3 rotate);
    void rotate(glm::quat quat);

    void calculate_mi_d();
    void calculate_mi_a();
    void calculate_bone_endpoints(glm::vec3 &p1, glm::vec3 &p2);

    glm::vec3 transform_forward_kinematics(glm::vec3 vertex);
    glm::vec3 transform_from_bonespace_animated(glm::vec3 vertex);
    glm::vec3 transform_from_bonespace_animated_without_local_transformation(glm::vec3 vertex);
    glm::vec3 transform_from_bonespace_default_pose(glm::vec3 vertex);

    glm::vec3 getTheta();
    glm::quat getQuat();

    inline int getLength() { return this->length; }
    inline std::string getName() { return this->name; }
    inline glm::mat4 getLocalTransformation() { return this->mi_l; }

private:
    Bone *parent;
    std::vector<Bone *> children;

    std::string name;
    int length;

    glm::mat4 mi_d;
    glm::mat4 mi_a;
    glm::mat4 mi_p;
    glm::mat4 mi_l;

    // Key data
    glm::vec3 t;
    glm::quat quat;
    glm::vec3 theta;
};