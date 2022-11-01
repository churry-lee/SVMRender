#include "svmMain.hpp"

int main(int argc, const char** argv)
{
	GLFWwindow * window = nullptr;
	try
	{
		if (!glfwInit())
		{
			const char* description = nullptr;
			glfwGetError(&description);
			throw logging("failed to initialize glfw " + std::string(description));
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
		printf("Create %d x %d window\n", WINDOW_WIDTH, WINDOW_HEIGHT);
		GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
		if (!window)
			throw logging("failed to create glfw window");

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw logging("failed to initialize glad");

		const GLubyte *glVersion = glGetString(GL_VERSION);
		printf("OpenGL context version: %s\n", glVersion);

		std::unique_ptr<Context> context = std::make_unique<Context>();
		context->Init();

		glfwSetWindowUserPointer(window, &context);
		glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
		glfwSetKeyCallback(window, OnKeyEvent);

		while (!glfwWindowShouldClose(window))
		{
			context->ProcessInput(window);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			context->Render();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		context.reset();
		return 0;
	}
	catch (std::string msg)
	{
		std::cout << msg << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
}

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height)
{
    printf("framebuffer size changed: (%d x %d)", width, height);
    auto context = (Context*)glfwGetWindowUserPointer(window);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    printf("key: %d, scancode: %d, action: %s, mods: %s%s%s\n",
			key, scancode,
			action == GLFW_PRESS ? "Pressed" :
			action == GLFW_RELEASE ? "Released" :
			action == GLFW_REPEAT ? "Repeat" : "Unknown",
			mods & GLFW_MOD_CONTROL ? "C" : "-",
			mods & GLFW_MOD_SHIFT ? "S" : "-",
			mods & GLFW_MOD_ALT ? "A" : "-");

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
