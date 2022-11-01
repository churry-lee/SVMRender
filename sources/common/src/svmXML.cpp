#include "svmXML.hpp"
#include <errno.h>

sanXML::sanXML()
{
	this->m_fileDir = std::string((std::string)PROJECT_SRC_DIR + "/settings/settings.xml");
}

bool sanXML::loadXML()
{
	const char * filepath = (const char *)&m_fileDir.c_str()[0];

	xmlDocPtr doc_ptr = xmlReadFile(filepath, NULL, 0);
	if (doc_ptr == NULL)
		return false;

	xmlNodePtr node_ptr = xmlDocGetRootElement(doc_ptr); //LEVEL1
	if (node_ptr == NULL)
	{
		xmlFreeDoc(doc_ptr);
		return false;
	}

	for (node_ptr = node_ptr->children; node_ptr != NULL; node_ptr = node_ptr->next) //LEVEL2
	{
		if (node_ptr->type == XML_ELEMENT_NODE)
		{
			xmlNodePtr pchildren  = node_ptr->xmlChildrenNode;
			for (pchildren = node_ptr->children; pchildren != NULL; pchildren = pchildren->next) //LEVEL3
			{
				if (pchildren->type == XML_ELEMENT_NODE)
				{
					if (!setParam(getParam((char*)pchildren->name) /* num */, (char*)xmlNodeGetContent(pchildren) /* val */))
					{
						xmlFreeDoc(doc_ptr);
						return false;
					}
				}
			}
		}
	}
	xmlFreeDoc(doc_ptr);
	return true;
}

bool sanXML::setParam(int num, const char* val)
{
	bool ret_val = true;
	switch (num)
	{
		/* image and device resolution */
	case 100: // camera
		ret_val = read_xy(val, &m_resolution.image);
		break;
	case 101: // camera
		ret_val = read_xy(val, &m_resolution.display);
		break;
		/* camera intrinsic and extrinsic  */
	case 200: case 201: case 202: case 203: // camera
		ret_val = read_rcam_parameters(val, &m_rcam[num - 200]);
		break;
		/* number of control points */
	case 300: case 301:
		ret_val = read_int(val, &m_points_num[num - 300]);
		break;
		/* feature points */
	case 400: case 401: case 402: case 403:
		ret_val = read_feature_points(val, &m_feature_pts[num - 400][0], m_points_num[0]);
		break;
		/* real local points */
	case 500: case 501: case 502: case 503:
		ret_val = read_local_points(val, &m_local_pts[num - 500][0], m_points_num[1]);
		break;
		/* view and layout */
	case 600:
		ret_val = read_layout(val, &m_layout[0]);
		break;
	case 601:
		ret_val = read_layout(val, &m_layout[1]);
		break;
		/* 3D bowl mesh */
	case 700:
		ret_val = read_mesh3D(val, &m_mesh);
		break;
		/* virtual camera */
	case 800: case 801: case 802: case 803: case 810:
		ret_val = read_vcam_pose(val, &m_vcam[num - 800]);
		break;
		/* model car */
	case 900:
		m_model.model_name = std::string(val);
		break;
	case 901:
		ret_val = read_float(val, &m_model.reduction_ratio);
		break;
	case 902:
		ret_val = read_xyz(val, &m_model.translation);
		break;
	case 903:
		ret_val = read_xyz(val, &m_model.rotation);
		break;
	case 904:
		ret_val = read_xyz(val, &m_model.scale);
		break;
		/* pattern and arrangement */
	case 1000:
		ret_val = read_xy(val, &m_pattern.unit);
		break;
	case 1001:
		ret_val = read_xy(val, &m_pattern.tplate[0]);
		break;
	case 1002:
		ret_val = read_xy(val, &m_pattern.tplate[1]);
		break;
		/* real car dimension */
	case 1100:
		ret_val = read_vehicle(val, &m_vehicle);
		break;
	case 9999:
		ret_val = true;
		break;
	default:
		ret_val = false;
		break;
	}

	return ret_val;
}

bool sanXML::read_xy(const char *src, XY *dst)
{
	float buf[2] = {0.0f,};
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 2))
		str >> buf[i++];

	if ((!str) || (i < 2))
	{
		std::cout << "Too much parameters" << std::endl;
		return false;
	}

	dst->x = buf[0];
	dst->y = buf[1];

	return true;
}

bool sanXML::read_xyz(const char * src, XYZ * dst)
{
	float buf[3] = {0};
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 3))
		str >> buf[i++];

	if ((!str) || (i < 3))
	{
		std::cout << "Too much parameters" << std::endl;
		return false;
	}

	dst->x = buf[0];
	dst->y = buf[1];
	dst->z = buf[2];

	return true;
}

