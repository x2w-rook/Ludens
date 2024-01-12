#include "Core/Application/Include/Application.h"
#include "Core/Application/Include/ApplicationLayer.h"
#include "Core/Application/Lib/ApplicationWindow.h"
#include "Core/Header/Include/Error.h"
#include "Core/OS/Include/Time.h"


namespace LD {

	// singleton instance
	Application* Application::sInstance = nullptr;


	Application::Application()
	{
		LD_DEBUG_ASSERT(sInstance == nullptr && "Application is a singleton class");

		sInstance = this;
	}

	Application::~Application()
	{
		sInstance = nullptr;
	}

	void Application::Setup(const ApplicationConfig& config)
	{
		LD_DEBUG_ASSERT(sInstance == this);

		mConfig = config;

		ApplicationWindowConfig windowConfig{};
		windowConfig.EnableVsync = true;
		windowConfig.Name = mConfig.Name;
		windowConfig.Width = mConfig.WindowWidth;
		windowConfig.Height = mConfig.WindowHeight;

		mWindow = MakeOwn<ApplicationWindow>();
		mWindow->Setup(windowConfig);

		mHasSetup = true;
	}

	void Application::Cleanup()
	{
		mHasSetup = false;

		mWindow->Cleanup();
		mWindow = nullptr;

		sInstance = nullptr;
	}

	bool Application::EventHandler(const Event& event)
	{
		Application& app = Application::Get();

		if (event.IsApplicationEvent())
		{
			switch (event.Type)
			{
			case EventType::ApplicationQuit:
				app.mIsRunning = false;
				return true;
			default:
				break;
			}
			
			// Application events are consumed and not forwarded to layers
			return true;
		}
		
		if (event.IsInputEvent())
		{
			app.OnInputEvent(event);

			// Input events will be forwarded to layers, and they can choose
			// to act on events or directly or poll the input state via Input.h
		}

		if (!app.mLayer)
			return false;

		return app.mLayer->OnEvent(event);
	}

	Application& Application::Get()
	{
		LD_DEBUG_ASSERT(sInstance != nullptr);

		return *sInstance;
	}

	void Application::Run()
	{
		LD_DEBUG_ASSERT(mHasSetup);

		// NOTE: currently uses the platform's window system timer
		//       which may or may not be the OS clock
		double timeLastFrame;
		double timeThisFrame = mWindow->GetTime();
		double fixedUpdateTimer = 0.0f;

		mIsRunning = true;
		while (mIsRunning && mWindow->IsAlive())
		{
			timeLastFrame = timeThisFrame;
			timeThisFrame = mWindow->GetTime();
			DeltaTime dt(timeThisFrame - timeLastFrame);

			OnInputNewFrame();

			mWindow->PollEvents();

			fixedUpdateTimer += dt.GetSeconds();
			if (fixedUpdateTimer > mConfig.FixedUpdateInterval)
			{
				fixedUpdateTimer -= mConfig.FixedUpdateInterval;
				mLayer->OnFixedUpdate({ mConfig.FixedUpdateInterval });
			}
			
			mLayer->OnDeltaUpdate(dt);

			mWindow->SwapBuffers();
		}
	}

} // namespace LD