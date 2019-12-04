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
                Matrix4F child = m_bones[i].GetCurrentTransformMatrix();
                Matrix4F parent = m_bones[m_bones[i].GetParentIndex()].GetCurrentTransformMatrix();
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

    void PlayAnimation(const size_t frame)
    {
        float m_matricesArray[976];
        Vector3F newPos{};
        Vector4F newRot;
        for (unsigned int i = 0; i < m_bones.size(); ++i)
        {
            GetAnimLocalBoneTransform("ThirdPersonRun.anim", i, frame % GetAnimKeyCount("ThirdPersonRun.anim"), newPos.x, newPos.y, newPos.z, newRot.w, newRot.x, newRot.y, newRot.z);
            Quaternion newRotQuat(newRot.x, newRot.y, newRot.z, newRot.w);
             
            Matrix4F newMat = Matrix4F::CreateTransformation(newPos, newRotQuat, Vector3F::one);
            if (m_bones[i].GetParentIndex() > 0)
                m_bones[i].SetCurrentTransformMatrix(m_bones[m_bones[i].GetParentIndex()].GetCurrentTransformMatrix() * m_bones[i].GetLocalTPose() * newMat);
            else
                m_bones[i].SetCurrentTransformMatrix(m_bones[i].GetLocalTPose() * newMat);
            
            for (int j = 0; j < 16; ++j)
                m_matricesArray[(i * 16) + j] = (m_bones[i].GetCurrentTransformMatrix() * Matrix4F::Inverse(m_bones[i].GetWorldTPose())).m_data[j];
        }
            SetSkinningPose(m_matricesArray, m_bones.size());
    }

    // We have no reason to create a class for this because there is only one squeleton
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

            newBone.SetCurrentTransformMatrix(newBone.GetWorldTPose());
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

