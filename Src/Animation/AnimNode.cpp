#include "AnimNode.h"
#include "Util.h"
#include"AnimModel.h"
#include"ZzRenderer.h"

std::string & AnimNode::GetNameRef()
{
    return m_name;
}

std::string AnimNode::GetNameCopy()
{
    return m_name;
}

AnimNode::AnimNode()
{
    m_localTransform = vmath::mat4::identity();
    m_combinedTransform = vmath::mat4::identity();
    m_defTransform = vmath::mat4::identity();
    m_nodeType = AnimNodeType::BONE;
}

std::shared_ptr<AnimNode> AnimNode::GetParent() const
{
    return m_parent;
}

void AnimNode::SetParent(std::shared_ptr<AnimNode> pParent)
{
    m_parent = pParent;
}

bool AnimNode::ClearBone()
{
    return AttachBone(nullptr);
}

bool AnimNode::AttachBone(aiBone* bone)
{
    m_linkedBone = bone;

    if (nullptr != bone)
    {
        m_nodeType = AnimNodeType::BONE;
    }
    else
    {
        m_nodeType = AnimNodeType::OTHER;
    }

    return true;
}

aiBone * AnimNode::GetAttachedBone()
{
    return m_linkedBone;
}

AnimNodeType AnimNode::GetNodeTypeCopy() const
{
    return m_nodeType;
}

AnimNodeType & AnimNode::GetNodeTypeRef()
{
    return m_nodeType;
}

void AnimNode::SetNodeType(AnimNodeType nodeType)
{
    m_nodeType = nodeType;
}

vmath::mat4 AnimNode::GetLocalTransformCopy() const
{
    return m_localTransform;
}

vmath::mat4& AnimNode::GetLocalTransformRef()
{
    return m_localTransform;
}

vmath::mat4 AnimNode::GetCombinedTransformCopy() const
{
    return m_combinedTransform;
}

vmath::mat4& AnimNode::GetCombinedTransformRef()
{
    return m_combinedTransform;
}

vmath::mat4 & AnimNode::GetDefTransformRef()
{
    return m_defTransform;
}

vmath::mat4 AnimNode::GetDefTransformCopy()
{
    return m_defTransform;
}

std::vector<std::shared_ptr<AnimNode>>& AnimNode::GetChildrenRef()
{
    return m_children;
}

bool AnimNode::ProcessAnimation(aiAnimation ** aiAnim, unsigned int numAnimations)
{

    for (unsigned int i = 0; i < numAnimations; i++)//walk, run, fly ...
    {
        aiAnimation *anim = aiAnim[i];
        std::string animName = anim->mName.C_Str();
        if (animName.length() == 0)//name not specified in file, give one yourself
        {
            THROW_DEBUG_EXCEPTION("Must have a name, since if anim doesn't have one, AnimModel::ProcessAnimation() gives it one")
        }

        DEBUG_ASSERT(m_animMap.end() == m_animMap.find(animName), "We have already processes animation with this name, shouldn't get again");

        for (unsigned int j = 0; j < anim->mNumChannels; j++)//bone1, bone2,... each channel is for a bone
        {
            if (anim->mChannels[j]->mNodeName.C_Str() == m_name)
            {//channel is for this bone

                m_animMap[animName].reset(new AnimInfo);
                m_animMap[animName]->duration = anim->mDuration;
                m_animMap[animName]->ticksPerSecond = anim->mTicksPerSecond;

                aiNodeAnim* thisBoneChannel = anim->mChannels[j];

                //DEBUG_VSCONSOLE("bone name: " + m_name + " " + STR(thisBoneChannel->mNumPositionKeys) + " " + STR(thisBoneChannel->mNumRotationKeys) + " " + STR(thisBoneChannel->mNumRotationKeys));

                for (unsigned int k = 0; k < thisBoneChannel->mNumPositionKeys; k++)
                {
                    m_animMap[animName]->positionKeys.push_back(thisBoneChannel->mPositionKeys[k]);
                }

                if (!std::is_sorted(m_animMap[animName]->positionKeys.begin(), m_animMap[animName]->positionKeys.end(), [](const aiVectorKey& a, const aiVectorKey& b) {return a.mTime <= b.mTime; }))
                {
                    DEBUG_ASSERT(0, "acc to assimp doc it should already be sorted on time");//http://assimp.sourceforge.net/lib_html/structai_node_anim.html
                    std::sort(m_animMap[animName]->positionKeys.begin(), m_animMap[animName]->positionKeys.end());
                }

                for (unsigned int k = 0; k < thisBoneChannel->mNumRotationKeys; k++)
                {
                    m_animMap[animName]->rotationKeys.push_back(thisBoneChannel->mRotationKeys[k]);
                }

                if (!std::is_sorted(m_animMap[animName]->rotationKeys.begin(), m_animMap[animName]->rotationKeys.end(), [](const aiQuatKey& a, const aiQuatKey& b) {return a.mTime <= b.mTime; }))
                {
                    DEBUG_ASSERT(0, "acc to assimp doc it should already be sorted on time");//http://assimp.sourceforge.net/lib_html/structai_node_anim.html
                    std::sort(m_animMap[animName]->rotationKeys.begin(), m_animMap[animName]->rotationKeys.end());
                }

                for (unsigned int k = 0; k < thisBoneChannel->mNumScalingKeys; k++)
                {
                    m_animMap[animName]->scalingKeys.push_back(thisBoneChannel->mScalingKeys[k]);
                }

                if (!std::is_sorted(m_animMap[animName]->scalingKeys.begin(), m_animMap[animName]->scalingKeys.end(), [](const aiVectorKey& a, const aiVectorKey& b) {return a.mTime <= b.mTime; }))
                {
                    DEBUG_ASSERT(0, "acc to assimp doc it should already be sorted on time");//http://assimp.sourceforge.net/lib_html/structai_node_anim.html
                    std::sort(m_animMap[animName]->scalingKeys.begin(), m_animMap[animName]->scalingKeys.end());
                }

                break;
            }
        }
    }

    return true;
}

