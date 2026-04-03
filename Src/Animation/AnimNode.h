#pragma once
#include "vmath.h"
#include <string>
#include <vector>
#include <assimp/scene.h>
#include <memory>
#include <map>

/*
* An object like human skeleton which has to be animated is represented by a DAC - directed acyclic graph
* of objects. A node may represent a bone/joint or just a point in hierarchy not directly representing anything in object.
* Each node of graph will represent transformations which will be applied to all children, eg. children of hip joint would be two legs, any transformation to hip joint will apply to the legs too.
* For animation we have to store complete node hierarchy since a node may not be a bone, but it may have transformations
* which may affect bones further down the tree.
*/

struct AnimInfo
{
    std::vector<aiVectorKey> positionKeys;
    std::vector<aiQuatKey>   rotationKeys;
    std::vector<aiVectorKey> scalingKeys;
    double duration;
    long double ticksPerSecond;
};

#include <optional>

enum class AnimNodeType
{
    BONE,
    OTHER
};

//counter part of aiNode. This is needed since aiNode lacks m_combinedTransform matrix
class AnimNode
{
    friend class AnimModel;
protected:
    std::string m_name;
    /// local transform of node wrt its parent //nomenclature from http://mathinfo.univ-reims.fr/image/dxMesh/extra/d3dx_skinnedmesh.pdf
    vmath::mat4 m_localTransform;
    /// total transform of node - constaining affects of transformations of all ancestors//nomenclature from http://mathinfo.univ-reims.fr/image/dxMesh/extra/d3dx_skinnedmesh.pdf
    vmath::mat4 m_combinedTransform;

    vmath::mat4 m_defTransform; //aiNode::mTransformation - this is to be used for local transform when there are no animation keys for a node/bone

    std::shared_ptr<AnimNode> m_parent = nullptr;
    aiBone* m_linkedBone = nullptr;
    std::vector<std::shared_ptr<AnimNode>> m_children;
    AnimNodeType m_nodeType = AnimNodeType::OTHER;

    /*
   * anim name with animation info for this bone
   * for eg. m_animMap["walk"], if there is an animation named
   * walk will give all the transformations required for it
   * as far as this bone is concerned
   */
    std::map<std::string, std::shared_ptr<AnimInfo>> m_animMap;

    vmath::mat4 m_offsetMatrix; //aiBone::mOffsetMatrix - "Matrix that transforms from mesh space to bone space in bind pose" - from doc - http://assimp.sourceforge.net/lib_html/structai_bone.html

public:

    std::string& GetNameRef();

    std::string GetNameCopy();

    AnimNode();

    std::shared_ptr<AnimNode> GetParent() const;
    void SetParent(std::shared_ptr<AnimNode> pParent);

    bool ClearBone();

    bool AttachBone(aiBone* bone);

    aiBone* GetAttachedBone();

    AnimNodeType GetNodeTypeCopy() const;
    AnimNodeType& GetNodeTypeRef();
    void SetNodeType(AnimNodeType nodeType);

    vmath::mat4 GetLocalTransformCopy() const;

    vmath::mat4& GetLocalTransformRef();

    vmath::mat4 GetCombinedTransformCopy() const;

    vmath::mat4& GetCombinedTransformRef();

    vmath::mat4& GetDefTransformRef();
    vmath::mat4 GetDefTransformCopy();

    std::vector<std::shared_ptr<AnimNode>>& GetChildrenRef();

    /*
    * process animation related data for this bone
    * @param[IN] aiAnim          : aiScene::mAnimations
    * @param[IN] numAnimations   : number of animations
    *                              each animation will be of a particular kind
    *                              like walk, run fly, etc.
    */
    bool ProcessAnimation(aiAnimation** aiAnim, unsigned int numAnimations);


    bool CalculateLocalTransform(long double ms, const std::string& currentAnimation);

    //convert time in millisecond to ticks
    long double MsToTicks(long double ms, const std::string& currentAnimation);

    aiVectorKey CalculateInterpolatedPosition(double ticks, const std::string& currentAnimation);
    aiVectorKey CalculateInterpolatedScaling(double ticks, const std::string& currentAnimation);
    aiQuatKey CalculateInterpolatedRotation(double ticks, const std::string& currentAnimation);

    vmath::mat4& GetOffsetMatrixRef();
};

