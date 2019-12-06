#include <Anim/Animation.h>
#include <windows.h>


void Animation::Init()
{
    CreateSkeleton();
    LoadAnimation("ThirdPersonRun.anim");
    LoadAnimation("ThirdPersonWalk.anim");
}

void Animation::Update(float frameTime)
{
    TimeElapsedSinceAnimStart += frameTime * animSpeed;
    CheckForInput();

    PlayAnimation(TimeElapsedSinceAnimStart, m_currentAnimation);

    DisplayBones();
}

// We have no reason to create a class for this because there is only one squeleton
void Animation::CreateSkeleton()
{
    m_skeletonBoneCount = GetSkeletonBoneCount();
    for (unsigned int i = 0; i < m_skeletonBoneCount; ++i)
    {
        Vector3F newPos{};
        Vector4F newRot{};

        std::string name = GetSkeletonBoneName(i);
        if (name._Starts_with("ik_"))
            continue;

        GetSkeletonBoneLocalBindTransform(i, newPos.x, newPos.y, newPos.z, newRot.w, newRot.x, newRot.y, newRot.z);

        const Quaternion quat(newRot.x, newRot.y, newRot.z, newRot.w);

        Anim::Bone newBone(name, GetSkeletonBoneParentIndex(i), newPos, quat);

        if (newBone.GetParentIndex() >= 0)
            newBone.SetWorldTPose(m_bones[newBone.GetParentIndex()].GetWorldTPose() * newBone.GetLocalTPose());
        else
            newBone.SetWorldTPose(newBone.GetLocalTPose());

        // We use Global T-Pose as default value
        newBone.SetCurrentTransformMatrix(newBone.GetWorldTPose());
        m_bones.push_back(newBone);
    }
}

void Animation::CheckForInput()
{
    if (GetKeyState('Y') & 0x8000)
    {
        if (animSpeed > 0)
            animSpeed -= 0.05f;
        else
            animSpeed = 0;
    }
    if (GetKeyState('U') & 0x8000)
    {
        animSpeed += 0.05f;
    }

    if (GetKeyState('O') & 0x8000)
    {
        m_animToPlay = "ThirdPersonWalk.anim";
    }
    if (GetKeyState('I') & 0x8000)
    {
        m_animToPlay = "ThirdPersonRun.anim";
    }
}

void Animation::LoadAnimation(const std::string& p_animName)
{
    Vector3F newCurrentPos{};
    Vector4F newCurrentRot{};

    std::vector<std::pair<Vector3F, Quaternion>> m_animData;

    const size_t nbOfFrames = GetAnimKeyCount(p_animName.c_str());

    for (unsigned int frame = 0; frame < nbOfFrames; ++frame)
    {
        for (unsigned int i = 0; i < m_bones.size(); ++i)
        {
            GetAnimLocalBoneTransform(p_animName.c_str(), i, frame, newCurrentPos.x, newCurrentPos.y, newCurrentPos.z,
                                      newCurrentRot.w, newCurrentRot.x, newCurrentRot.y, newCurrentRot.z);

            Quaternion newCurrentQuat(newCurrentRot.x, newCurrentRot.y, newCurrentRot.z, newCurrentRot.w);
            m_animData.emplace_back(newCurrentPos, newCurrentQuat);
        }
    }
    AddNewAnimation(p_animName, m_animData);
}

void Animation::PlayAnimation(const float p_animationFrameTarget, const std::string& p_animName)
{
    float m_matricesArray[976];
    std::vector<std::pair<Vector3F, Quaternion>> animData = m_animations.find(m_currentAnimation)->second;
    std::vector<std::pair<Vector3F, Quaternion>> nextAnimData;

    if (m_currentAnimation != m_animToPlay)
        nextAnimData = m_animations.find(m_animToPlay)->second;
    else
        nextAnimData = animData;

    const size_t nbOfFrames = animData.size() / m_bones.size();
    const size_t currentAnimKeyFrame = static_cast<size_t>(p_animationFrameTarget) % (nbOfFrames - 1);
    const size_t nextAnimKeyFrame = static_cast<size_t>(p_animationFrameTarget + 1) % (nbOfFrames - 1);

    for (unsigned int i = 0; i < m_bones.size(); ++i)
    {
        const size_t currentFrame = (currentAnimKeyFrame * m_bones.size()) + i;
        const size_t nextFrame = (nextAnimKeyFrame * m_bones.size()) + i;

        DrawProgressBar(p_animationFrameTarget, nbOfFrames);

        const float interTime = Tools::Utils::GetDecimalPart(TimeElapsedSinceAnimStart);
        Vector3F interpolatedPos{ Vector3F::Lerp(animData[currentFrame].first, nextAnimData[nextFrame].first, interTime) };
        Quaternion interpolatedQuat{ Quaternion::SlerpShortestPath(animData[currentFrame].second, nextAnimData[nextFrame].second, interTime) };

        Matrix4F newMat4 = Matrix4F::CreateTransformation(interpolatedPos, interpolatedQuat, Vector3F::one);

        if (m_bones[i].GetParentIndex() > 0)
            m_bones[i].SetCurrentTransformMatrix(m_bones[m_bones[i].GetParentIndex()].GetCurrentTransformMatrix() * m_bones[i].GetLocalTPose() * newMat4);
        else
            m_bones[i].SetCurrentTransformMatrix(m_bones[i].GetLocalTPose() * newMat4);

        for (int j = 0; j < 16; ++j)
            m_matricesArray[(i * 16) + j] = (m_bones[i].GetCurrentTransformMatrix() * Matrix4F::Inverse(m_bones[i].GetWorldTPose())).m_data[j];
    }

    m_currentAnimation = m_animToPlay;

    SetSkinningPose(m_matricesArray, m_bones.size());
}

void Animation::DisplayBones()
{
    for (unsigned int i = 2; i < m_bones.size(); ++i)
    {
        if (m_bones[i].GetParentIndex() > 0)
        {
            Matrix4F       child     = m_bones[i].GetCurrentTransformMatrix();
            Matrix4F       parent    = m_bones[m_bones[i].GetParentIndex()].GetCurrentTransformMatrix();
            const Vector3F childPos  = {child.m_data[3], child.m_data[7], child.m_data[11]};
            const Vector3F parentPos = {parent.m_data[3], parent.m_data[7], parent.m_data[11]};
            DrawLine(parentPos.x, parentPos.y, parentPos.z, childPos.x, childPos.y, childPos.z, 1, 0, 1);
        }
        else
        {
            Matrix4F       child    = m_bones[i].GetWorldTPose();
            const Vector3F childPos = {child.m_data[3], child.m_data[7], child.m_data[11]};
            DrawLine(0, 0, 0, childPos.x, childPos.y, childPos.z, 1, 0, 1);
        }
    }
}

void Animation::DrawProgressBar(const float p_animationFrameTarget, const int nbOfFrames)
{
    const int displacement = 3;
    const int zOffset{ 175 };

    for (int i = (nbOfFrames / 2 * - 1) * displacement; i <= (nbOfFrames / 2) * displacement; i += displacement)
        DrawLine(i, 0, zOffset, i, 0, zOffset+ 10, 1, 1, 0);

    DrawLine(((static_cast<int>(p_animationFrameTarget) % nbOfFrames) - nbOfFrames / 2) * displacement, 0, zOffset,
             ((static_cast<int>(p_animationFrameTarget) % nbOfFrames) - nbOfFrames / 2) * displacement, 0, zOffset + 10,
                1, 0, 0);
}
