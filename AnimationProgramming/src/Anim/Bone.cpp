#include <Anim/Bone.h>
#include <utility>

Anim::Bone::Bone(std::string p_name, const unsigned p_index, const Vector3F& p_position, Quaternion p_rotation) :
m_name(std::move(p_name)), m_parentIndex(p_index), m_position(p_position), m_rotation(std::move(p_rotation))
{
    m_localTPose = Matrix4F::CreateTransformation(m_position, m_rotation, Vector3F::one);
}

Anim::Bone::Bone(std::string&& p_name, const unsigned p_index, Vector3F&& p_position, Quaternion&& p_rotation) :
m_name (p_name), m_parentIndex(p_index), m_position(p_position), m_rotation(p_rotation)
{
    m_localTPose = Matrix4F::CreateTransformation(m_position, m_rotation, Vector3F::one);
}