bool sanXML::read_rcam_parameters(const char * src, RCAM_PARAMETERS* dst)
{
	float buf[25] = {0};
	std::stringstream str;
	str << src;
	int i = 0;
	while ((str) && (i < 25))
		str >> buf[i++];

	if ((!str) || (i < 25))
	{
		std::cout << "Too much parameters" << std::endl;
		return false;
	}

	dst->fmu = buf[0];
	dst->fmv = buf[1];
	dst->sf = buf[2];
	dst->cx = buf[3];
	dst->cy = buf[4];

	dst->k1 = buf[5];
	dst->k2 = buf[6];
	dst->p1 = buf[7];
	dst->p2 = buf[8];
	dst->k3 = buf[9];

	dst->d0 = buf[10];
	dst->d1 = buf[11];
	dst->d2 = buf[12];
	dst->d3 = buf[13];
	dst->d4 = buf[14];
	dst->d5 = buf[15];
	dst->d6 = buf[16];
	dst->d7 = buf[17];
	dst->d8 = buf[18];

	dst->x = buf[19];
	dst->y = buf[20];
	dst->z = buf[21];
	dst->tilt = buf[22];
	dst->pan = buf[23];
	dst->roll = buf[24];

	return true;
}

bool sanXML::read_local_points(const char* src, XYZ * dst, const int iter)
{
	float buf[48] = {0.0f,};
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 48))
		str >> buf[i++];

	if ((!str) || (i < 48))
	{
		std::cout << "Too much parameters" << std::endl;
		return false;
	}

	for (int num = 0; num < iter; num += 1)
	{
		dst[num].x = buf[num * 3 + 0];
		dst[num].y = buf[num * 3 + 1];
		dst[num].z = buf[num * 3 + 2];
	}

	return true;
}

bool sanXML::read_feature_points(const char* src, XY* dst, const int iter)
{
	float buf[32] = { 0.0f, };
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 32))
		str >> buf[i++];

	if ((!str) || (i < 32))
	{
		std::cout << "Too much parameters" << std::endl;
		return false;
	}

	for (int num = 0; num < iter; num += 1)
	{
		dst[num].x = buf[num * 2 + 0];
		dst[num].y = buf[num * 2 + 1];
	}

	return true;
}


bool sanXML::read_layout(const char * src, LAYOUT * dst)
{
	int buf[6] = {0,};
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 6))
		str >> buf[i++];

	if ((!str) || (i < 6))
	{
		std::cout << "Too much parameters for layout" << std::endl;
		return false;
	}

	dst->x = buf[0];
	dst->y = buf[1];
	dst->width = buf[2];
	dst->height = buf[3];
	dst->view_mode = buf[4];
	dst->animation = (bool)buf[5];

	return true;
}

bool sanXML::read_vcam_pose(const char *src, POSE *dst)
{
	float buf[6] = {0.0f, };
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 6))
		str >> buf[i++];

	if ((!str) || (i < 6))
	{
		std::cout << "Too much parameters for vcam_pose." << std::endl;
		return false;
	}

	dst->pos.x  = buf[0];
	dst->pos.y  = buf[1];
	dst->pos.z  = buf[2];
	dst->dir.x  = buf[3];
	dst->dir.y  = buf[4];
	dst->dir.z  = buf[5];

	return true;

}

bool sanXML::read_mesh3D(const char* src, MESH_BOWL* dst)
{
	float buf[5] = {0.0f,};
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 5))
		str >> buf[i++];

	if ((!str) || (i < 5))
	{
		std::cout << "Too much parameters for mesh3D" << std::endl;
		return false;
	}

	dst->sector_nums = (int)buf[0];
	dst->start_sector_index = (int)buf[1];
	dst->nop_z = (int)buf[2];
	dst->step_x = buf[3];
	dst->radius = buf[4];

	return true;
}

bool sanXML::read_vehicle(const char* src, VEHICLE* dst)
{
	std::string str_buf[2];
	float buf[8] = {0.0f,};
	std::stringstream str;
	str << src;
	int i = 0;

	while ((str) && (i < 10))
	{
		if (i < 2)
			str >> str_buf[i];
		else if ((i >= 2) && (i < 10))
			str >> buf[i - 2];
		i++;
	}

	if ((!str) || (i < 10))
	{
		std::cout << "Too much parameters for vehicle" << std::endl;
		return false;
	}

	dst->customer = str_buf[0];
	dst->vehicle_type_name = str_buf[1];
	dst->length = buf[0];
	dst->width = buf[1];
	dst->height = buf[2];
	dst->reserved0 = buf[3];
	dst->reserved1 = buf[4];
	dst->reserved2 = buf[5];
	dst->reserved3 = buf[6];
	dst->reserved4 = buf[7];

	return true;
}


