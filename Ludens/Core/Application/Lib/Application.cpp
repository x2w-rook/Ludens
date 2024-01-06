#include "Core/Application/Include/Application.h"
#include "Core/Application/Include/ApplicationLayer.h"
#include "Core/Application/Lib/ApplicationWindow.h"
#include "Core/OS/Include/Error.h"
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
		while (mIsRunning)
		{
			timeLastFrame = timeThisFrame;
			timeThisFrame = mWindow->GetTime();
			DeltaTime dt(timeThisFrame - timeLastFrame);

			mWindow->PollEvents();

			fixedUpdateTimer += dt.GetSeconds();
			if (fixedUpdateTimer > mConfig.FixedUpdateInterval)
			{
				fixedUpdateTimer -= mConfig.FixedUpdateInterval;
				mLayer->OnFixedUpdate({ mConfig.FixedUpdateInterval });
			}
			
			mLayer->OnDeltaUpdate(dt);

			mWindow->SwapBuffers();
			mIsRunning = mWindow->IsAlive();
		}
	}

} // namespace LD