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
        Bone(std::string p_name, unsigned p_index, const Vector3F& p_position, const Quaternion& p_rotation);
        ~Bone() = default;

        [[nodiscard]] const std::string& GetName() const { return m_name; }
        [[nodiscard]] int GetParentIndex() const { return m_parentIndex; }

        [[nodiscard]] const Matrix4F& GetLocalTPose() const { return m_localTPose; }
        [[nodiscard]] const Matrix4F& GetWorldTPose() const { return m_worldTPose; }
        [[nodiscard]] const Matrix4F& GetCurrentTransformMatrix() const { return m_currentTransformMatrix; }

        void SetName(const std::string& p_newName) { m_name = p_newName; }
        void SetParentIndex(const int p_newIndex) { m_parentIndex = p_newIndex; }

        void SetLocalTPose(const Matrix4F& p_newMatrix) { m_localTPose = p_newMatrix; }
        void SetWorldTPose(const Matrix4F& p_newMatrix) { m_worldTPose = p_newMatrix; }
        void SetCurrentTransformMatrix(const Matrix4F& p_newMatrix) { m_currentTransformMatrix = p_newMatrix; }

    private:
        std::string m_name{};
        int m_parentIndex{};

        Matrix4F m_localTPose{};
        Matrix4F m_worldTPose{};
        Matrix4F m_currentTransformMatrix{};
    };
}