bool sanXML::read_int(const char* src, int* dst)
{
	errno = 0;
	//*dst = atoi(src);
	*dst = (int)strtol(src, (char **)NULL, 10);

	if (errno == ERANGE)
	{
		std::cout << "can't convert std::string to signed integer." << std::endl;
		return false;
	}

	return true;
}


bool sanXML::read_uint(const char* src, unsigned int* dst)
{
	errno = 0;
	//*dst = atoi(src);

	*dst = (unsigned int)strtoul(src, (char**)NULL, 10);

	if (errno == ERANGE) {
		std::cout << "can't convert std::string to unsigned integer." << std::endl;
		return false;
	}
	return true;
}

bool sanXML::read_float(const char* src, float* dst)
{
	errno = 0;
	//*dst = atof(src);
	*dst = strtof(src, (char**)NULL);

	if (errno == ERANGE) {
		std::cout << "can't convert std::string to float." << std::endl;
		return false;
	}

	return true;
}

bool sanXML::read_bool(const char* src, bool* dst)
{
	errno = 0;
	//int intval = atoi(src);
	int intval = (int)strtol(src, (char**)NULL, 10);

	if (errno == ERANGE) {
		std::cout << "can't convert std::string to bool." << std::endl;
		return false;
	}

	*dst = (intval != 0);
	return true;
}

/*
int sanXML::getTmpMaxVal(const char* filename, int* val)
{
    *val = 0;
    struct stat st;
    std::string ref_points_txt = tmplt + "/" + (string)filename;
    if (stat(ref_points_txt.c_str(), &st) != 0)
    {
        std::cout << "File " << ref_points_txt << " not found" << std::endl;
        return (-1);
    }
    int x, y;
    ifstream ifs_ref(ref_points_txt.c_str());
    while (ifs_ref >> x >> y)
        if (x > *val) *val = x;
    ifs_ref.close();
    return (0);
}
*/

int sanXML::getParam(const char* name)
{
	int tokkenID = 0;
	/* image and device resolution  */
	if (strcmp(name, "image_resolution") == 0) tokkenID = 100;
	else if (strcmp(name, "display_resolution") == 0) tokkenID = 101;
		/* real camera intrinsic and extrinsic  */
	else if (strcmp(name, "rcam0_parameters") == 0) tokkenID = 200;
	else if (strcmp(name, "rcam1_parameters") == 0) tokkenID = 201;
	else if (strcmp(name, "rcam2_parameters") == 0) tokkenID = 202;
	else if (strcmp(name, "rcam3_parameters") == 0) tokkenID = 203;
		/* number of control points */
	else if (strcmp(name, "feature_points_num") == 0) tokkenID = 300;
	else if (strcmp(name, "real_local_points_num") == 0) tokkenID = 301;
		/* feature points */
	else if (strcmp(name, "rcam0_feature") == 0) tokkenID = 400;
	else if (strcmp(name, "rcam1_feature") == 0) tokkenID = 401;
	else if (strcmp(name, "rcam2_feature") == 0) tokkenID = 402;
	else if (strcmp(name, "rcam3_feature") == 0) tokkenID = 403;
		/* real local points */
	else if (strcmp(name, "rcam0_local") == 0) tokkenID = 500;
	else if (strcmp(name, "rcam1_local") == 0) tokkenID = 501;
	else if (strcmp(name, "rcam2_local") == 0) tokkenID = 502;
	else if (strcmp(name, "rcam3_local") == 0) tokkenID = 503;
		/* layout and view */
	else if (strcmp(name, "layout0") == 0) tokkenID = 600;
	else if (strcmp(name, "layout1") == 0) tokkenID = 601;
		/* 3D bowl mesh */
	else if (strcmp(name, "mesh_bowl") == 0) tokkenID = 700;
		/* virtual camera */
	else if (strcmp(name, "vcam0") == 0) tokkenID  = 800;
	else if (strcmp(name, "vcam1") == 0) tokkenID  = 801;
	else if (strcmp(name, "vcam2") == 0) tokkenID  = 802;
	else if (strcmp(name, "vcam3") == 0) tokkenID  = 803;
	else if (strcmp(name, "vcam10") == 0) tokkenID = 810;
		/* model car */
	else if (strcmp(name, "model_name") == 0) tokkenID = 900;
	else if (strcmp(name, "model_reduction_ratio") == 0) tokkenID = 901;
	else if (strcmp(name, "model_translation") == 0) tokkenID = 902;
	else if (strcmp(name, "model_rotation") == 0) tokkenID = 903;
	else if (strcmp(name, "model_scale") == 0) tokkenID = 904;
		/* pattern and arrangement */
	else if (strcmp(name, "unit_pattern") == 0) tokkenID = 1000;
	else if (strcmp(name, "vertical_template") == 0) tokkenID = 1001;
	else if (strcmp(name, "horizontal_template") == 0) tokkenID = 1002;
		/* real car dimension */
	else if (strcmp(name, "vehicle_specification") == 0) tokkenID = 1100;
	else tokkenID = 9999;

	return (tokkenID);
}

