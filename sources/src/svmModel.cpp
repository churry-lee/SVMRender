#include "svmModel.hpp"

sanCar::sanCar(const std::string &filename)
{
	this->m_fileName = filename;
}

GLboolean sanCar::Init()
{
    Assimp::Importer importer;
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

    const aiScene * scene = importer.ReadFile(m_fileName,
                                              aiProcess_JoinIdenticalVertices |
                                              aiProcess_LimitBoneWeights |
                                              aiProcess_PreTransformVertices |
                                              aiProcess_Triangulate |
                                              aiProcess_GenUVCoords |
                                              aiProcess_SortByPType |
                                              aiProcess_FindDegenerates |
                                              aiProcess_FindInvalidData |
                                              aiProcess_GenNormals
                                              );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		throw logging("failed to load model: " + m_fileName);

    processNode(scene->mRootNode, scene);

    return true;
}

void sanCar::processMesh(aiMesh *mesh, const aiScene *scene)
{
	printf("process mesh: %s, #vert: %d, #face: %d\n", mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

    m_vertices.resize(mesh->mNumVertices);
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex & vertex = m_vertices[i];
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0])
            vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
    }

    m_indices.resize(mesh->mNumFaces * 3);
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        m_indices[3*i  ] = mesh->mFaces[i].mIndices[0];
        m_indices[3*i+1] = mesh->mFaces[i].mIndices[1];
        m_indices[3*i+2] = mesh->mFaces[i].mIndices[2];
    }

    m_textures.resize(scene->mNumMaterials);
    for (GLuint i = 0; i < scene->mNumMaterials; i++)
    {
        Materials & texture = m_textures[i];
        aiMaterial * material = scene->mMaterials[i];

        aiColor3D ambient, diffuse, specular;
        float shininess = 0.0f;

        if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_AMBIENT, ambient))
			throw logging("Cannot load color ambient property");
        if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
			throw logging("Cannot load color diffuse property");
        if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_SPECULAR, specular))
			throw logging("Cannot load color specular property");
        if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininess))
			throw logging("Cannot load color shininess property");

        texture.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
        texture.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
        texture.specular = glm::vec3(specular.r, specular.g, specular.b);
        texture.shininess = glm::max(shininess, 255.0f);
    }

    Mesh glMesh = Mesh(m_vertices, m_indices, GL_TRIANGLES);
	glMesh.Init();
    m_meshes.push_back(glMesh);
}

void sanCar::processNode(aiNode *node, const aiScene *scene)
{

    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

void sanCar::Draw(const Program * program) const
{

    for(GLuint i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Draw(program, m_textures[i]);
}


sanCar::~sanCar()
{}

