#include "AnimMesh.h"

#include "MyWin32.h"
#include "OpenGLState.h"
#include "ZzRenderer.h"

void AnimMesh::CalculateCombinedTransforms(std::shared_ptr<AnimNode> node)
{
    if (node->GetParent())
    {
        node->GetCombinedTransformRef() = node->GetLocalTransformRef() * node->GetParent()->GetCombinedTransformRef();
    }
    else//root node
    {
        node->GetCombinedTransformRef() = node->GetLocalTransformRef();
    }
}

void AnimMesh::Process()
{
    if (nullptr == m_animModel)
    {
        THROW_DEBUG_EXCEPTION("m_animModel is null. Attach() not called?");
    }

    bool retflag;
    ProcessVertices(retflag);
    DEBUG_ASSERT(retflag == false, "ProcessVertices() failed")
        ProcessBones(retflag);
    DEBUG_ASSERT(retflag == false, "ProcessVertices() failed")
        if (retflag) return;

    InitShader();
    CreateVertexVao();

    AnimModel::DumpGraphvizDotFile("bonesTree_new.dot", m_skeletonRoot);
}

void AnimMesh::ProcessVertices(bool& retflag)
{
    auto numVertices = m_aiMeshRef->mNumVertices;
    m_vPosition = std::vector<V3GLf>(numVertices);
    m_vTexCoord = std::vector<V2GLf>(numVertices);
    m_vNormal = std::vector<V3GLf>(numVertices);
    m_vTangent = std::vector<V3GLf>(numVertices);
    m_vJointIndices = std::vector<V4GLi>(numVertices);
    m_vJointWeights = std::vector<V4GLf>(numVertices);
    m_vJointIndicesWeights_shadow = std::vector<std::vector<std::pair<float, int>>>(numVertices);

    for (unsigned int i = 0; i < numVertices; i++)
    {
        aiVector3D aiVert = m_aiMeshRef->mVertices[i];
        m_vPosition[i].i() = aiVert[0];
        m_vPosition[i].j() = aiVert[1];
        m_vPosition[i].k() = aiVert[2];

        aiVector3D aiNormals = m_aiMeshRef->mNormals[i];
        m_vNormal[i] = V3GLf{ aiNormals[0], aiNormals[1] , aiNormals[2] };

        aiVector3D aiTangents = m_aiMeshRef->mTangents[i];
        m_vTangent[i] = V3GLf{ aiTangents[0],aiTangents[1],aiTangents[2] };

        DEBUG_ASSERT(m_aiMeshRef->HasTextureCoords(1) == false, "multi texturing not supported");

        if (m_aiMeshRef->HasTextureCoords(0))
        {
            m_vTexCoord[i].i() = m_aiMeshRef->mTextureCoords[0][i].x;
            m_vTexCoord[i].j() = m_aiMeshRef->mTextureCoords[0][i].y;
            m_textureIndex = m_aiMeshRef->mMaterialIndex;
        }
    }

    for (unsigned int i = 0; i < m_aiMeshRef->mNumFaces; i++)
    {
        aiFace aiFace = m_aiMeshRef->mFaces[i];

        DEBUG_ASSERT(aiFace.mNumIndices == 3, "assumption: aiProcess_Triangulate flag is used, hence all faces should be triangles");

        for (unsigned int j = 0; j < aiFace.mNumIndices; j++)
        {
            m_indices.push_back(aiFace.mIndices[j]);
        }
    }
    m_vertexProcessingDone = true;
    retflag = false;
}

