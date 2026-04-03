#pragma once
#include"stdafx.h"
#include "AnimBase.h"
#include "AnimModel.h"
#include <type_traits>

class AnimModel;
class AnimMesh;

#include <regex>
#include "debugOfstream.h"

//aiMesh wrapper. Contains self contained skeletal structure consiting of AnimBone tree.
// Once this is created, there should be no need to refer to AnimNode, or aiNode again and again.
class AnimMesh : AnimBase
{
    friend class AnimModel;
    aiMesh* m_aiMeshRef;//A direct reference from aiScene data structure, do not delete the original DS till this object is in use
    std::map<std::string, std::shared_ptr<AnimNode>> m_boneMap;//we need to remember the order in which we got aiBones from aiMesh. keep puchin name to m_boneNames as it comes. Laer when creating AnimBone objects, keep adding them to m_boneMap. Thereafter m_boneNames, m_boneMap together can determine order for AnimBone objects. it is in this order that we have to fill boneTransformation[] matrices uniform to be sent to shaders. 
    std::vector<std::string> m_boneNames;//see m_boneMap doc

 #pragma region vertex data 
/*Initially I had a separate class for vertex - AnimVertex, but that was making data copy more time consuming since first data used to be filled in m_vertex vector for each vertex, and then while sending to open GL, it was required to create position, normal, etc. arrays each cotaining data from all vertices - glBufferData()*/
    std::vector<V3GLf> m_vPosition;
    std::vector<V2GLf> m_vTexCoord; //texture coordinates
    std::vector<V3GLf> m_vNormal;
    std::vector<V3GLf> m_vTangent;
    std::vector<V4GLi> m_vJointIndices; //Integer. which joints have an effect on this vertex
    std::vector<V4GLf> m_vJointWeights;
    unsigned int m_textureIndex;//aiMesh::mMaterialIndex - which texture to use

    /*before copying data into m_vJointIndices, and m_vJointWeights, copy data to m_vJointIndicesWeights_shadow,
    * then sort on the basis of weights descending, and take first CONFIG_MAX_BONES_PER_VERTEX entries.
    * m_vJointIndicesWeights_shadow[0] -> info for 0th vertex in the form of <weightVal, boneIndex> - bone at boneIndex index affects
    * 0th vertex by weightVal weight
    */
    std::vector< std::vector<std::pair<float, int>>> m_vJointIndicesWeights_shadow; //Integer. which joints have an effect on this vertex
  
#pragma endregion

    std::vector<GLuint> m_indices;//assuming all faces are triangles because of aiProcess_Triangulate flag

    std::shared_ptr<AnimNode> m_skeletonRoot;//root bone in the mesh marks the root of the skeleton
    unsigned int m_numBones = 0; //number of bones in the mesh

    AnimModel* m_animModel = nullptr;//reference to container AnimModel class to access node data

    /*
    * ProcessVertices() has to be called before ProcessBones(), since ProcessBones() adds info to already created m_vertices.
    */
    bool m_vertexProcessingDone = false;

    //OGL data
    GLuint m_shaderProgramObject = 0;

    GLuint m_u_m_matrix                    = (GLuint)-1;//model matrix uniform location
    GLuint m_u_v_matrix                    = (GLuint)-1;
    GLuint m_u_p_matrix                    = (GLuint)-1;
    GLuint m_u_jointTransforms_location    = (GLuint)-1;
    GLuint m_u_gColorTexture               = (GLuint)-1;

    GLuint m_vao = 0;//vertex vao
    GLuint m_vboPosition = 0;
    GLuint m_vboTexCoord = 0;
    GLuint m_vboNormals = 0;
    GLuint m_vboTangents = 0;
    GLuint m_vboJointIndices = 0;
    GLuint m_vboJointWeights = 0;
    GLuint m_vboFaceIndices = 0;

