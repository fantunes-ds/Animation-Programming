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


    //float counter;
    virtual void Update(float frameTime) override
    {
        //counter += frameTime;
        DisplayBones();
    }

    void DisplayBones()
    {
        for (unsigned int i = 0; i < m_bones.size(); ++i)
        {
            if (m_bones[i].GetParentIndex() > 0)
            {
                Matrix4F child = m_bones[i].GetWorldTransformMatrix();
                Matrix4F parent = m_bones[m_bones[i].GetParentIndex()].GetWorldTransformMatrix();
                const Vector3F childPos = { child.m_data[3],child.m_data[7],child.m_data[11] };
                const Vector3F parentPos = { parent.m_data[3],parent.m_data[7],parent.m_data[11] };
                DrawLine(parentPos.x, parentPos.y, parentPos.z, childPos.x, childPos.y, childPos.z, 1, 0, 1);
                //std::cout << i << "drawing a line between " << m_bones[m_bones[i].GetParentIndex()].GetName() << " and " << m_bones[i].GetName() << '\n';
            }
            else
            {
                Matrix4F child = m_bones[i].GetWorldTransformMatrix();
                const Vector3F childPos = { child.m_data[3],child.m_data[7],child.m_data[11] };
                DrawLine(0, 0, 0, childPos.x, childPos.y, childPos.z, 1, 0, 1);
            }
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
            newBone.SetLocalTransformMatrix(Matrix4F::CreateTransformation(newBone.GetPosition(), newBone.GetRotation(), Vector3F::one));

            if (newBone.GetParentIndex() >= 0)
                newBone.SetWorldTransformMatrix(m_bones[newBone.GetParentIndex()].GetWorldTransformMatrix() * newBone.GetLocalTransformMatrix());
            else
                newBone.SetWorldTransformMatrix(newBone.GetLocalTransformMatrix());

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

