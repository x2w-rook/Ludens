#pragma once

#include <string>

class GLFWwindow;

namespace LD {

	struct ApplicationWindowConfig
	{
		std::string Name;
		uint32_t Width;
		uint32_t Height;
		bool EnableVsync = true;
	};

	class ApplicationWindow
	{
	public:
		ApplicationWindow() {}
		~ApplicationWindow() {}

		void Setup(const ApplicationWindowConfig& config);
		void Cleanup();

		// elapsed time in seconds since window Setup
		double GetTime();
		
		void PollEvents();
		void SwapBuffers();
		bool IsAlive();

	private:
		void SetupCallbacks();

		ApplicationWindowConfig mConfig;
		GLFWwindow* mHandle = nullptr;
		bool mHasSetup = false;
	};

}; // namespace LD