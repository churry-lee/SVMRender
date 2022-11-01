#ifndef _SVMBOWLMESH_HPP_
#define _SVMBOWLMESH_HPP_

#include "svmCore.hpp"
#include "svmProgram.hpp"
#include "svmMesh.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>


class BowlMesh
{
public:
    BowlMesh();
    ~BowlMesh();
    GLboolean Init(int camID, std::string path);

    void Draw(const Program * program);

    Vertex vertex;
    std::vector<Vertex> m_vertices;
private:
    std::vector<GLuint> m_indices;
    std::vector<Mesh> m_meshes;

    void ReadBowlLUT(const std::string & filename);
    void ArrayToVector(std::string & line);
};


#endif //_SVMBOWLMESH_HPP_
