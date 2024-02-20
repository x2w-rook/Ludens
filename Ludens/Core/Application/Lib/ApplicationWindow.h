#pragma once

#include <string>
#include "Core/Header/Include/Types.h"

class GLFWwindow;

namespace LD {

	struct ApplicationWindowConfig
	{
		std::string Name;
		u32 Width;
		u32 Height;
		bool EnableVsync = true;
	};

	class ApplicationWindow
	{
	public:
		ApplicationWindow() {}
		ApplicationWindow(const ApplicationWindow&) = delete;
		~ApplicationWindow() {}

		ApplicationWindow& operator=(const ApplicationWindow&) = delete;

		void Setup(const ApplicationWindowConfig& config);
		void Cleanup();

		// elapsed time in seconds since window Setup
		double GetTime();

		// mouse cursor position in screen space
		void GetCursorPosition(float& screenX, float& screenY);
		void* GetHandle();
		inline std::string GetName() const { return mName; }
		inline u32 GetWidth() const { return mWidth; }
		inline u32 GetHeight() const { return mHeight; }
		inline float GetAspectRatio() const { return (float)mWidth / (float)mHeight; }
		
		void PollEvents();
		void SwapBuffers();
		bool IsAlive();

		void SetCursorHidden();
		void SetCursorNormal();
		void SetCursorGrabbed();

	private:
		void SetupCallbacks();

		std::string mName;
		u32 mWidth = 0;
		u32 mHeight = 0;
		GLFWwindow* mHandle = nullptr;
		bool mHasSetup = false;
	};

}; // namespace LD