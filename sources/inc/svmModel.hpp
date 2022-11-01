#ifndef _SVMMODEL_HPP_
#define _SVMMODEL_HPP_

#include "svmCore.hpp"
#include "svmMesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


class sanCar
{
public:
    sanCar(const std::string &filename);
    ~sanCar();

    GLboolean Init();
    int GetMeshCount() const { return (int)m_meshes.size(); }

    void Draw(const Program * program) const;

private:
	std::string m_fileName;

    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Materials> m_textures;
    std::vector<Mesh> m_meshes;

    void processNode(aiNode * node, const aiScene * scene);
    void processMesh(aiMesh * mesh, const aiScene * scene);
};


#endif //_SVMMODEL_HPP_