    std::shared_ptr<mat4[]> m_u_jointTransforms_Matrices;
    //OGL data end
    vmath::mat4 m_globalInverserTransform;//http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
public:
    AnimMesh()
    {
        m_skeletonRoot.reset(new AnimNode);
        m_globalInverserTransform = vmath::mat4::identity();
        m_textureIndex = (decltype(m_textureIndex))-1;
    }
    ~AnimMesh()
    {
        //GLuint m_vao = 0;//vertex vao

        if (m_vao)
        {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }

        if (m_vboPosition)
        {
            glDeleteBuffers(1, &m_vboPosition);
            m_vboPosition = 0;
        }
        if (m_vboTexCoord)
        {
            glDeleteBuffers(1, &m_vboTexCoord);
            m_vboTexCoord = 0;
        }
        if (m_vboNormals)
        {
            glDeleteBuffers(1, &m_vboNormals);
            m_vboNormals = 0;
        }

        if (m_vboTangents)
        {
            glDeleteBuffers(1, &m_vboTangents);
            m_vboTangents = 0;
        }
        if (m_vboJointIndices)
        {
            glDeleteBuffers(1, &m_vboJointIndices);
            m_vboJointIndices = 0;
        }
        if (m_vboJointWeights)
        {
            glDeleteBuffers(1, &m_vboJointWeights);
            m_vboJointWeights = 0;
        }
        if (m_vboFaceIndices)
        {
            glDeleteBuffers(1, &m_vboFaceIndices);
            m_vboFaceIndices = 0;
        }
    }

    void CalculateCombinedTransforms(std::shared_ptr<AnimNode> node);

    // Inherited via AnimBase
    virtual void Process() override;

    /*
    The data assimp gives us has with each bone, the vertices it affects,
    we are interested in computing the opposite - which bones affect a particular vertex.
    The data should be at vertex, rather than at bone, like now.
    This is how thin matrix video on skeletal animation does it.
    */
    void ProcessVertices(bool& retflag);

    /*build skeleton
    Logic -
    go through list of bones of the mesh, keep on attaching them to corresponding nodes.
    nodes have hierarchy information, bones (aiBones) do not.
    Once all bones are attached, parse nodes from top, creating one bone in skeleton for each node
    which has a bone attached.
    aiProcess_PopulateArmatureData seems to help in this, but I haven't been able to figure out how.
    ProcessVertices() should be called before this
    */
    void ProcessBones(bool& retflag);

    void Attach(AnimModel* animModel);

    //Create vao, vbo, etc. for all vertex properties - positions, normals, etc.
    GLuint CreateVertexVao();

    //create shader program, and get uniform locations
    bool InitShader();

    //ms : time in millisecond since start of animation
    void Render(long double ms, const std::string& currentAnimation);
    void ZzRender(void);

    unsigned int GetNumBones()
    {
        return m_numBones;
    }

    std::shared_ptr<mat4[]> GetJointMatricesPointer(void)
    {
        return m_u_jointTransforms_Matrices;
    }

    /*
    * Compute skeletal transform of all the bones,
    * also fills m_u_jointTransforms_Matrices
    * @param[IN] ms: time in millisecond since start of animation
    */
    void ComputeSkeletonTransforms(long double ms, const std::string& currentAnimation);

    std::shared_ptr<AnimNode> GetBoneByName(const std::string& nodeName);

private:
    /*fill data from AnimNode to AnimBone, and process children
    * aiScene has a single note hierarchy rooted at aiScene::mRootNode
    * This is used for all meches in the model.
    * Thus this function creates a copy of the hierarchy for each mesh, to prevent contention with other meshes.
    * @ param[IN] bone    : The object of node hierarchy copy into which corrsponding nodes info will be copied. (The root of hierarchy of this mesh's copy for 1st call)
    * @param[IN] animNode :  The object from original node hierarchy.
    */
    void CopyNodeHierarchy(std::shared_ptr<AnimNode> bone, std::shared_ptr<AnimNode> animNode, std::shared_ptr<AnimNode> pParentBone);
    
    /*
    * @param[IN] ms: time in millisecond since start of animation
    */
    void ComputeSkeletonTransformsInternal(std::shared_ptr<AnimNode> bone, long double ms, const std::string& currentAnimation);

    /*
    * once we have done static model init - load vertex data, create voa, shaders, etc.
    * process animation related data
    * @param[IN] aiAnim          : aiScene::mAnimations
    * @param[IN] numAnimations   : number of animations 
    *                              each animation will be of a particular kind
    *                              like walk, run fly, etc.
    */
    bool ProcessAnimation(aiAnimation** aiAnim, unsigned int numAnimations);
};

