#include "AnimModel.h"
#include "debugOfstream.h"
#include <filesystem>

void AnimModel::LoadTextures()
{
    unsigned int numTextures = m_scene->mNumMaterials;
    m_textures = std::vector<GLuint>(numTextures);

    for (unsigned int i = 0; i < numTextures; i++)
    {
        aiMaterial* texture = m_scene->mMaterials[i];

        if (texture->GetTextureCount(aiTextureType_DIFFUSE) == 0)
        {
#ifdef NDEBUG
            {
                unsigned int sum = 0;
                for (int tex = 0; tex < 19; tex++)
                {
                    auto num = texture->GetTextureCount(aiTextureType(tex));
                    sum += num;
                    DEBUG_VSCONSOLE("tex: " + std::to_string(tex) + " num:" + std::to_string(num));
                }
                DEBUG_VSCONSOLE("sum of all texture types: " + std::to_string(sum));
            }
#endif
            DEBUG_VSCONSOLE("numTextures > 0, but of type aiTextureType_DIFFUSE is 0. See VS output console");
        }

        if (texture->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;
            texture->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            std::wstring fileW = std::filesystem::path(path.C_Str()).filename().c_str();
            std::string filePath = m_modelDir + W2STR(fileW);

            LoadTextureFromPath(filePath, i);
        }
        else if (m_defaultTextures.find(aiTextureType_DIFFUSE) != m_defaultTextures.end())
        {
            LoadTextureFromPath(m_defaultTextures.find(aiTextureType_DIFFUSE)->second, i);
        }
        else
        {
            m_textures[i] = GLuint(-1);
        }
    }
}

