#include <iostream>
#include <GLFW/glfw3.h>
#include "Core/Application/Lib/ApplicationWindow.h"
#include "Core/Application/Include/Application.h"
#include "Core/OS/Include/Error.h"

namespace LD {

	void ApplicationWindow::Setup(const ApplicationWindowConfig& config)
	{
		mConfig = config;

		int result = glfwInit();
		LD_DEBUG_ASSERT(result == GLFW_TRUE);

		// TODO: currently GLFW creates a graphics context for us,
		//       we should be managing this ourselves
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		mHandle = glfwCreateWindow((int)mConfig.Width, (int)mConfig.Height, mConfig.Name.c_str(), nullptr, nullptr);
		LD_DEBUG_ASSERT(mHandle != nullptr);

		glfwMakeContextCurrent(mHandle);
		glfwSwapInterval(mConfig.EnableVsync ? 1 : 0);

		// TODO: event propagation
		glfwSetKeyCallback(mHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS)
				std::cout << "glfw key " << key << std::endl;
		});

		mHasSetup = true;
	}

	void ApplicationWindow::Cleanup()
	{
		mHasSetup = false;

		glfwDestroyWindow(mHandle);
		glfwTerminate();
	}

	double ApplicationWindow::GetTime()
	{
		LD_DEBUG_ASSERT(mHasSetup);

		return glfwGetTime();
	}

	void ApplicationWindow::PollEvents()
	{
		LD_DEBUG_ASSERT(mHasSetup);

		glfwPollEvents();
	}

	void ApplicationWindow::SwapBuffers()
	{
		LD_DEBUG_ASSERT(mHasSetup);

		// TODO: this is a graphics context operation, related to the image swap chain.
		//       currently we have GLFW manage our OpenGL context.
		glfwSwapBuffers(mHandle);
	}

	bool ApplicationWindow::IsAlive()
	{
		LD_DEBUG_ASSERT(mHasSetup);

		return !glfwWindowShouldClose(mHandle);
	}

} // namespace LD