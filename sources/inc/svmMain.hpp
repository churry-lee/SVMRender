#ifndef _SVMMAIN_HPP_
#define _SVMMAIN_HPP_

#include "svmContext.hpp"

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height);
void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
void OnCursorPos(GLFWwindow* window, double x, double y);
void OnMouseButton(GLFWwindow* window, int button, int action, int modifier);

#endif //_SVMMAIN_HPP_