void AnimModel::LoadTextureFromPath(const std::string& filePath, const int index)
{
    DEBUG_ASSERT(std::filesystem::exists(filePath), "model texture file not found! Path not found");

    BITMAP bmp = Util::LoadTextureFromFile(filePath);

    if (nullptr == bmp.bmBits)
    {//if the texture image given with model is not .bmp, then place a bmp version of it at same path as texture
        std::wstring fileW = std::filesystem::path(filePath).replace_extension(".bmp").c_str();
        std::string filePath2 = W2STR(fileW);
        DEBUG_ASSERT(std::filesystem::exists(filePath2), ".bmp version of model texture file not found! It should be in same directory as model");
        bmp = Util::LoadTextureFromFile(filePath2);
        DEBUG_ASSERT(nullptr != bmp.bmBits, "failed to load even bmp version of texture. Texture loading failed!");
    }

    glGenTextures(1, &m_textures[index]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, m_textures[index]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

AnimModel::AnimModel()
{
    m_rootNode.reset(new AnimNode);
}

void AnimModel::SetDefaultTexture(aiTextureType type, std::string path)
{
    m_defaultTextures[type] = path;
}

GLuint AnimModel::GetTexture(unsigned int index)
{
    return m_textures[index];
}

std::map<std::string, std::shared_ptr<AnimNode>>& AnimModel::GetNodeMapRef()
{
    return m_nodeMap;
}

void AnimModel::DumpGraphvizDotFile(std::string path, std::shared_ptr<AnimNode> rootNode)
{
    debugOfstream of(path, std::ios::trunc);
    std::string graphName = "AnimModelNodes";

    if (rootNode->m_name.length() > 0)
    {
        graphName += rootNode->m_name;
    }

    of << "digraph " << graphName << " { " << std::endl;

    std::stringstream graphStructure, graphAttributes;
    int iD = 0;
    OutputNodeGraph(rootNode, iD, graphStructure, graphAttributes);
    of << R"(//attributes)" << std::endl;
    of << graphAttributes.str() << std::endl;
    of << R"(//grah structure)" << std::endl;
    of << graphStructure.str() << std::endl;
    of << " } " << std::endl;
    of.close();
}

//output graph for just a single node
//iD : give each node a unique identity in graph, since some nodes may have common names

void AnimModel::OutputNodeGraph(std::shared_ptr<AnimNode> animNode, int& iD, std::stringstream& graphStructure, std::stringstream& graphAttributes)
{
    std::string nodeName = std::to_string(iD);
    std::string nodeAttributes;
    std::string  nodeLabel = animNode->m_name.c_str();
    if (nodeLabel.length() > 0)
    {
        nodeAttributes += " label=" + nodeLabel;
    }
    else
    {
        nodeAttributes += " label=NULL";
    }

    if (AnimNodeType::BONE == animNode->GetNodeTypeCopy())
    {
        nodeAttributes += " shape=box";
    }

    if (nodeAttributes.length() > 0)
    {
        graphAttributes << nodeName << "[ " + nodeAttributes + " ]" << std::endl;
    }

    if (0 == animNode->m_children.size() && animNode->m_parent == nullptr)
    {
        graphStructure << R"(")" << nodeName << R"(")" << std::endl;
    }

    for (int i = 0; i < animNode->m_children.size(); i++)
    {
        graphStructure << R"(")" << nodeName << R"(")";
        graphStructure << " -> ";
        graphStructure << R"(")" << std::to_string(iD + 1) << R"(")";
        graphStructure << std::endl;
        OutputNodeGraph(animNode->m_children[i], ++iD, graphStructure, graphAttributes);
    }

    graphStructure << std::endl;
}


std::shared_ptr<AnimNode> AnimModel::GetRootNode()
{
    return m_rootNode;
}

void AnimModel::ClearAllLinkedBonesFromNode()
{
    for (auto& animNode : m_nodeMap)
    {
        animNode.second->ClearBone();
    }
}

std::shared_ptr<AnimNode> AnimModel::GetNodeByName(const std::string& nodeName)
{
    auto itr = m_nodeMap.find(nodeName);
    if (m_nodeMap.end() == itr)
    {
        THROW_DEBUG_EXCEPTION("node not found")
            return nullptr;
    }
    return itr->second;
}

bool AnimModel::LoadModel(std::string filePath)
{
   // m_scene = m_importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality );// ERROR:aiProcessPreset_TargetRealtime_MaxQuality causes some meshes to have non multiple of 3 faces even after using aiProcess_Triangulate
    m_scene = m_importer.ReadFile(filePath, aiProcess_Triangulate| aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace );
   

    if (!std::filesystem::exists(filePath))
    {
        THROW_DEBUG_EXCEPTION("model file does not exist")
    }

    if (nullptr == m_scene)
    {
        THROW_DEBUG_EXCEPTION("failed to load assimp model")
        return false;
    }

    std::wstring mdir = std::filesystem::path(filePath).remove_filename().c_str();
    m_modelDir.assign(mdir.begin(), mdir.end());
    ParseModel();
    LoadTextures();
    return true;
}

bool AnimModel::ParseModel()
{
    DEBUG_ASSERT(nullptr != m_scene, "m_scene is null. LoadModel() not called/failed?");

    ProcessNode(m_rootNode, m_scene->mRootNode, nullptr);

    //node hierarchy ready
    ProcessMeshes();
    //each mesh has a skeletal structure now, and doesn't need to refer to node hierarchy
    //shaders, vao, etc. have been set up

    //DumpGraphvizDotFile("originalNodeTree.dot", m_rootNode);

    ProcessAnimation();
    return true;
}

bool AnimModel::ProcessMeshes()
{
    for (unsigned int i = 0; i < m_scene->mNumMeshes; i++)
    {
        aiMesh* mesh = m_scene->mMeshes[i];
        std::string meshName = mesh->mName.C_Str();
        meshName += std::to_string(i); //meshes may have duplicated names, make the names unique

        m_meshes[meshName] = AnimMesh();
        m_meshes[meshName].m_aiMeshRef = mesh;
        m_meshes[meshName].Attach(this);
        m_meshes[meshName].Process();
    }
    return true;
}

bool AnimModel::ProcessAnimation()
{
    DEBUG_ASSERT(nullptr != m_scene, "m_scene is null. LoadModel() not called/failed?");

    if (!m_scene->HasAnimations() || m_scene->mNumAnimations == 0)
    {
        return false;
    }

    //sanity check - anim names should not be empty
    for (unsigned int i = 0; i < m_scene->mNumAnimations; i++)
    {
        if (m_scene->mAnimations[i]->mName.length == 0)
        {
            std::string animName = "ASSIMP_ANIMATION" + STR(i);
            DEBUG_VSCONSOLE("Animation name was empty. Name given to animation : " + animName);
            m_scene->mAnimations[i]->mName = aiString(animName);
        }
        m_animationNames.push_back(m_scene->mAnimations[i]->mName.C_Str());
    }

    for (auto& mesh : m_meshes)
    {
        mesh.second.ProcessAnimation(m_scene->mAnimations, m_scene->mNumAnimations);
    }

    return true;
}

void AnimModel::RenderMeshes(long double ms)
{

    for(auto& mesh : m_meshes)
    {
        mesh.second.Render(ms, m_currentAnimation);//TODO: add code for current animation
    }

}

//fill data from aiNode into animNode

bool AnimModel::ProcessNode(std::shared_ptr<AnimNode> animNode, aiNode* aiNode, std::shared_ptr<AnimNode> pParent)
{
    if (nullptr == aiNode)
    {
        return false;
    }

    if (!animNode)
    {
        THROW_DEBUG_EXCEPTION("animNode node memory not allocated");
    }

    animNode->m_name = aiNode->mName.C_Str();
    animNode->m_localTransform = mat4::identity();
    animNode->m_combinedTransform = mat4::identity();
    animNode->m_defTransform = aiMatrix4x4ToVmathMat4(aiNode->mTransformation);

    animNode->m_parent = pParent;

    m_nodeMap[animNode->m_name] = animNode;

    if (aiNode->mNumChildren > 0)
    {
        animNode->m_children.reserve(aiNode->mNumChildren);
        for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
        {
            std::shared_ptr<AnimNode> child(new AnimNode);
            animNode->m_children.push_back(child);
            ProcessNode(animNode->m_children[i], aiNode->mChildren[i], animNode);
        }
    }

    return true;
}

bool AnimModel::SetCurrentAnimation(std::string animName)//TODO: add check to verify this string exists in current list of animations
{
    //sanity check - check if animation exists - when changing model, dev may forget to change animation name;
    auto itr = std::find(m_animationNames.begin(), m_animationNames.end(), animName);
    if (m_animationNames.end() == itr)
    {
        std::string animList;
        for (auto& anim : m_animationNames)
        {
            animList += anim;
            animList += ", ";
        }
        DEBUG_ASSERT(false, ("SetCurrentAnimation() no such animation!. Possible values: " + animList + " Trying to set: " + animName).c_str());
    }
    m_currentAnimation = animName;
    return true;
}

const aiScene * AnimModel::GetAiScene()
{
    return m_scene;
}
