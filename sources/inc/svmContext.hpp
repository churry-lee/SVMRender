#ifndef _SVMCONTEXT_HPP_
#define _SVMCONTEXT_HPP_

#include "svmCore.hpp"
#include "svmProgram.hpp"
#include "svmModel.hpp"
#include "svmVCam.hpp"
#include "svmBowlMesh.hpp"
#include "svmTex.hpp"
#include "svmXML.hpp"

struct Light
{
    glm::vec3 position { glm::vec3(0.0f, 2.0f, 0.0f) };
    glm::vec3 direction { glm::vec3(0.0f, -1.0f, 0.0f) };
    glm::vec2 cutoff { glm::vec2(20.0f, 5.0f) };
    GLfloat distance { 50.0f };
    glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
    glm::vec3 diffuse { glm::vec3(0.8f, 0.8f, 0.8f) };
    glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
};

class Context
{
public:
    Context();
    ~Context();
	void Init();
    void Render();
    void ProcessInput(GLFWwindow* window);

    void Reshape(GLint width, GLint height);
    int m_width {WINDOW_WIDTH};
    int m_height {WINDOW_HEIGHT};

	int m_animation_idx;
private:
    Light m_light;

	void RenderBowlOverlap(unsigned int camID, glm::mat4 mvp);
	void RenderBowlNonOverlap(unsigned int camID, glm::mat4 mvp);
	void RenderBowl(unsigned int camID, glm::mat4 mvp);
	void RenderCarModel(glm::vec3 vCamPos, glm::mat4 mvp);
    void RenderLayout0(int camID);
    void RenderLayout13D(int camID);
    void RenderLayout12D();
	void UpdateTexture(int camID);

	glm::vec3 topview_vCamPos, topview_vCamDir;
////////////////////////////////////////////////////////////////
	std::unique_ptr<sanXML> m_pxml;
////////////////////////////////////////////////////////////////
	std::vector<BowlMesh *> m_pobowl;
	std::unique_ptr<Program> m_ppobowl;
	bool LoadBowlOverlap();

	std::vector<BowlMesh *> m_pnobowl;
	std::unique_ptr<Program> m_ppnobowl;
	bool LoadBowlNonOverlap();
////////////////////////////////////////////////////////////////
	std::unique_ptr<sanCar> m_pcar;
	std::unique_ptr<Program> m_ppcar;
	glm::mat4 car_transform;
	bool loadCarModel();
////////////////////////////////////////////////////////////////
	std::unique_ptr<vCam> vCamPtr;
	bool FLAG_3DVIEW;
	VIEWMODE view_mode;

	glm::vec3 prev_vCamPos;
	glm::vec3 prev_vCamDir;
	glm::vec3 target_vCamPos;
	glm::vec3 target_vCamDir;

	glm::mat4 Animate();
	std::vector<glm::mat4> CreateAnimationList(VIEWMODE view_mode);
	std::vector<glm::mat4> m_animation_list;
	std::vector<glm::vec3> vCamViewMode(VIEWMODE mode);
////////////////////////////////////////////////////////////////
	std::unique_ptr<Texture> TexturePtr;
    unsigned int InputTextureID[4]{ 0, 0, 0, 0 };
	unsigned int MaskTextureID[4] { 0, 0, 0, 0 };
	cv::Mat img, mask;
	bool LoadBowlTexture();
	bool LoadOverlapMask();
////////////////////////////////////////////////////////////////
	Mesh * view2D_meshes;
	Program * view2D_ProgramPtr;
    std::vector<Vertex> view2D_vertices;
    std::vector<GLuint> view2D_indices;
    bool LoadCamView2D();
};

#endif //_SVMCONTEXT_HPP_
