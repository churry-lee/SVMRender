#ifndef SVMVIEW_HPP_
#define SVMVIEW_HPP_

/*******************************************************************************************
 * Includes
 *******************************************************************************************/
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

// String
#include <string>

/*//OpenGL
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

//Capturing
#include "src_v4l2.hpp"
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
//#include <g2d.h>

//OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>*/

//Shaders
#include "gl_shaders.hpp"
#include "shaders.hpp"

/**********************************************************************************************************************
 * Macros
 **********************************************************************************************************************/
#ifdef IMX8QM	
#define GL_PIXEL_TYPE GL_RGBA
//#define CAM_PIXEL_TYPE V4L2_PIX_FMT_RGB32
#else	
#define GL_PIXEL_TYPE GL_VIV_UYVY
//#define CAM_PIXEL_TYPE V4L2_PIX_FMT_UYVY
#endif

/**********************************************************************************************************************
 * Types
 **********************************************************************************************************************/
struct vertices_obj			
{
	GLuint	vao;
	GLuint	vbo;
	GLuint	tex;
	int		num;
};

/*******************************************************************************************
 * Classes
 *******************************************************************************************/
/* Render class */
class View {
public:
	View();
	~View();

	int addProgram(const char* v_shader, const char* f_shader);
	int setProgram(uint index);
	void cleanView();
	
	int addCamera(int index, int width, int height);
	int addMesh(string filename);	
	
	void renderView(int camera, int mesh);
	int runCamera(int index);
	
	int createMesh(Mat xmap, Mat ymap, string filename, int density, Point2f top);
	void reloadMesh(int index, string filename);
	int changeMesh(Mat xmap, Mat ymap, int density, Point2f top, int index);
	
	Mat takeFrame(int index);
	
	
	int getVerticesNum(uint num) {if (num < v_obj.size()) return (v_obj[num].num); return (-1);}
	
	
	int addBuffer(GLfloat* buf, int num);
	int setBufferAsAttr(int buf_num, int prog_num, char* atr_name);
	void renderBuffer(int buf_num, int type, int vert_num);
	void updateBuffer(int buf_num, GLfloat* buf, int num);
	//int getProgId(uint index) {if (index < render_prog.size()) return render_prog[index].getHandle();	return (-1);}
	
	
	
private:
	int current_prog;	
	vector<Programs> render_prog;
	vector<vertices_obj> v_obj;
	vector<v4l2Camera> v4l2_cameras;	// Camera buffers
	
	void vLoad(GLfloat** vert, int* num, string filename);
	void bufferObjectInit(GLuint* text_vao, GLuint* text_vbo, GLfloat* vert, int num);
	void texture2dInit(GLuint* texture);	
};

#endif /* SRC_RENDER_HPP_ */
