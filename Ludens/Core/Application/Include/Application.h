#pragma once

#include <string>
#include <cstdint>
#include "Core/Header/Include/Types.h"
#include "Core/Application/Include/Event.h"
#include "Core/Application/Include/ApplicationLayer.h"
#include "Core/OS/Include/Memory.h"


#define LD_APPLICATION_DEFAULT_FIXED_UPDATE_INTERVAL  (1.0 / 60.0)
#define LD_APPLICATION_DEFAULT_WINDOW_WIDTH           1600;
#define LD_APPLICATION_DEFAULT_WINDOW_HEIGHT          900;


namespace LD {

	class ApplicationWindow;
	class ApplicationLayer;
	class Event;


	struct ApplicationConfig
	{
		std::string Name;
		u32 WindowWidth  = LD_APPLICATION_DEFAULT_WINDOW_WIDTH;
		u32 WindowHeight = LD_APPLICATION_DEFAULT_WINDOW_HEIGHT;
		double FixedUpdateInterval = LD_APPLICATION_DEFAULT_FIXED_UPDATE_INTERVAL;
		Ref<ApplicationLayer> Layer;
	};


	class Application
	{
	public:
		Application();
		Application(const Application&) = delete;
		virtual ~Application();

		Application& operator=(const Application&) = delete;

		static bool EventHandler(const Event& event);
		static Application& Get();

		void Setup(const ApplicationConfig& config);
		void Cleanup();

		inline std::string GetName() const { return mConfig.Name; }
		void* GetWindowHandle();
		void GetWindowSize(u32* width, u32* height) const;
		void SetWindowCursorNormal();
		void SetWindowCursorGrabbed();

		// Application entry point
		void Run();

	private:
		void OnInputEvent(const Event& event);
		void OnInputNewFrame();

		static Application* sInstance;
		bool mHasSetup = false;
		bool mIsRunning = false;
		bool mIsMinimized = false;
		ApplicationConfig mConfig;
		Own<ApplicationWindow> mWindow = nullptr;
		Ref<ApplicationLayer> mLayer = nullptr;
	};

} // namespace LD