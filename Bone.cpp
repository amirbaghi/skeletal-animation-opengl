#include "./Headers/Bone.h"

void Bone::rotate(glm::vec3 theta)
{
    this->mi_l = glm::identity<glm::mat4>();
    this->mi_l = glm::rotate(this->mi_l, theta.z, glm::vec3(0, 0, 1));
    this->mi_l = glm::rotate(this->mi_l, theta.y, glm::vec3(0, 1, 0));
    this->mi_l = glm::rotate(this->mi_l, theta.x, glm::vec3(1, 0, 0));

    this->quat = glm::quat_cast(this->mi_l);
}

void Bone::calculate_mi_d()
{
    if (parent == nullptr)
    {
        this->mi_d = glm::identity<glm::mat4>();
    }
    else
    {
        this->mi_d = this->parent->mi_d * this->mi_p;
    }

    for (auto child : this->children)
    {
        child->calculate_mi_d();
    }
}

void Bone::calculate_mi_a()
{
    if (parent == nullptr)
    {
        this->mi_a = this->mi_l;
    }
    else
    {
        this->mi_a = this->parent->mi_a * this->mi_p * this->mi_l;
    }

    for (auto child : this->children)
    {
        child->calculate_mi_a();
    }
}

void Bone::calculate_bone_endpoints(glm::vec3 &p1, glm::vec3 &p2)
{
    glm::vec4 v1 = glm::vec4(0, 0, 0, 1);
    glm::vec4 v2 = glm::vec4(this->length, 0, 0, 1);

    v1 = this->mi_a * v1;
    v2 = this->mi_a * v2;

    p1 = glm::vec3(v1.x / v1.w, v1.y / v1.w, v1.z / v1.w);
    p2 = glm::vec3(v2.x / v2.w, v2.y / v2.w, v2.z / v2.w);
}

glm::vec3 Bone::transform_forward_kinematics(glm::vec3 vertex)
{
    glm::mat4 m_i = this->mi_a * glm::inverse(this->mi_d);
    auto v = m_i * glm::vec4(vertex, 1);

    return glm::vec3(v.x / v.w, v.y / v.w, v.z / v.w);
}

glm::vec3 Bone::transform_from_bonespace_animated(glm::vec3 vertex)
{
    glm::vec4 v = this->mi_a * glm::vec4(vertex, 1);
    return glm::vec3(v.x / v.w, v.y / v.w, v.z / v.w);
}

glm::vec3 Bone::transform_from_bonespace_default_pose(glm::vec3 vertex)
{
    glm::vec4 v = this->mi_d * glm::vec4(vertex, 1);
    return glm::vec3(v.x / v.w, v.y / v.w, v.z / v.w);
}

glm::vec3 Bone::transform_from_bonespace_animated_without_local_transformation(glm::vec3 vertex)
{
    if (this->parent != nullptr)
    {
        glm::vec4 v = this->parent->mi_a * this->mi_p * glm::vec4(vertex, 1);
        return glm::vec3(v.x / v.w, v.y / v.w, v.z / v.w);
    }
    else
    {
        return vertex;
    }
}