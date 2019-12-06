#pragma once
#include <Engine/Simulation.h>
#include <Anim/Bone.h>

class Animation : public ISimulation
{
public: 
    Animation() = default;
    ~Animation() = default;

    /**
     * @brief Override for the ISimulation Init
     */
    virtual void Init() override;

    /**
     * @brief Update for the ISimulation Init
     */
    virtual void Update(float frameTime) override;
    void UpdateTime(const float frameTime);

    /**
     * @brief This function check if user has pressed keys and applies consequences
     */
    void CheckForInput();

    /**
     * Creates the skeleton that will be used for the animations.
     */
    void CreateSkeleton();

    /**
     * @brief Function used to show the bones on the model.
     * @note You will only see them if the mesh doesn't load, or if you turn the mesh off in the shader
     */
    void DisplaySkeleton();

    /**
     * @brief Loads an animation and stores it in m_animations map.
     * @note You can store as many animations as you want.
     */
    void LoadAnimation(const std::string& p_animName);

    /**
     * @brief Plays an animation, given it has been inserted in the map.
     */
    void PlayAnimation(float p_animationFrameTarget, const std::string& p_animName);

    /**
     * @brief Draws an animation progression bar, which shows which frame of the animation you are at.
     */
    static void DrawProgressBar(float p_animationFrameTarget, const int nbOfFrames);

    /**
     * @brief Adds pre-calculated animation to the map.
     * @note : animation must be sent frame by frame for every bone (ex: {frame[0], bone[0...->...61]}, frame[1], bone[0...->...61]
     */
    void AddNewAnimation(const std::string& p_animName, const std::vector<std::pair<Vector3F, Quaternion>>& p_animData) { m_animations.insert_or_assign(p_animName, p_animData); }

private:

    std::vector<Anim::Bone> m_bones{};
    size_t m_skeletonBoneCount{0};

    float TimeElapsedSinceAnimStart{0.0f};
    float animSpeed{5.0f};
    size_t m_currentAnimationFrameCount = 0;

    std::string m_currentAnimation{ "ThirdPersonRun.anim" };
    std::string m_animToPlay{ "ThirdPersonRun.anim" };

    std::unordered_map<std::string, std::vector<std::pair<Vector3F, Quaternion>>> m_animations{};
};

