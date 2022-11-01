#include "svmContext.hpp"

Context::Context()
{
	/* member variable initialize */
	FLAG_3DVIEW = true;
	view_mode = CAMVIEW_FRONT;
	prev_vCamPos   = glm::vec3{ 0.0f, 0.0f, 0.0f };
	prev_vCamDir   = glm::vec3{ 0.0f, 0.0f, 0.0f };
	target_vCamPos = glm::vec3{ 0.0f, 0.0f, 0.0f };
	target_vCamDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_animation_list.clear();
	m_animation_idx = 0;
}

bool Context::LoadBowlTexture()
{
	for (unsigned int camera = 0; camera < SVM_CAM_NUM; camera++)
	{
		TexturePtr->Init(GL_TEXTURE0, InputTextureID[camera]);
		printf("Input Texture ID: %d\n", InputTextureID[camera]);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return true;
}

bool Context::LoadOverlapMask()
{
	cv::Mat mask;
	for (unsigned int camera = 0; camera < SVM_CAM_NUM; camera++)
	{
		TexturePtr->Init(GL_TEXTURE1, MaskTextureID[camera]);
		printf("Mask Texture ID: %d\n", MaskTextureID[camera]);
		mask = cv::imread((std::string)PROJECT_SRC_DIR + "/masks/mask" + std::to_string(camera) + ".jpg");
		if (mask.empty())
			throw logging("Mask is Empty");

		glBindTexture(GL_TEXTURE_2D, MaskTextureID[camera]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mask.cols, mask.rows, 0, GL_RED, GL_UNSIGNED_BYTE, mask.ptr());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return true;
}

bool Context::LoadBowlOverlap()
{
	for (int camera = 0; camera < SVM_CAM_NUM; camera++)
    {
		auto * temp_grid = new BowlMesh();
        if (!temp_grid->Init(camera, "overlap"))
			return false;
		m_pobowl.push_back(temp_grid);
    }
    if (!m_ppobowl->Init())
        return false;
    printf("BowlMesh Program id: %d\n", m_ppobowl->Get());

    return true;
}

bool Context::LoadBowlNonOverlap()
{
	for (int camera = 0; camera < SVM_CAM_NUM; camera++)
	{
		auto * temp_grid = new BowlMesh();
		if (!temp_grid->Init(camera, "non_overlap"))
			return false;
		m_pnobowl.push_back(temp_grid);
	}
	if (!m_ppnobowl->Init())
		return false;
	printf("BowlMesh Program id: %d\n", m_ppnobowl->Get());

	return true;
}

bool Context::loadCarModel()
{
    if (!m_pcar->Init())
		return false;
    if (!m_ppcar->Init())
		return false;
    printf("sanCar Program id: %d\n", m_ppcar->Get());

	car_transform = glm::mat4(1.0f);
	glm::mat4 car_translation = glm::translate(car_transform, glm::vec3(m_pxml->m_model.translation.x, m_pxml->m_model.translation.y, m_pxml->m_model.translation.z));
	glm::mat4 car_rotation_y = glm::rotate(car_translation, glm::radians(m_pxml->m_model.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 car_rotation_z = glm::rotate(car_rotation_y, glm::radians(m_pxml->m_model.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 car_rotation_x = glm::rotate(car_rotation_z, glm::radians(m_pxml->m_model.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 car_scale = glm::scale(car_rotation_x, glm::vec3(m_pxml->m_model.scale.x, m_pxml->m_model.scale.y, m_pxml->m_model.scale.z));
	car_transform = car_scale;

    return true;
}

bool Context::LoadCamView2D()
{
    view2D_vertices.clear();
    view2D_indices.clear();

	view2D_vertices = {
		Vertex{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},

		Vertex{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{ glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
	};
	view2D_meshes = new Mesh(view2D_vertices, view2D_indices, GL_TRIANGLES);

    if (!view2D_ProgramPtr->Init())
		throw logging("Failed to link 2D view shader program");
    printf("2D Camara View Program id: %d\n", view2D_ProgramPtr->Get());

    return true;
}

void Context::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////////////////////
	m_pxml = std::make_unique<sanXML>();
	if (!m_pxml->loadXML())
		throw logging("Failed to load XML file");
	prev_vCamPos = m_pxml->m_vcam[0].pos;
	prev_vCamDir = m_pxml->m_vcam[0].dir;
	//////////////////////////////////////////////////////////////////////////////////////
	m_ppobowl = std::make_unique<Program>((std::string)PROJECT_SRC_DIR+"/shader/ObowlMesh.vert", (std::string)PROJECT_SRC_DIR+"/shader/ObowlMesh.frag");
	m_ppnobowl = std::make_unique<Program>((std::string)PROJECT_SRC_DIR+"/shader/NObowlMesh.vert", (std::string)PROJECT_SRC_DIR+"/shader/NObowlMesh.frag");
	//////////////////////////////////////////////////////////////////////////////////////
	m_pcar = std::make_unique<sanCar>((std::string)PROJECT_SRC_DIR + "/models/ferrari.dae");
	m_ppcar = std::make_unique<Program>((std::string)PROJECT_SRC_DIR+"/shader/modelCar.vert", (std::string)PROJECT_SRC_DIR+"/shader/modelCar.frag");
	//////////////////////////////////////////////////////////////////////////////////////
	view2D_ProgramPtr = new Program((std::string)PROJECT_SRC_DIR + "/shader/camView2D.vert", (std::string)PROJECT_SRC_DIR + "/shader/camView2D.frag");
	vCamPtr = std::make_unique<vCam>();
	TexturePtr = std::make_unique<Texture>();
    //////////////////////////////////////////////////////////
	if (!LoadBowlOverlap())
		throw logging("Failed to load Bowl Mesh!");
	if (!LoadBowlNonOverlap())
		throw logging("Failed to load Bowl Mesh!");
	if (!loadCarModel())
		throw logging("Failed to load 3D Car sanCar!");
	if (!LoadCamView2D())
		throw logging("Failed to load Second Texture!");
	if (!LoadBowlTexture())
		throw logging("Failed to create Input source Texture!");
	if (!LoadOverlapMask())
		throw logging("Failed to create Mask Texture!");
}

void Context::UpdateTexture(int camID)
{
	cv::Mat src = cv::imread((std::string)PROJECT_SRC_DIR + "/images/src" + std::to_string(camID) + ".jpg");
	if (src.empty())
		throw logging("Input Source is not opened");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, InputTextureID[camID]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src.cols, src.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, src.ptr());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Context::Render()
{
	for (unsigned int camID = 0; camID < SVM_CAM_NUM; camID++)
	{
		UpdateTexture(camID);

		glViewport(m_pxml->m_layout[0].x, m_pxml->m_layout[0].y, m_pxml->m_layout[0].width, m_pxml->m_layout[0].height);
		RenderLayout0(camID);

		glViewport(m_pxml->m_layout[1].x, m_pxml->m_layout[1].y, m_pxml->m_layout[1].width, m_pxml->m_layout[1].height);
		if (FLAG_3DVIEW)
			RenderLayout13D(camID);
		else
			RenderLayout12D();
	}
}

void Context::RenderBowlOverlap(unsigned int camID, glm::mat4 mvp)
{
	m_ppobowl->Use();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, MaskTextureID[camID]);
	m_ppobowl->SetUniform("mask", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, InputTextureID[camID]);
	m_ppobowl->SetUniform("tex", 0);

	m_ppobowl->SetUniform("mvp", mvp);
	m_pobowl[camID]->Draw(m_ppobowl.get());

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Context::RenderBowlNonOverlap(unsigned int camID, glm::mat4 mvp)
{
	m_ppnobowl->Use();
	m_ppnobowl->SetUniform("mvp", mvp);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, InputTextureID[camID]);
	m_ppobowl->SetUniform("tex", 0);
	m_pnobowl[camID]->Draw(m_ppnobowl.get());

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Context::RenderBowl(unsigned int camID, glm::mat4 mvp)
{
	glEnable(GL_DEPTH_TEST);
	RenderBowlOverlap(camID, mvp);
	RenderBowlNonOverlap(camID, mvp);
	glDisable(GL_DEPTH_TEST);
}

void Context::RenderCarModel(glm::vec3 vCamPos, glm::mat4 mvp)
{
	glEnable(GL_DEPTH_TEST);
	m_ppcar->Use();
	m_ppcar->SetUniform("light.position", m_light.position);
	m_ppcar->SetUniform("light.direction", m_light.direction);
	m_ppcar->SetUniform("light.cutoff", glm::vec2(cosf(glm::radians(m_light.cutoff[0])), cosf(glm::radians(m_light.cutoff[0] + m_light.cutoff[1]))));
	m_ppcar->SetUniform("light.attenuation", GetAttenuationCoeff(m_light.distance));
	m_ppcar->SetUniform("light.ambient", m_light.ambient);
	m_ppcar->SetUniform("light.diffuse", m_light.diffuse);
	m_ppcar->SetUniform("light.specular", m_light.specular);

	m_ppcar->SetUniform("viewPos", vCamPos);
	m_ppcar->SetUniform("model", car_transform);
	m_ppcar->SetUniform("mvp", mvp);
	m_pcar->Draw(m_ppcar.get());
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
}

void Context::RenderLayout0(int camID)
{
    glm::mat4 ortho = glm::ortho(-2.8f, 2.8f, -2.8f, 2.8f, 0.1f, 100.0f);
    glm::vec3 vCamPos = vCamViewMode(static_cast<VIEWMODE>(m_pxml->m_layout[0].view_mode))[0];
    glm::vec3 vCamDir = vCamViewMode(static_cast<VIEWMODE>(m_pxml->m_layout[0].view_mode))[1];
    glm::mat4 view1 = vCamPtr->sanvCamPose(vCamPos, vCamDir);

    glm::mat4 car_mvp = ortho * view1 * car_transform;
	RenderCarModel(vCamPos, car_mvp);

	glm::mat4 bowl_mvp = ortho * view1;
	RenderBowl(camID, bowl_mvp);
}

void Context::RenderLayout13D(int camID)
{
	glm::mat4 pers = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 view{1.0f};

	if (!m_animation_list.empty())
		view = Animate();
	if (camID == 0)
		m_animation_idx++;

	glm::mat4 car_mvp = pers * view * car_transform;
	RenderCarModel(prev_vCamPos, car_mvp);

	glm::mat4 bowl_mvp = pers * view;
	RenderBowl(camID, bowl_mvp);
}

void Context::RenderLayout12D()
{
    GLint camera = view_mode - 1;

    view2D_ProgramPtr->Use();
    view2D_meshes->Draw(view2D_ProgramPtr);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

std::vector<glm::vec3> Context::vCamViewMode(VIEWMODE mode)
{
	glm::vec3 Position;
	glm::vec3 Direction;

	std::vector<glm::vec3> vCamPose;

	switch (mode)
	{
	case TOPVIEW3D:  // Top
		Position = m_pxml->m_vcam[10].pos;
		Direction = m_pxml->m_vcam[10].dir;
		break;
	case CAMVIEW_FRONT:
		Position = m_pxml->m_vcam[0].pos;
		Direction = m_pxml->m_vcam[0].dir;
		break;
	case CAMVIEW_RIGHT:
		Position = m_pxml->m_vcam[1].pos;
		Direction = m_pxml->m_vcam[1].dir;
		break;
	case CAMVIEW_REAR:
		Position = m_pxml->m_vcam[2].pos;
		Direction = m_pxml->m_vcam[2].dir;
		break;
	case CAMVIEW_LEFT:
		Position = m_pxml->m_vcam[3].pos;
		Direction = m_pxml->m_vcam[3].dir;
		break;
	}
	vCamPose.push_back(Position);
	vCamPose.push_back(Direction);

	return vCamPose;
}

std::vector<glm::mat4> Context::CreateAnimationList(VIEWMODE view_mode)
{
	m_animation_list.clear();

	target_vCamPos = vCamViewMode(view_mode)[0];
	target_vCamDir = vCamViewMode(view_mode)[1];

	m_animation_list = vCamPtr->sanAnimation4(prev_vCamPos, prev_vCamDir, target_vCamPos, target_vCamDir, ANIMATION_STEPS);

	return m_animation_list;
}

glm::mat4 Context::Animate()
{
	glm::mat4 view{1.0f};

/*
	if (m_animation_list.empty())
	{
		target_vCamPos = vCamViewMode(view_mode)[0];
		target_vCamDir = vCamViewMode(view_mode)[1];

		view = vCamPtr->sanvCamPose(prev_vCamPos, prev_vCamDir);
		m_animation_list = vCamPtr->sanAnimation4(prev_vCamPos, prev_vCamDir, target_vCamPos, target_vCamDir, ANIMATION_STEPS);
	}
	else
	{
		if (m_animation_idx != 0 && m_animation_idx == m_animation_list.size()) // animation stop condition
		{
			prev_vCamPos = target_vCamPos;
			prev_vCamDir = target_vCamDir;

			view = vCamPtr->sanvCamPose(prev_vCamPos, prev_vCamDir);

			m_animation_list.clear();
			m_animation_idx = 0;
		}
		else
		{
			if (camID == 0) m_animation_idx++;
			view = m_animation_list[m_animation_idx];
		}
	}
//	else
//		view = m_animation_list[m_animation_idx];
*/
	if (m_animation_idx != 0 && m_animation_idx == m_animation_list.size()) // animation stop condition
	{
		prev_vCamPos = target_vCamPos;
		prev_vCamDir = target_vCamDir;

		view = vCamPtr->sanvCamPose(prev_vCamPos, prev_vCamDir);

		m_animation_list.clear();
		m_animation_idx = 0;
	}
	else
	{
		view = m_animation_list[m_animation_idx];
	}
	return view;
}


void Context::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		view_mode = CAMVIEW_FRONT;
	}
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		view_mode = CAMVIEW_REAR;
	}
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		view_mode = CAMVIEW_RIGHT;
	}
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		view_mode = CAMVIEW_LEFT;
	}
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (FLAG_3DVIEW) FLAG_3DVIEW = false;
        else FLAG_3DVIEW = true;
    }
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		throw logging("catch test");

	CreateAnimationList(view_mode);
}

Context::~Context()
{
//	m_pxml.reset();

//	vCamPtr.reset();
//
//	delete m_pcar;
//	delete m_ppcar;
//	delete view2D_meshes;
//	delete view2D_ProgramPtr;

	printf("End Program\n");
}