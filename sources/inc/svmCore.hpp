#ifndef SVMCORE_HPP_
#define SVMCORE_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"

const int SVM_CAM_NUM = 4;
const int ANIMATION_STEPS = 20;
typedef enum { TOPVIEW3D = 0, CAMVIEW_FRONT = 1, CAMVIEW_RIGHT = 2, CAMVIEW_REAR = 3, CAMVIEW_LEFT = 4 } VIEWMODE;

#define logging(msg) (msg)

glm::vec3 GetAttenuationCoeff(float distance);

#endif //SVMCORE_HPP_