bool sanXML::writeXML(const std::vector<glm::vec2> & val, const char * node)
{
	const char * filepath = (const char *)&m_fileDir.c_str()[0];

	xmlDocPtr doc_ptr = xmlReadFile(filepath, NULL, 0);
	if (doc_ptr == NULL)
		return false;

	xmlNodePtr node_ptr = xmlDocGetRootElement(doc_ptr); //LEVEL1
	if (node_ptr == NULL)
	{
		xmlFreeDoc(doc_ptr);
		return false;
	}

	float val_buf[32];
	int idx = 0;
	for (int i = 0; i < val.size(); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			val_buf[idx] = val[i][j];
			idx++;
		}
	}

	for (node_ptr = node_ptr->children; node_ptr != NULL; node_ptr = node_ptr->next) //LEVEL2
	{
		if ((node_ptr->type == XML_ELEMENT_NODE) && (strcmp((char *)node_ptr->name, "feature_points") == 0))
		{
			xmlNodePtr pchildren  = node_ptr->xmlChildrenNode;
			for (pchildren = node_ptr->children; pchildren != NULL; pchildren = pchildren->next) //LEVEL3
			{
				if ((pchildren->type == XML_ELEMENT_NODE) && (strcmp((char *)pchildren->name, node) == 0))
				{
					xmlNodePtr pcchildren = pchildren->xmlChildrenNode;
					int iter = 0;
					for (pcchildren = pchildren->children; pcchildren != NULL; pcchildren = pcchildren->next)
					{
						if (pcchildren->type == XML_ELEMENT_NODE)
						{
							std::string str = std::to_string(val_buf[iter]);
							char * buf = new char[str.size() + 1];
							copy(str.begin(), str.end(), buf);
							buf[str.size()] = '\0';

							xmlNodeSetContent(pcchildren, (xmlChar *)buf);
							iter++;
							delete[] buf;
						}
					}
				}
			}
		}
	}

	xmlSaveFormatFileEnc(filepath, doc_ptr, "UTF-8", 1);
	xmlFreeDoc(doc_ptr);
	xmlCleanupParser();

	return true;
}

/*
bool sanXML::writeXML2(const std::vector<glm::vec2> & val)
{
	time_t curr_time;
	struct tm timeinfo;

	time(&curr_time);
	localtime_s(&timeinfo, &curr_time);

	std::string YEAR = std::to_string(1900 + timeinfo.tm_year);
	std::string MON  = std::to_string(1 + timeinfo.tm_mon);
	std::string DAY  = std::to_string(timeinfo.tm_mday);
	std::string HOUR = std::to_string(timeinfo.tm_hour);
	std::string MIN  = std::to_string(timeinfo.tm_min);
	std::string DATE = YEAR + MON + DAY + HOUR + MIN;

	std::string wfilepath = (std::string)PROJECT_SRC_DIR + std::string("/settings/wfeature_") + DATE + std::string(".xml");
	const char* filepath = (const char*)&wfilepath.c_str()[0];

	xmlDocPtr doc_ptr = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node_ptr = xmlNewNode(NULL, BAD_CAST "ImageFeaturePoints");
	xmlDocSetRootElement(doc_ptr, root_node_ptr);

	xmlNewChild(root_node_ptr, NULL, BAD_CAST "CUSTOMER", BAD_CAST m_vehicle.customer.c_str());
	xmlNewChild(root_node_ptr, NULL, BAD_CAST "VEHICLE_TYPE", BAD_CAST m_vehicle.vehicle_type_name.c_str());

	for (int iter = 0; iter < val.size(); iter++)
	{
		for (int i = 0; i < 2; i++)
		{
			std::string str = std::to_string(val[iter][i]);
			char * buf = new char[str.size() + 1];
			copy(str.begin(), str.end(), buf);
			buf[str.size()] = '\0';

			if (i == 0)
				xmlNewChild(root_node_ptr, NULL, BAD_CAST ("x" + std::to_string(iter)).c_str(), BAD_CAST buf);
			else if (i == 1)
				xmlNewChild(root_node_ptr, NULL, BAD_CAST ("y" + std::to_string(iter)).c_str(), BAD_CAST buf);

			delete[] buf;
		}
	}

	xmlSaveFormatFileEnc(filepath, doc_ptr, "UTF-8", 1);
	xmlFreeDoc(doc_ptr);
	xmlCleanupParser();

	return true;
}*/
