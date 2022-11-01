#include "svmBowlMesh.hpp"

BowlMesh::BowlMesh()
{
	m_vertices.clear();
	m_indices.clear();
	m_meshes.clear();
}

GLboolean BowlMesh::Init(int camID, std::string path)
{
    using std::filesystem::directory_iterator;
    std::string array_path = (std::string)PROJECT_SRC_DIR + "/arrays/" + path + "/array";
    std::string array_file = array_path + std::to_string(camID);

    ReadBowlLUT(array_file);

#ifdef CAMERA
    Mesh glMesh = Mesh(m_vertices, m_indices, GL_TRIANGLES);
	glMesh.Init();
#else
    Mesh glMesh = Mesh(m_vertices, m_indices, GL_POINTS);
	glMesh.Init();
#endif
    m_meshes.push_back(glMesh);

    return true;
}

void BowlMesh::ReadBowlLUT(const std::string& filename)
{
	std::ifstream input_file(filename);
    if (!input_file.is_open())
		throw logging("Failed to open array file: " + filename);

    std::string lineBuffer;
    while (std::getline(input_file, lineBuffer))
        ArrayToVector(lineBuffer);

    input_file.close();
}

void BowlMesh::ArrayToVector(std::string & line)
{
    std::istringstream iss(line);
    std::vector<std::string> tmp;
    std::string buffer;

    while (std::getline(iss, buffer, ' '))
        tmp.push_back(buffer);

    vertex.position = glm::vec3(std::stof(tmp[0]), std::stof(tmp[1]), std::stof(tmp[2]));
    vertex.texCoord = glm::vec2(std::stof(tmp[3]), std::stof(tmp[4]));
    m_vertices.push_back(vertex);
}

void BowlMesh::Draw(const Program * program)
{
    for(GLuint i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Draw(program);
}

BowlMesh::~BowlMesh()
{
}