bool AnimNode::CalculateLocalTransform(long double ms, const std::string & currentAnimation)
{
    if (m_animMap.empty())
    {
        m_localTransform = m_defTransform;
        return true;
    }

    long double ticks = MsToTicks(ms, currentAnimation);

    ticks = std::fmod(ticks, m_animMap.at(currentAnimation)->duration);
 
    float temp =(float) m_animMap.at(currentAnimation)->duration;

    if (m_animMap[currentAnimation]->positionKeys.size() > 0)
    {
        aiVectorKey pos = CalculateInterpolatedPosition(ticks, currentAnimation);
        mat4 posTransform = vmath::translate(float(pos.mValue.x), float(pos.mValue.y), float(pos.mValue.z));

        aiVectorKey scale = CalculateInterpolatedScaling(ticks, currentAnimation);
        mat4 scaleTransform = vmath::scale(float(scale.mValue.x), float(scale.mValue.y), float(scale.mValue.z));

        aiQuatKey quat = CalculateInterpolatedRotation(ticks, currentAnimation);
        mat4 rotTransform = aiMatrix4x4ToVmathMat4(aiMatrix4x4t(quat.mValue.GetMatrix()));

        m_localTransform = posTransform * rotTransform * scaleTransform;
    }
    else
    {
        m_localTransform = m_defTransform;
    }
    return true;
}

//convert time in millisecond to ticks

long double AnimNode::MsToTicks(long double ms, const std::string & currentAnimation)
{
    long double s = ms / 1000; //time in seconds
    long double ticksPerSecond = (m_animMap.at(currentAnimation)->ticksPerSecond == 0) ? 25.0 : m_animMap.at(currentAnimation)->ticksPerSecond;//http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
    long double ticks = ticksPerSecond * s;
    return ticks;
}

aiVectorKey AnimNode::CalculateInterpolatedPosition(double ticks, const std::string & currentAnimation)//TODO: improvement : strict typedef for ticks, so that time in s or ms is never sent
{
    auto& posKeys = m_animMap[currentAnimation]->positionKeys;

    if (posKeys.size() == 0)
    {
        return aiVectorKey(ticks, aiVector3D(0.0, 0.0, 0.0));
    }

    if (posKeys.size() == 1)
    {
        return posKeys[0];
    }

    __int64 nextFrame = 0;
    //for (nextFrame = 0; nextFrame < posKeys.size(); nextFrame++)
    //{
    //    if (posKeys[nextFrame].mTime > ticks)
    //    {
    //        break;
    //    }
    //}

    auto nextFI = std::upper_bound(posKeys.begin(), posKeys.end(), aiVectorKey(ticks, aiVector3D(0.0, 0.0, 0.0)), [](const aiVectorKey& a, const aiVectorKey& b) -> bool
        {
            return a.mTime < b.mTime;
        });

    if (nextFI == posKeys.end())
    {
        nextFrame = 0;
    }
    else
    {
        nextFrame = nextFI - posKeys.begin();
    }

    if (0 == nextFrame)//not even 1st frame time reached
    {
        return posKeys[0];
    }

    if (posKeys.size() == nextFrame)//loop got over before "posKeys[nextFrame].mTime > ticks" condition satisfied
    {
        return *(posKeys.end() - 1);
    }

    __int64 prevFrame = nextFrame - 1;//next frame lies somewhere in (0, and posKeys.size() - 1)

    long double timeGapTicks = posKeys[nextFrame].mTime - posKeys[prevFrame].mTime;
    DEBUG_ASSERT(timeGapTicks >= 0, "time gap cannot be negative");

    float ratio =(float) ((ticks - posKeys[prevFrame].mTime) / timeGapTicks);
    DEBUG_ASSERT(ratio >= 0.0 && ratio <= 1.0, "ratio out of range");

    aiVector3D posV = ratio * posKeys[nextFrame].mValue + (1 - ratio)*posKeys[prevFrame].mValue;

    return aiVectorKey{ ticks, posV };
}

