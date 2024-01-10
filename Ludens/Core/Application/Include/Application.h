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
		virtual ~Application();

		void Setup(const ApplicationConfig& config);
		void Cleanup();

		static bool EventHandler(const Event& event);

		static Application& Get();
		inline std::string GetName() const { return mConfig.Name; }

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
		static Application* sInstance;
		bool mHasSetup = false;
		bool mIsRunning = false;
		ApplicationConfig mConfig;
		Own<ApplicationWindow> mWindow = nullptr;
		Ref<ApplicationLayer> mLayer;
	};

} // namespace LD