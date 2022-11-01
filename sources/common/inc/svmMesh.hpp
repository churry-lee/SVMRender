#ifndef _SVMMESH_HPP_
#define _SVMMESH_HPP_

#include "svmCore.hpp"
#include "svmBuffer.hpp"
#include "svmVertexLayout.hpp"
#include "svmProgram.hpp"

struct Vertex
{
    glm::vec3 position;      // 12 bytes
    glm::vec3 normal;        // 12 bytes
    glm::vec2 texCoord;      // 8  bytes
};

struct Materials
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, GLuint primitiveType);
    ~Mesh();

    void Init();
    void Draw(const Program * program) const;
    void Draw(const Program * program, Materials texture) const;
private:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Materials> m_textures;
    GLuint m_primitiveType;

    std::shared_ptr<VertexLayout> vertexLayoutPtr = std::make_shared<VertexLayout>();   // VAO
    std::shared_ptr<Buffer> vertexBufferPtr = std::make_shared<Buffer>();               // VBO
    std::shared_ptr<Buffer> indexBufferPtr = std::make_shared<Buffer>();                // IBO
};


#endif //_SVMMESH_HPP_
