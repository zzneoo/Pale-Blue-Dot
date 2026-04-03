#pragma once
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "AnimNode.h"
#include <map>
#include "util.h"
#include "AnimMesh.h"
#include <fstream>
#include <sstream>
#include <memory>
#include <filesystem>

class AnimModel;
class AnimMesh;

static vmath::mat4 aiMatrix4x4ToVmathMat4(aiMatrix4x4 aiMat)
{
    return mat4(
        vec4(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1),
        vec4(aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2),
        vec4(aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3),
        vec4(aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4)
    );
}


class AnimModel
{
    //DO NOT DELETE m_scene. A AnimNode, and other classes refer to data directly within it - perf - save data copyoverhead
    //Gets cleaned up when Assimp::Importer m_importer desctructor is called;
    const aiScene* m_scene = nullptr;
    Assimp::Importer m_importer;
    std::shared_ptr<AnimNode> m_rootNode;
    std::string m_modelDir;

    /*
    * @brief : A mesh represents a connected set of vertices.
    * For eg. for a human body, the eye, nose, torso, etc. may be inidividual meshes.
    */
    std::map<std::string, AnimMesh> m_meshes;

    /*
    * @brief : for direct access to a node. save reference to it.
    * bones, etc. refer to node by name, this would help speed up the lookup.
    */
    std::map<std::string, std::shared_ptr<AnimNode>> m_nodeMap;

    /*
    * @brief : each aiAnimation can have one type of enimation.
    *          eg. walk, run, fly, etc.
    *          The animation will be in its entirety.
    *          This means that for eg. walk, all the transformations
    *          required for it would be in one aiAnimation object
    */
    //std::map<std::string, std::shared_ptr<AnimInfo>> m_animMap;

    std::string m_currentAnimation;//name of current animation - walk, run, etc.
    std::vector<std::string> m_animationNames; //to quickly lookup if m_currentAnimation being set is correct or not
    
    std::vector<GLuint> m_textures;
    std::map<aiTextureType /*tex type*/, std::string /*tex path*/> m_defaultTextures;//some models provide texture coordinate but the texture image information itself is not given, can use an arbitrary image in that case

    void LoadTextures();
    /*
    * @param[IN] index : index of m_textures where glentextures ID will be stored for the texture given at path
    */
    void LoadTextureFromPath(const std::string& filePath, const int index);
public:
    AnimModel();

    void SetDefaultTexture(aiTextureType type, std::string path);

    GLuint GetTexture(unsigned int index);

    std::map<std::string, AnimMesh>& Get_m_meshesRef(void)
    {
        return m_meshes;
    }

    std::map<std::string, std::shared_ptr<AnimNode>>& GetNodeMapRef();


    /* Output dot graphvix graph which can be used to create svg, png, etc. representation of the Node tree
    eg. cat c.graph|dot -Tsvg -o c.svg
    eg. http://www.webgraphviz.com/
    Helpful to view hierarchy of nodes.
    This function is static, so that classes derived from AnimNode could also use it.
    @param[IN] path : path to output file
    @param[IN] rootNode : root of tree
    */
    static void DumpGraphvizDotFile(std::string path, std::shared_ptr<AnimNode> rootNode);

    std::shared_ptr<AnimNode> GetRootNode();

    //Before a new skeleton is created by AnimMesh, any linkages to bones of any other mesh must be removed
    void ClearAllLinkedBonesFromNode();

    std::shared_ptr<AnimNode> GetNodeByName(const std::string& nodeName);
    bool LoadModel(std::string filePath);
    bool ParseModel();

    bool ProcessMeshes();

    //time in millisecond
    void RenderMeshes(long double ms);

    //fill data from aiNode into animNode
    bool ProcessNode(std::shared_ptr<AnimNode> animNode, aiNode* aiNode, std::shared_ptr <AnimNode> pParent);

    bool SetCurrentAnimation(std::string animName);

    const aiScene* GetAiScene();

private:
    //called from AnimModel::ParseModel();
    bool ProcessAnimation();

    //output graph for just a single node
    //iD : give each node a unique identity in graph, since some nodes may have common names
    static void OutputNodeGraph(std::shared_ptr<AnimNode> animNode, int& iD, std::stringstream& graphStructure, std::stringstream& graphAttributes);
};

