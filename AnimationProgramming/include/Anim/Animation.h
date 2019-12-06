#pragma once
#include <Engine/Simulation.h>
#include <Anim/Bone.h>

class Animation : public ISimulation
{
public: 
    Animation() = default;
    ~Animation() = default;

    virtual void Init() override;

    virtual void Update(float frameTime) override;


    void CheckForInput();
    void DisplayBones();
    void LoadAnimation(const std::string& p_animName);
    void PlayAnimation(const float p_animationFrameTarget, const std::string& p_animName);
    static void DrawProgressBar(float p_animationFrameTarget, const int nbOfFrames);
    void CreateSkeleton();

    void AddNewAnimation(const std::string& p_animName, const std::vector<std::pair<Vector3F, Quaternion>>& p_animData) { m_animations.insert_or_assign(p_animName, p_animData); }

private:

    std::vector<Anim::Bone> m_bones;
    size_t m_skeletonBoneCount;

    float TimeElapsedSinceAnimStart;
    float animSpeed = 5;

    std::string m_currentAnimation = "ThirdPersonRun.anim";
    std::string m_animToPlay = "ThirdPersonRun.anim";

    std::unordered_map<std::string, std::vector<std::pair<Vector3F, Quaternion>>> m_animations;
};

