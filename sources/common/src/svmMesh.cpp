#include "svmMesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, GLuint primitiveType)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
    this->m_primitiveType = primitiveType;
}

// setupMesh
void Mesh::Init()
{
    // VAO Generation & Binding
    vertexLayoutPtr->Init();
    // VBO Generation & Binding
    vertexBufferPtr->Init(GL_ARRAY_BUFFER, GL_STATIC_DRAW, m_vertices.data(), sizeof(Vertex), m_vertices.size());
    vertexLayoutPtr->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    if (!m_indices.empty())
    {
        // EBO(IBO) Generation & Binding
        indexBufferPtr->Init(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, &m_indices[0], sizeof(GLuint), m_indices.size());
        // VAO <- VBO setting Attribute
        vertexLayoutPtr->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
    }
	else
		vertexLayoutPtr->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
    vertexLayoutPtr->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
}



void Mesh::Draw(const Program * program) const
{
    vertexLayoutPtr->Bind();

    glDrawArrays(m_primitiveType, 0, vertexBufferPtr->GetCount());
    glBindVertexArray(0);
}

void Mesh::Draw(const Program * program, Materials texture) const
{
    program->SetUniform("material.ambient", texture.ambient);
    program->SetUniform("material.diffuse", texture.diffuse);
    program->SetUniform("material.specular", texture.specular);
    program->SetUniform("material.shininess", texture.shininess);

    vertexLayoutPtr->Bind();
    glDrawElements(m_primitiveType, indexBufferPtr->GetCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{}