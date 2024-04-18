#pragma once

#include <string>
#include <cstdint>
#include "Core/Header/Include/Types.h"
#include "Core/RenderBase/Include/RTypes.h"
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

	struct ApplicationWindowConfig
	{
		std::string Name;
		u32 Width;
		u32 Height;
		bool EnableVsync = true;
		bool IsVisible = true;
	};

	struct ApplicationConfig
	{
		ApplicationWindowConfig Window;
		double FixedUpdateInterval = LD_APPLICATION_DEFAULT_FIXED_UPDATE_INTERVAL;
		RBackend RendererBackend;
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

		void Startup(const ApplicationConfig& config);
		void Cleanup();

		inline std::string GetName() const { return mConfig.Window.Name; }
		inline RBackend GetRendererBackend() const { return mConfig.RendererBackend; }
		void* GetWindowHandle() const;
		void GetWindowSize(u32* width, u32* height) const;
		void SetWindowCursorNormal();
		void SetWindowCursorGrabbed();

		// Application entry point
		void Run();

	private:
		void OnInputEvent(const Event& event);
		void OnInputNewFrame();

		static Application* sInstance;
		bool mHasStartup = false;
		bool mIsRunning = false;
		bool mIsMinimized = false;
		ApplicationConfig mConfig;
		ApplicationWindow* mWindow = nullptr;
		Ref<ApplicationLayer> mLayer = nullptr;
	};

} // namespace LD
