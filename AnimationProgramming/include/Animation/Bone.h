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
        const Matrix4F& GetLocalTPose() const { return m_localTPose; }
        const Matrix4F& GetWorldTPose() const { return m_worldTPose; }
        const Matrix4F& GetAnimTransformMatrix() const { return m_animTransformMatrix; }
        const std::vector<unsigned int>& GetChildIndexes() const { return m_childIndexes; }
        unsigned int GetChildIndex(unsigned int p_index) const { return m_childIndexes[p_index]; }

        void SetPosition(const Vector3F& p_newPosition) { m_position = p_newPosition; }
        void SetRotation(const Quaternion& p_newRotation) { m_rotation = p_newRotation; }
        void SetParentIndex(const int p_newIndex) { m_parentIndex = p_newIndex; }
        void SetNewChildIndex(const unsigned int p_newIndex) { m_childIndexes.push_back(p_newIndex); }
        void SetName(const std::string& p_newName) { m_name = p_newName; }
        void SetLocalTransformMatrix(const Matrix4F& p_newMatrix) { m_localTPose = p_newMatrix; };
        void SetWorldTransformMatrix(const Matrix4F& p_newMatrix) { m_worldTPose = p_newMatrix; };
        void SetAnimTransformMatrix(const Matrix4F& p_newMatrix) { m_animTransformMatrix = p_newMatrix; };

    private:
        Vector3F m_position{};
        Quaternion m_rotation{};
        std::string m_name{};
        unsigned int m_parentIndex{};
        std::vector<unsigned int> m_childIndexes;
        Matrix4F m_localTPose{};
        Matrix4F m_worldTPose{};
        Matrix4F m_animTransformMatrix{};

    };
}