#include <GLFW/glfw3.h>
#include "Core/Application/Lib/ApplicationWindow.h"
#include "Core/Application/Include/Application.h"
#include "Core/Header/Include/Error.h"

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

		SetupCallbacks();

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

	void ApplicationWindow::GetCursorPosition(float& screenX, float& screenY)
	{
		LD_DEBUG_ASSERT(mHasSetup);

		double x, y;
		glfwGetCursorPos(mHandle, &x, &y);
		screenX = (float)x;
		screenY = (float)y;
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

	void ApplicationWindow::SetupCallbacks()
	{
		glfwSetKeyCallback(mHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				KeyPressedEvent event;
				event.Key = static_cast<KeyCode>(key);
				event.Repeat = action == GLFW_REPEAT;
				EventDispatch(event, &Application::EventHandler);
			}
			else if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent event;
				event.Key = static_cast<KeyCode>(key);
				EventDispatch(event, &Application::EventHandler);
			}
		});

		glfwSetMouseButtonCallback(mHandle, [](GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent event;
				event.Button = static_cast<MouseButton>(button);
				EventDispatch(event, &Application::EventHandler);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent event;
				event.Button = static_cast<MouseButton>(button);
				EventDispatch(event, &Application::EventHandler);
			}
		});

		glfwSetCursorPosCallback(mHandle, [](GLFWwindow* window, double xpos, double ypos) {
			MouseMotionEvent event;
			event.XPos = (float)xpos;
			event.YPos = (float)ypos;
			EventDispatch(event, &Application::EventHandler);
		});

		glfwSetScrollCallback(mHandle, [](GLFWwindow* window, double xoffset, double yoffset) {
			MouseScrolledEvent event;
			event.XOffset = (float)xoffset;
			event.YOffset = (float)yoffset;
			EventDispatch(event, &Application::EventHandler);
		});
	}

} // namespace LD