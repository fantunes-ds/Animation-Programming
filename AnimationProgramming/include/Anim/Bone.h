#pragma once

#include <GPM/GPM.h>
#include <unordered_map>

using namespace GPM;

namespace Anim
{
    class Bone
    {
    public:
        Bone() = delete;
        Bone(std::string p_name, const unsigned int p_index, const Vector3F& p_position,
             Quaternion p_rotation);
        Bone(std::string&& p_name, unsigned p_index, Vector3F&& p_position, Quaternion&& p_rotation);
        ~Bone() = default;

        const Vector3F& GetPosition() const { return m_position; }
        const Quaternion& GetRotation() const { return m_rotation; }
        int GetParentIndex() const { return m_parentIndex; }
        const std::string& GetName() const { return m_name; }
        const Matrix4F& GetLocalTPose() const { return m_localTPose; }
        const Matrix4F& GetWorldTPose() const { return m_worldTPose; }
        const Matrix4F& GetCurrentTransformMatrix() const { return m_currentTransformMatrix; }

        void SetPosition(const Vector3F& p_newPosition) { m_position = p_newPosition; }
        void SetRotation(const Quaternion& p_newRotation) { m_rotation = p_newRotation; }
        void SetParentIndex(const int p_newIndex) { m_parentIndex = p_newIndex; }
        void SetName(const std::string& p_newName) { m_name = p_newName; }
        void SetLocalTransformMatrix(const Matrix4F& p_newMatrix) { m_localTPose = p_newMatrix; }
        void SetWorldTransformMatrix(const Matrix4F& p_newMatrix) { m_worldTPose = p_newMatrix; }
        void SetCurrentTransformMatrix(const Matrix4F& p_newMatrix) { m_currentTransformMatrix = p_newMatrix; }

    private:
        std::string m_name{};
        unsigned int m_parentIndex{};
        Vector3F m_position{};
        Quaternion m_rotation{};

        Matrix4F m_localTPose{};
        Matrix4F m_worldTPose{};
        Matrix4F m_currentTransformMatrix{};
    };
}