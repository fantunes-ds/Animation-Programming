// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Engine/Engine.h>
#include <Engine/Simulation.h>
#include <string>
#include <iostream>
#include <vector>

class CSimulation : public ISimulation
{
	virtual void Init() override
	{
		int spine01 =	GetSkeletonBoneIndex("spine_01");
		int spineParent = GetSkeletonBoneParentIndex(spine01);
		const char* spineParentName = GetSkeletonBoneName(spineParent);

		float posX, posY, posZ, quatW, quatX, quatY, quatZ;
		size_t keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");
		GetAnimLocalBoneTransform("ThirdPersonWalk.anim", spineParent, keyCount / 2, posX, posY, posZ, quatW, quatX, quatY, quatZ);
		
		printf("Spine parent bone : %s\n", spineParentName);
		printf("Anim key count : %ld\n", keyCount);
		printf("Anim key : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", posX, posY, posZ, quatW, quatX, quatY, quatZ);
	}

	virtual void Update(float frameTime) override
	{
        struct Bone
        {
            float x{}, y{}, z{};
            float w{}, qx{}, qy{}, qz{};
            float truex{ x + qx }, truey{ y + qy }, truez{ z + qz };
            std::string name{};
            int parentIndex{};
        };

        std::vector<Bone> m_bones;

        for (unsigned int i = 0; i < GetSkeletonBoneCount(); ++i)
        {
            Bone newBone;
            GetAnimLocalBoneTransform("ThirdPersonWalk.anim", i, 0, newBone.x, newBone.y , newBone.z, newBone.w, newBone.qx, newBone.qy, newBone.qz);
            newBone.name = GetSkeletonBoneName(i);
            newBone.truex = newBone.x + newBone.qx;
            newBone.truey = newBone.y + newBone.qy;
            newBone.truez = newBone.z + newBone.qz;
            m_bones.push_back(newBone);
        }


        //Bone b0;
        //Bone b1;
        //Bone b2;
        //Bone b3;
        //Bone b4;
        //Bone b5;

        //GetAnimLocalBoneTransform("ThirdPersonWalk.anim", 0, 0, b0.x, b0.y , b0.z, b0.w, b0.qx, b0.qy, b0.qz);
        //GetAnimLocalBoneTransform("ThirdPersonWalk.anim", 1, 0, b1.x, b1.y , b1.z, b1.w, b1.qx, b1.qy, b1.qz);
        //GetAnimLocalBoneTransform("ThirdPersonWalk.anim", 2, 0, b2.x, b2.y , b2.z, b2.w, b2.qx, b2.qy, b2.qz);
        //GetAnimLocalBoneTransform("ThirdPersonWalk.anim", 3, 0, b3.x, b3.y , b3.z, b3.w, b3.qx, b3.qy, b3.qz);
        //GetAnimLocalBoneTransform("ThirdPersonWalk.anim", 4, 0, b4.x, b4.y , b4.z, b4.w, b4.qx, b4.qy, b4.qz);
        //GetAnimLocalBoneTransform("ThirdPersonWalk.anim", 5, 0, b5.x, b5.y , b5.z, b5.w, b5.qx, b5.qy, b5.qz);
        //b0.name = GetSkeletonBoneName(0);

        for (unsigned int i = 1; i < m_bones.size() - 2; ++i)
            DrawLine(m_bones[i].truex + m_bones[i - 1].truex, m_bones[i].truey + m_bones[i-1].truey, m_bones[i].truez + m_bones[i-1].truez, m_bones[i+1].truex + m_bones[i].truex, m_bones[i+1].truey + m_bones[i].truey, m_bones[i+1].truez + m_bones[i].truez, 1, 0, 1);

		//// X axis
		//DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);

		//// Y axis
		//DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);

		//// Z axis
		//DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);

	}
};

int main()
{
	CSimulation simulation;
	Run(&simulation, 1400, 800);

    return 0;
}

