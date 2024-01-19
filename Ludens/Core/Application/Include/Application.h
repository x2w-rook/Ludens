#pragma once

#include <string>
#include <cstdint>
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
		uint32_t WindowWidth  = LD_APPLICATION_DEFAULT_WINDOW_WIDTH;
		uint32_t WindowHeight = LD_APPLICATION_DEFAULT_WINDOW_HEIGHT;
		double FixedUpdateInterval = LD_APPLICATION_DEFAULT_FIXED_UPDATE_INTERVAL;
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
		void SetWindowCursorNormal();
		void SetWindowCursorGrabbed();


		// TODO: multiple application layers and event propagation,
		//       currently we have only one layer
		inline void PushLayer(Ref<ApplicationLayer> layer)
		{
			mLayer = layer;
			mLayer->OnAttach(*this);
		}

		inline void PopLayer(Ref<ApplicationLayer> layer)
		{
			if (layer != mLayer)
				return;
			mLayer->OnDetach(*this);
			mLayer = nullptr;
		}

		// Application entry point
		void Run();

	private:
		void OnInputEvent(const Event& event);
		void OnInputNewFrame();

		static Application* sInstance;
		bool mHasSetup = false;
		bool mIsRunning = false;
		ApplicationConfig mConfig;
		Own<ApplicationWindow> mWindow = nullptr;
		Ref<ApplicationLayer> mLayer = nullptr;
	};

} // namespace LD