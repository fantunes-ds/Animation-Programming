#pragma once

#include <GPM/GPM.h>

using namespace GPM;

namespace Animation
{
    class Bone
    {
    public:
        Bone() = default;
        ~Bone() = default;

        const Vector3F& GetPosition() const { return m_position; }
        const Quaternion& GetRotation() const { return m_rotation; }
        int GetParentIndex() const { return m_parentIndex; }
        const std::string& GetName() const { return m_name; }
        const Matrix4F& GetLocalTransformMatrix() const { return m_localTransformMatrix; }
        const Matrix4F& GetWorldTransformMatrix() const { return m_worldTransformMatrix; }

        void SetPosition(const Vector3F& p_newPosition) { m_position = p_newPosition; }
        void SetRotation(const Quaternion& p_newRotation) { m_rotation = p_newRotation; }
        void SetParentIndex(const int p_newIndex) { m_parentIndex = p_newIndex; }
        void SetName(const std::string& p_newName) { m_name = p_newName; }
        void SetLocalTransformMatrix(const Matrix4F& p_newMatrix) { m_localTransformMatrix = p_newMatrix; };
        void SetWorldTransformMatrix(const Matrix4F& p_newMatrix) { m_worldTransformMatrix = p_newMatrix; };

    private:
        Vector3F m_position {};
        Quaternion m_rotation {};
        std::string m_name{};
        int m_parentIndex{};
        Matrix4F m_localTransformMatrix{};
        Matrix4F m_worldTransformMatrix{};

    };
}