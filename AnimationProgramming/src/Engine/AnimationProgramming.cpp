// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Engine/Engine.h>
#include <Engine/Simulation.h>
#include <string>
#include <iostream>
#include <vector>
#include "Animation/Bone.h"
#include <GPM/GPM.h>

class CSimulation : public ISimulation
{

    std::vector<Animation::Bone> m_bones;

    virtual void Init() override
    {
        CreateSkeleton();
    }


    float counter;
    float animSpeed = 16;
    virtual void Update(float frameTime) override
    {
        counter += frameTime * animSpeed;
        PlayAnimation(static_cast<size_t>(counter));

        DisplayBones();
    }

    void DisplayBones()
    {
        for (unsigned int i = 2; i < m_bones.size(); ++i)
        {
            if (m_bones[i].GetParentIndex() > 0)
            {
                Matrix4F child = m_bones[i].GetAnimTransformMatrix();
                Matrix4F parent = m_bones[m_bones[i].GetParentIndex()].GetAnimTransformMatrix();
                const Vector3F childPos = { child.m_data[3],child.m_data[7],child.m_data[11] };
                const Vector3F parentPos = { parent.m_data[3],parent.m_data[7],parent.m_data[11] };
                DrawLine(parentPos.x, parentPos.y, parentPos.z, childPos.x, childPos.y, childPos.z, 1, 0, 1);
                //std::cout << i << "drawing a line between " << m_bones[m_bones[i].GetParentIndex()].GetName() << " and " << m_bones[i].GetName() << '\n';
            }
            else
            {
                Matrix4F child = m_bones[i].GetWorldTPose();
                const Vector3F childPos = { child.m_data[3],child.m_data[7],child.m_data[11] };
                DrawLine(0, 0, 0, childPos.x, childPos.y, childPos.z, 1, 0, 1);
            }
        }
    }

    void UpdateSkeleton(const unsigned int p_boneIndex)
    {
            if (!m_bones[p_boneIndex].GetChildIndexes().empty())
                for (unsigned int b = 0; b < m_bones[p_boneIndex].GetChildIndexes().size(); ++b)
                    m_bones[m_bones[p_boneIndex].GetChildIndexes()[b]].SetAnimTransformMatrix(m_bones[m_bones[p_boneIndex].GetChildIndexes()[b]].GetLocalTPose() * m_bones[p_boneIndex].GetWorldTPose());
    }

    void PlayAnimation(const size_t frame)
    {
        Vector3F newPos{};
        Vector4F newRot;

        for (unsigned int i = 0; i < m_bones.size(); ++i)
        {
            GetAnimLocalBoneTransform("ThirdPersonWalk.anim", i, frame % GetAnimKeyCount("ThirdPersonWalk.anim"), newPos.x, newPos.y, newPos.z, newRot.w, newRot.x, newRot.y, newRot.z);
            Quaternion newRotQuat(newRot.x, newRot.y, newRot.z, newRot.w);
             
            Matrix4F newMat = Matrix4F::CreateTransformation(newPos, newRotQuat, Vector3F::one);
            if(m_bones[i].GetParentIndex() > 0)
               m_bones[i].SetAnimTransformMatrix(m_bones[m_bones[i].GetParentIndex()].GetAnimTransformMatrix() * m_bones[i].GetLocalTPose()  * newMat);
            else
               m_bones[i].SetAnimTransformMatrix(m_bones[i].GetLocalTPose()  * newMat);

            //UpdateSkeleton(i);
        }
    }

    void CreateSkeleton()
    {
        for (unsigned int i = 0; i < GetSkeletonBoneCount(); ++i)
        {
            Animation::Bone newBone;
            Vector3F newPos{};
            Vector4F newRot;

            GetSkeletonBoneLocalBindTransform(i, newPos.x, newPos.y, newPos.z, newRot.w, newRot.x, newRot.y, newRot.z);


            newBone.SetName(GetSkeletonBoneName(i));
            if (newBone.GetName()._Starts_with("ik_"))
                continue;

            newBone.SetPosition(newPos);
            const Quaternion quat(newRot.x, newRot.y, newRot.z, newRot.w);
            newBone.SetRotation(quat);
            newBone.SetParentIndex(GetSkeletonBoneParentIndex(i));
            if (i > 0)
                m_bones[newBone.GetParentIndex()].SetNewChildIndex(i);

            newBone.SetLocalTransformMatrix(Matrix4F::CreateTransformation(newBone.GetPosition(), newBone.GetRotation(), Vector3F::one));

            if (newBone.GetParentIndex() >= 0)
                newBone.SetWorldTransformMatrix(m_bones[newBone.GetParentIndex()].GetWorldTPose() * newBone.GetLocalTPose());
            else
                newBone.SetWorldTransformMatrix(newBone.GetLocalTPose());

            newBone.SetAnimTransformMatrix(newBone.GetWorldTPose());
            m_bones.push_back(newBone);
        }
    }
};



int main()
{
	CSimulation simulation;
	Run(&simulation, 1400, 800);

    return 0;
}