aiVectorKey AnimNode::CalculateInterpolatedScaling(double ticks, const std::string & currentAnimation)//TODO: improvement : strict typedef for ticks, so that time in s or ms is never sent
{
    auto& scaleKeys = m_animMap[currentAnimation]->scalingKeys;

    if (scaleKeys.size() == 0)
    {
        return aiVectorKey(ticks, aiVector3D(1.0, 1.0, 1.0));
    }

    if (scaleKeys.size() == 1)
    {
        return scaleKeys[0];
    }

    __int64 nextFrame = 0;
    //for (nextFrame = 0; nextFrame < scaleKeys.size(); nextFrame++)
    //{
    //    if (scaleKeys[nextFrame].mTime > ticks)
    //    {
    //        break;
    //    }
    //}

    auto nextFI = std::upper_bound(scaleKeys.begin(), scaleKeys.end(), aiVectorKey(ticks, aiVector3D(1.0, 1.0, 1.0)), [](const aiVectorKey& a, const aiVectorKey& b) -> bool
        {
            return a.mTime < b.mTime;
        });
    
    if (nextFI == scaleKeys.end())
    {
        nextFrame = 0;
    }
    else
    {
        nextFrame = nextFI - scaleKeys.begin();
    }

    if (0 == nextFrame)//not even 1st frame time reached
    {
        return scaleKeys[0];
    }

    if (scaleKeys.size() == nextFrame)//loop got over before "posKeys[nextFrame].mTime > ticks" condition satisfied
    {
        return *(scaleKeys.end() - 1);
    }

    __int64 prevFrame = nextFrame - 1;//next frame lies somewhere in (0, and posKeys.size() - 1)

    long double timeGapTicks = scaleKeys[nextFrame].mTime - scaleKeys[prevFrame].mTime;
    DEBUG_ASSERT(timeGapTicks >= 0, "time gap cannot be negative");

    float ratio =(float)( (ticks - scaleKeys[prevFrame].mTime) / timeGapTicks);
    DEBUG_ASSERT(ratio >= 0.0 && ratio <= 1.0, "ratio out of range");

    aiVector3D posV = ratio * scaleKeys[nextFrame].mValue + (1 - ratio)*scaleKeys[prevFrame].mValue;

    return aiVectorKey{ ticks, posV };
}

aiQuatKey AnimNode::CalculateInterpolatedRotation(double ticks, const std::string & currentAnimation)//TODO: improvement : strict typedef for ticks, so that time in s or ms is never sent
{
    auto& rotationKeys = m_animMap[currentAnimation]->rotationKeys;

    if (rotationKeys.size() == 0)
    {
        return aiQuatKey(ticks, aiQuaternion(0.0, 0.0, 0.0));
    }

    if (rotationKeys.size() == 1)
    {
        return rotationKeys[0];
    }

    __int64 nextFrame = 0;
    //for (nextFrame = 0; nextFrame < rotationKeys.size(); nextFrame++)
    //{
    //    if (rotationKeys[nextFrame].mTime > ticks)
    //    {
    //        break;
    //    }
    //}

    auto nextFI = std::upper_bound(rotationKeys.begin(), rotationKeys.end(), aiQuatKey(ticks, aiQuaternion(0.0, 0.0, 0.0)), [](const aiQuatKey& a, const aiQuatKey& b) -> bool
        {
            return a.mTime < b.mTime;
        });
    
    if (nextFI == rotationKeys.end())
    {
        nextFrame = 0;
    }
    else
    {
        nextFrame = nextFI - rotationKeys.begin();
    }


    if (0 == nextFrame)//not even 1st frame time reached
    {
        return rotationKeys[0];
    }

    if (rotationKeys.size() == nextFrame)//loop got over before "posKeys[nextFrame].mTime > ticks" condition satisfied
    {
        return *(rotationKeys.end() - 1);
    }

    __int64 prevFrame = nextFrame - 1;//next frame lies somewhere in (0, and posKeys.size() - 1)

    long double timeGapTicks = rotationKeys[nextFrame].mTime - rotationKeys[prevFrame].mTime;
    DEBUG_ASSERT(timeGapTicks >= 0, "time gap cannot be negative");

    float ratio = float((ticks - rotationKeys[prevFrame].mTime) / timeGapTicks);
    DEBUG_ASSERT(ratio >= 0.0 && ratio <= 1.0, "ratio out of range");

    aiQuaternion res(0.0,0.0,0.0);
    aiQuaternion::Interpolate(res, rotationKeys[prevFrame].mValue, rotationKeys[nextFrame].mValue, ratio);

    return aiQuatKey(ticks, res);
}



vmath::mat4 & AnimNode::GetOffsetMatrixRef()
{
    return m_offsetMatrix;
}
