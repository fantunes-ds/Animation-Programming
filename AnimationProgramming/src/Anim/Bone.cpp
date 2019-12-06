#include <Anim/Bone.h>
#include <utility>

Anim::Bone::Bone(std::string p_name, const unsigned p_index, const Vector3F& p_position, const Quaternion& p_rotation) :
m_name(std::move(p_name)), m_parentIndex(p_index)
{
    m_localTPose = Matrix4F::CreateTransformation(p_position, p_rotation, Vector3F::one);
}

Anim::Bone::Bone(std::string&& p_name, const unsigned p_index, Vector3F&& p_position, Quaternion&& p_rotation) :
m_name (p_name), m_parentIndex(p_index)
{
    m_localTPose = Matrix4F::CreateTransformation(p_position, p_rotation, Vector3F::one);
}


