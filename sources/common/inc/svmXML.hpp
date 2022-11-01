#ifndef _SVMXML_HPP_
#define _SVMXML_HPP_

#include "svmCore.hpp"
#include "libxml/parser.h"
#include "libxml/xmlmemory.h"
//#include "libxml/xmlreader.h"
//#include <boost/functional/hash.hpp>
//#include <boost/lexical_cast.hpp>

#include "libxml/xmlwriter.h"
#include "ctime"

struct RCAM_PARAMETERS {
	float fmu, fmv;
	float sf;
	float cx, cy;
	float k1, k2, p1, p2, k3;
	float d0, d1, d2, d3, d4, d5, d6, d7, d8;
	float x, y, z, tilt, pan, roll;
};

struct XY {
	union {
		float x;
		float width;
	};
	union {
		float y;
		float height;
	};
};

struct XYZ {
	float x;
	float y;
	float z;
};

struct VIEWPORT {
	int x;
	int y;
	int width;
	int height;
};

struct POSE {
	glm::vec3 pos;
	glm::vec3 dir;
};


struct LAYOUT {
	int x;
	int y;
	int width;
	int height;
	int view_mode;
	bool animation;
};

struct RESOLUTION
{
	XY image;
	XY display;
};

struct MESH_BOWL {
	int sector_nums;
	int start_sector_index;
	int nop_z;
	float step_x;
	float radius;
};

struct MODEL {
	std::string model_name;
	float reduction_ratio;
	XYZ translation;
	XYZ rotation;
	XYZ scale;
};

struct VEHICLE {
	std::string customer;
	std::string vehicle_type_name;
	float length;
	float width;
	float height;
	float reserved0;
	float reserved1;
	float reserved2;
	float reserved3;
	float reserved4;
};

struct SVM_PATTERN
{
	XY unit;
	XY tplate[2];
};

class sanXML
{
public:
	std::string m_fileDir;
	sanXML();
public:
	RESOLUTION m_resolution;              // resolution
	RCAM_PARAMETERS m_rcam[SVM_CAM_NUM];  // real_camera
	int m_points_num[2];                  // control_points_num
	XY m_feature_pts[SVM_CAM_NUM][16];    // feature_points
	XYZ m_local_pts[SVM_CAM_NUM][16];     // rea_local_points
	LAYOUT m_layout[2];                   // layout_view
	MESH_BOWL m_mesh;                     // mesh3D
	POSE m_vcam[11]; 	                  // virtual_camera(front:0, right:1, rear:2, left:3, top:10)
	SVM_PATTERN m_pattern;                    // pattern
	MODEL  m_model;                       // model_car
	VEHICLE m_vehicle;                    // vehicle

public:
	bool loadXML();

	bool setParam(int num, const char * val);
	int getParam(const char * name);

	bool read_xy(const char * src, XY * dst);
	bool read_xyz(const char * src, XYZ * dst);
	bool read_rcam_parameters(const char * src, RCAM_PARAMETERS * dst);
	bool read_local_points(const char * src, XYZ *dst, const int iter);
	bool read_feature_points(const char* src, XY* dst, const int iter);

	bool read_layout(const char * src, LAYOUT * dst);
	bool read_vcam_pose(const char* src, POSE* dst);
	bool read_mesh3D(const char * src, MESH_BOWL * dst);

	bool read_vehicle(const char * src, VEHICLE * dst);

	bool read_bool(const char * src, bool * dst);
	bool read_int(const char * src, int * dst);
	bool read_uint(const char * src, unsigned int * dst);
	bool read_float(const char * src, float * dst);

	bool writeXML(const std::vector<glm::vec2> & val, const char * node);
	bool writeXML2(const std::vector<glm::vec2> & val);
};
#endif