GLuint AnimMesh::CreateVertexVao()
{
    DEBUG_OGL_INFO("AnimMesh::CreateVertexVao enter");
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    {
        /*1. POSITION*/
        DEBUG_OGL_INFO("AnimMesh::CreateVertexVao POSITION data");
        glGenBuffers(1, &m_vboPosition);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
        {
            //sanity check - glBufferData() expects C like layout of data, this may not be true in case of virtual functions, etc. in C++ class.
            DEBUG_ASSERT(std::is_standard_layout<std::remove_reference<decltype(m_vPosition[0])>>::value, "glBufferData() is going to fail. class data not in contigous memory locations.");
            glBufferData(GL_ARRAY_BUFFER, m_vPosition.size() * sizeof(m_vPosition[0]), m_vPosition.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(ZZNEO_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(ZZNEO_ATTRIB_POSITION);
        }

        /*2. TEXTURE COORDINATES*/
        DEBUG_OGL_INFO("AnimMesh::CreateVertexVao TEXTURE data");
        glGenBuffers(1, &m_vboTexCoord);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
        {
            //sanity check - glBufferData() expects C like layout of data, this may not be true in case of virtual functions, etc. in C++ class.
            DEBUG_ASSERT(std::is_standard_layout<std::remove_reference<decltype(m_vTexCoord[0])>>::value, "glBufferData() is going to fail. class data not in contigous memory locations.");
            glBufferData(GL_ARRAY_BUFFER, m_vTexCoord.size() * sizeof(m_vTexCoord[0]), m_vTexCoord.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(ZZNEO_ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(ZZNEO_ATTRIB_TEXCOORD);
        }

        /*3. NORMALS*/
        DEBUG_OGL_INFO("AnimMesh::CreateVertexVao NORMAL data");
        glGenBuffers(1, &m_vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
        {
            //sanity check - glBufferData() expects C like layout of data, this may not be true in case of virtual functions, etc. in C++ class.
            DEBUG_ASSERT(std::is_standard_layout<std::remove_reference<decltype(m_vNormal[0])>>::value, "glBufferData() is going to fail. class data not in contigous memory locations.");
            glBufferData(GL_ARRAY_BUFFER, m_vNormal.size() * sizeof(m_vNormal[0]), m_vNormal.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(ZZNEO_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(ZZNEO_ATTRIB_NORMAL);
        }

        /*4. TANGENTS*/
        DEBUG_OGL_INFO("AnimMesh::CreateVertexVao TANGENT data");
        glGenBuffers(1, &m_vboTangents);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboTangents);
        {
            //sanity check - glBufferData() expects C like layout of data, this may not be true in case of virtual functions, etc. in C++ class.
            DEBUG_ASSERT(std::is_standard_layout<std::remove_reference<decltype(m_vTangent[0])>>::value, "glBufferData() is going to fail. class data not in contigous memory locations.");
            glBufferData(GL_ARRAY_BUFFER, m_vTangent.size() * sizeof(m_vTangent[0]), m_vTangent.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(ZZNEO_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(ZZNEO_ATTRIB_TANGENT);
        }

        /*5. JOINT INDICES*/
        DEBUG_OGL_INFO("AnimMesh::CreateVertexVao JOINT INDICES data");
        glGenBuffers(1, &m_vboJointIndices);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboJointIndices);
        {
            //sanity check - glBufferData() expects C like layout of data, this may not be true in case of virtual functions, etc. in C++ class.
            DEBUG_ASSERT(std::is_standard_layout<std::remove_reference<decltype(m_vJointIndices[0])>>::value, "glBufferData() is going to fail. class data not in contigous memory locations.");
            glBufferData(GL_ARRAY_BUFFER, m_vJointIndices.size() * sizeof(m_vJointIndices[0]), m_vJointIndices.data(), GL_STATIC_DRAW);
            glVertexAttribIPointer(ZZNEO_ATTRIB_JOINT_INDICES, 4, GL_INT, 0, nullptr);
            glEnableVertexAttribArray(ZZNEO_ATTRIB_JOINT_INDICES);
        }

        /*6. Joint Weights*/
        DEBUG_OGL_INFO("AnimMesh::CreateVertexVao JOINT WEIGHTS data");
        glGenBuffers(1, &m_vboJointWeights);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboJointWeights);
        {
            //sanity check - glBufferData() expects C like layout of data, this may not be true in case of virtual functions, etc. in C++ class.
            DEBUG_ASSERT(std::is_standard_layout<std::remove_reference<decltype(m_vJointWeights[0])>>::value, "glBufferData() is going to fail. class data not in contigous memory locations.");
            glBufferData(GL_ARRAY_BUFFER, m_vJointWeights.size() * sizeof(m_vJointWeights[0]), m_vJointWeights.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(ZZNEO_ATTRIB_JOINT_WEIGHTS, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(ZZNEO_ATTRIB_JOINT_WEIGHTS);
        }

        /*7. Face Indices*/
        DEBUG_OGL_INFO("AnimMesh::CreateVertexVao FACE INDICES data");
        glGenBuffers(1, &m_vboFaceIndices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboFaceIndices);
        {
            //sanity check - glBufferData() expects C like layout of data, this may not be true in case of virtual functions, etc. in C++ class.
            DEBUG_ASSERT(std::is_standard_layout<std::remove_reference<decltype(m_indices[0])>>::value, "glBufferData() is going to fail. class data not in contigous memory locations.");
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(m_indices[0]), m_indices.data(), GL_STATIC_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }
    glBindVertexArray(0);

    DEBUG_OGL_INFO("AnimMesh::CreateVertexVao exit");
    return m_vao;
}

bool AnimMesh::InitShader()
{
    DEBUG_OGL_INFO("AnimMesh::InitShader enter");
    GLuint vertexShaderObject = 0;
    GLuint fragmentShaderObject = 0;

    /*1. VERTEX SHADER*/
    std::string vertexShaderSourceCode =
        R"(#version 450 core

                #define CONFIG_MAX_BONES_PER_VERTEX 4

                in vec4  vPosition;
                in vec2  vTexCoord; 
                in vec3  vNormal;
                in vec3  vTangent;
                in ivec4 vJointIndices;
                in vec4  vJointWeights;

                uniform mat4 u_m_matrix; //model matrix
                uniform mat4 u_v_matrix; //view matrix
                uniform mat4 u_p_matrix; //projection matrix
                
                uniform mat4 u_jointTransforms[REGEX_REPLACE_VAR_NUM_BONES1];//REGEX_REPLACE_VAR_NUM_BONES1 will be replaced by string substitution

                out vec3 out_normal;
                out vec3 out_light_direction_from_vertex;
                out vec2 out_textCoord;

                out vec3 color;  

                void main()
                {
                    vec4 u_lightPosition_vec4 = vec4(1.0,1.0,1.0,1.0);

                    mat4 mv_matrix = u_v_matrix * u_m_matrix;
                    
                    mat4 boneTransform = mat4(0.0);
                    for(int i = 0; i < CONFIG_MAX_BONES_PER_VERTEX; i++)
                    {
                        boneTransform += u_jointTransforms[vJointIndices[i]] * vJointWeights[i];
                    } 
                    
                    mat3 normalTransform = mat3(transpose(inverse(boneTransform)));
                    vec4 localPos        = boneTransform * vPosition;
                    vec3 localNormal     = normalTransform * vNormal;
                    vec4 eye_coords      = mv_matrix * localPos;
                    gl_Position = u_p_matrix * eye_coords;
                    out_normal = mat3(transpose(inverse(mv_matrix))) * localNormal;
                    out_light_direction_from_vertex = normalize(vec3( u_v_matrix*u_lightPosition_vec4 - eye_coords)); //variable name S in sir's code

                    out_textCoord = vTexCoord;

                 //   color=vTangent;
                }
                )";

    std::regex numBones("REGEX_REPLACE_VAR_NUM_BONES1");
    vertexShaderSourceCode = std::regex_replace(vertexShaderSourceCode, numBones, std::to_string(m_numBones));
    DEBUG_ASSERT(m_numBones != 0, "m_numBones : no bones in this mesh of the model ?");

    vertexShaderObject = Util::generateShader(GL_VERTEX_SHADER, vertexShaderSourceCode.c_str());

    /*2. FRAGMENT SHADER*/
    const GLchar* fragmentShaderSourceCode =
        R"(#version 450 core

               in vec3 out_normal;
               in vec3 out_light_direction_from_vertex;
               in vec2 out_textCoord;

               in vec3 color;

               uniform sampler2D u_gColorTexture;

               out vec4 FragColor;
               uniform mat4 u_v_matrix; //view matrix
               void main()
                {
                    vec3 u_LD_vec3 = vec3(1.0,1.0,1.0);
                    vec3 u_KD_vec3 = vec3(0.5,0.0,0.0);

                    vec3 out_normalN = normalize(out_normal);
                    vec3 light_direction_from_vertexN = normalize(out_light_direction_from_vertex);
                    //vec3 diffuse_color = u_LD_vec3*u_KD_vec3*max(dot(light_direction_from_vertexN, out_normalN), 0.0f); 
                    vec3 diffuse_color = abs(u_LD_vec3*u_KD_vec3*dot(light_direction_from_vertexN, out_normalN)) + vec3(0.1,0.0,0.0); //sahils-debug remove this
                    FragColor = vec4(diffuse_color, 1.0);  
                    //FragColor = vec4(1.0);  

                       vec4 texCol = texture(u_gColorTexture, out_textCoord);
                      FragColor = texCol*dot(out_normalN, vec3(1.0))*0.9 + texCol*0.1;
                       FragColor.a=0;
                     //  FragColor.rgb=color;
                }
                )";
    fragmentShaderObject = Util::generateShader(GL_FRAGMENT_SHADER, fragmentShaderSourceCode);

    m_shaderProgramObject = glCreateProgram();

    glAttachShader(m_shaderProgramObject, vertexShaderObject);
    glAttachShader(m_shaderProgramObject, fragmentShaderObject);

    //pre link
    glBindAttribLocation(m_shaderProgramObject, ZZNEO_ATTRIB_POSITION, "vPosition");
    glBindAttribLocation(m_shaderProgramObject, ZZNEO_ATTRIB_TEXCOORD, "vTexCoord");
    glBindAttribLocation(m_shaderProgramObject, ZZNEO_ATTRIB_NORMAL, "vNormal");
    glBindAttribLocation(m_shaderProgramObject, ZZNEO_ATTRIB_TANGENT, "vTangent");
    glBindAttribLocation(m_shaderProgramObject, ZZNEO_ATTRIB_JOINT_INDICES, "vJointIndices");
    glBindAttribLocation(m_shaderProgramObject, ZZNEO_ATTRIB_JOINT_WEIGHTS, "vJointWeights");

    //link program
    glLinkProgram(m_shaderProgramObject);

    //check for linking errors

    GLint iProgramLinkStatus = 0;
    GLint iInfoLogLength = 0;
    GLchar* szInfoLog = NULL;

    glGetProgramiv(m_shaderProgramObject, GL_LINK_STATUS, &iProgramLinkStatus);

    if (iProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(m_shaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(iInfoLogLength);

            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(m_shaderProgramObject, iInfoLogLength, &written, szInfoLog);
                UTIL_SET_BREAKPOINT;
                free(szInfoLog);
                DEBUG_ASSERT(false, "shader linking failed");
                //uninitialize();
                exit(0);
            }
        }
    }

    //post link
    m_u_m_matrix      = glGetUniformLocation(m_shaderProgramObject, "u_m_matrix");
    m_u_v_matrix      = glGetUniformLocation(m_shaderProgramObject, "u_v_matrix");
    m_u_p_matrix      = glGetUniformLocation(m_shaderProgramObject, "u_p_matrix");
    m_u_gColorTexture = glGetUniformLocation(m_shaderProgramObject, "u_gColorTexture");
    m_u_jointTransforms_location = glGetUniformLocation(m_shaderProgramObject, "u_jointTransforms");

    DEBUG_OGL_INFO("AnimMesh::InitShader exit");
    return true;
}

//void AnimMesh::Render(long double ms, const std::string& currentAnimation)
void AnimMesh::Render(long double ms, const std::string& currentAnimation)
{
    DEBUG_OGL_INFO("AnimMesh::Render");
    ComputeSkeletonTransforms(ms, currentAnimation);

    glUseProgram(m_shaderProgramObject);

    //matrices
    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = ZzRenderer::Cam.GetViewMatrix();
    mat4 projectionMatrix = MyWin32::gProjectionMatrixForDepth;

    float sc = 50.0;
    modelMatrix = scale(sc, sc, sc);
    //modelMatrix = translate(0.0f, 0.0f, 100.0f);
    static float angle = 80.0f;
    //angle += 1;
    angle = (angle >= 360.0f)? 0 : angle;
    modelMatrix = rotate(angle, 0.0f, 1.0f, 0.0f) * modelMatrix;
    modelMatrix = translate(0.0f, 23.0f, 0.0f) * modelMatrix;

    glUniformMatrix4fv(m_u_m_matrix, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(m_u_v_matrix, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_u_p_matrix, 1, GL_FALSE, projectionMatrix);
    DEBUG_OGL_INFO("AnimMesh::Render sending bones matrices");
    glUniformMatrix4fv(m_u_jointTransforms_location, (GLsizei)m_boneNames.size(), GL_FALSE, m_u_jointTransforms_Matrices[0]);

    //bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_animModel->GetTexture(m_textureIndex));
    glUniform1i(m_u_gColorTexture, 0);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboFaceIndices);

    glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AnimMesh::ZzRender(void)
{
   // DEBUG_OGL_INFO("AnimMesh::Render");


  //  DEBUG_OGL_INFO("AnimMesh::Render sending bones matrices");
 //   glUniformMatrix4fv(m_u_jointTransforms_location, m_boneNames.size(), GL_FALSE, m_u_jointTransforms_Matrices[0]);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboFaceIndices);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/*build skeleton
    Logic -
    go through list of bones of the mesh, keep on attaching them to corresponding nodes.
    nodes have hierarchy information, bones (aiBones) do not.
    Once all bones are attached, parse nodes from top, creating one bone in skeleton for each node
    which has a bone attached.
    aiProcess_PopulateArmatureData seems to help in this, but I haven't been able to figure out how.
    */
void AnimMesh::ProcessBones(bool& retflag)
{
    retflag = true;

    if (m_vertexProcessingDone == false)
    {
        ProcessVertices(retflag);

        DEBUG_ASSERT(retflag == false, "ProcessVertices() failed")
    }

    m_animModel->ClearAllLinkedBonesFromNode();
    m_numBones = m_aiMeshRef->mNumBones;
    m_u_jointTransforms_Matrices.reset(new mat4[m_numBones]);

    memset(m_u_jointTransforms_Matrices.get(), 0, sizeof(mat4)*m_numBones);

    for (unsigned int i = 0; i < m_numBones; i++)
    {
        aiBone* bone = m_aiMeshRef->mBones[i];
        std::shared_ptr<AnimNode> node = m_animModel->GetNodeByName(bone->mName.C_Str());
        node->AttachBone(bone);
        m_boneNames.push_back(bone->mName.C_Str());

        for (unsigned int j = 0; j < bone->mNumWeights; j++)
        {
            aiVertexWeight vWeight = bone->mWeights[j];
            auto vertexId = vWeight.mVertexId;

            m_vJointIndicesWeights_shadow[vertexId].push_back(std::make_pair(vWeight.mWeight, i));

        }
    }

    //sort m_vJointIndicesWeights_shadow for each vertex on the basis of weight
    for (int i = 0; i < m_vJointIndicesWeights_shadow.size(); i++)
    {
        auto& vertexData = m_vJointIndicesWeights_shadow[i];
        std::sort(vertexData.begin(), vertexData.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b)
            {
                return b.first < a.first; //sort in descending order of weight
            });

        m_vJointWeights[i] = V4GLf(0.0);
        m_vJointIndices[i] = V4GLi(0);

        for (int j = 0; j < CONFIG_MAX_BONES_PER_VERTEX && j < vertexData.size(); j++)
        {
            m_vJointWeights[i][j] = vertexData[j].first;
            m_vJointIndices[i][j] = vertexData[j].second;
        }
    }


    for (auto& vertexWeights : m_vJointWeights)
    {
        GLfloat sum = vertexWeights[0] + vertexWeights[1] + vertexWeights[2] + vertexWeights[3];
        if (sum > 0)
        {
            vertexWeights = vertexWeights / sum;
        }
    }

    CopyNodeHierarchy(m_skeletonRoot, m_animModel->GetRootNode(), nullptr);
    retflag = false;
}


void AnimMesh::CopyNodeHierarchy(std::shared_ptr<AnimNode> bone, std::shared_ptr<AnimNode> animNode, std::shared_ptr<AnimNode> pParentBone)
{
    bone->GetNameRef() = animNode->GetNameCopy();
    bone->SetParent(pParentBone);
    bone->GetNodeTypeRef() = animNode->GetNodeTypeCopy();

    if (animNode->GetNodeTypeCopy() == AnimNodeType::BONE)
    {
        DEBUG_ASSERT(animNode->GetAttachedBone() != nullptr, "attahced bone not found in node type of bone")
        bone->GetOffsetMatrixRef() = aiMatrix4x4ToVmathMat4(animNode->GetAttachedBone()->mOffsetMatrix);
    }
    else
    {
        bone->GetOffsetMatrixRef() = mat4::identity();
    }

    bone->GetDefTransformRef() = animNode->GetDefTransformCopy();

    /*
    * no need to get transforms for corresponding bones.
    * we will computed them using animation info - http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
    * NodeTransformation = TranslationM * RotationM * ScalingM;
    * from the link
    * The node transformation is initialized from the mTransformation member in the node.
    * If the node does not correspond to a bone then that is its final transformation. If it does we overwrite it with a matrix that we generate.
    */
    //bone->GetLocalTransformRef() = animNode->GetLocalTransformCopy();
    //bone->GetCombinedTransformRef() = animNode->GetCombinedTransformCopy();
    bone->SetNodeType(AnimNodeType::BONE);

    //if (nullptr == pParentBone)//if this is root bone, we do not want transforms of corresponding node, since they may be in relation to other parent nodes
    //                            //which are not bones, our skeleton hierarchy starts at root bone
    //{
    //    bone->GetLocalTransformRef() = mat4::identity();
    //    bone->GetCombinedTransformRef() = mat4::identity();
    //}

    m_boneMap[bone->GetNameRef()] = bone;

    auto& animNodeChildren = animNode->GetChildrenRef();
    size_t numChildren = animNodeChildren.size();

    if (numChildren > 0)
    {
        bone->GetChildrenRef().reserve(numChildren);

        for (unsigned int i = 0; i < numChildren; i++)
        {
            std::shared_ptr<AnimNode> child(new AnimNode);
            bone->GetChildrenRef().push_back(child);
            CopyNodeHierarchy(bone->GetChildrenRef()[i], animNodeChildren[i], bone);
        }
    }
}

void AnimMesh::ComputeSkeletonTransforms(long double ms, const std::string& currentAnimation)
{
    ComputeSkeletonTransformsInternal(m_skeletonRoot, ms, currentAnimation);

    for (int i = 0; i < m_boneNames.size(); i++)
    {
        std::shared_ptr<AnimNode> bone = GetBoneByName(m_boneNames[i]);
        m_u_jointTransforms_Matrices[i] = m_globalInverserTransform * bone->GetCombinedTransformRef() * bone->GetOffsetMatrixRef();
    }
}

std::shared_ptr<AnimNode> AnimMesh::GetBoneByName(const std::string & nodeName)
{
    auto itr = m_boneMap.find(nodeName);
    if (m_boneMap.end() == itr)
    {
        THROW_DEBUG_EXCEPTION("bone not found")
            return nullptr;
    }
    return itr->second;
}

void AnimMesh::ComputeSkeletonTransformsInternal(std::shared_ptr<AnimNode> bone, long double ms, const std::string& currentAnimation)
{
    if (!bone)
    {
        return;
    }

    //sahils-debug remove this
    /*DEBUG_VSCONSOLE("bone: " + bone->GetNameCopy());
    for (auto& elem : bone->m_animMap)
    {
        DEBUG_VSCONSOLE("\tAnimation: " + elem.first);
        for (auto& key : elem.second->positionKeys)
        {
            DEBUG_VSCONSOLE("\t\tPositionKey: " + std::to_string(key.mTime) + " " + std::to_string(key.mValue.x) + " " + std::to_string(key.mValue.y) + " " + std::to_string(key.mValue.z));
        }

        for (auto& key : elem.second->rotationKeys)
        {
            DEBUG_VSCONSOLE("\t\tRotationKey: "  + std::to_string(key.mTime) + " " + std::to_string(key.mValue.x) + " " + std::to_string(key.mValue.y) + " " + std::to_string(key.mValue.z) + " " + std::to_string(key.mValue.w));
        }

        for (auto& key : elem.second->scalingKeys)
        {
            DEBUG_VSCONSOLE("\t\tScalingKey: " + std::to_string(key.mTime) + " " + std::to_string(key.mValue.x) + " " + std::to_string(key.mValue.y) + " " + std::to_string(key.mValue.z));
        }
    }
*/
    bone->CalculateLocalTransform(ms, currentAnimation);

    if (bone->GetParent())//non root
    {
        bone->GetCombinedTransformRef() = bone->GetParent()->GetCombinedTransformRef() * bone->GetLocalTransformRef();
    }
    else//sahils-debug remove this
    {
        mat4 armature = mat4(
            vec4(0.480000f, 0.000000f, 0.000000f, 0.000000f),
            vec4(0.000000f, 0.480000f, 0.000000f, 0.000000f),
            vec4(0.000000f, 0.000000f, 0.480000f, 0.000000f),
            vec4(0.000000f, 0.000000f, 0.000000f, 1.000000f)
        );

        mat4 scene = mat4(
            vec4(0.010000f, 0.000000f, 0.000000f, 0.000000f),
            vec4(0.000000f, 0.000000f, 0.010000f, 0.000000f),
            vec4(0.000000f, -0.010000f, 0.000000f, 0.000000f),
            vec4(0.000000f, 0.000000f, 0.000000f, 1.000000f)
        );

        //bone->GetCombinedTransformRef() = scene.transpose() * armature.transpose() * bone->GetLocalTransformRef();
        //bone->GetCombinedTransformRef() = scene * armature * bone->GetLocalTransformRef() * bone->GetOffsetMatrixRef();
        //bone->GetCombinedTransformRef() = bone->GetLocalTransformRef() * bone->GetOffsetMatrixRef();
        bone->GetCombinedTransformRef() = bone->GetLocalTransformRef();
    }

    for (auto& child : bone->GetChildrenRef())
    {
        ComputeSkeletonTransformsInternal(child, ms, currentAnimation); //sahils-test check
    }
}

bool AnimMesh::ProcessAnimation(aiAnimation** aiAnim, unsigned int numAnimations)
{
    m_globalInverserTransform = aiMatrix4x4ToVmathMat4(m_animModel->GetAiScene()->mRootNode->mTransformation);
    m_globalInverserTransform = Util::inverse(m_globalInverserTransform);

    for (auto& elem : m_boneMap)
    {
        elem.second->ProcessAnimation(aiAnim, numAnimations);
    }

    return true;
}

void AnimMesh::Attach(AnimModel* animModel)
{
    m_animModel = animModel;
}