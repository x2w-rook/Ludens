#include "Core/Application/Include/Application.h"
#include "Core/Application/Include/ApplicationLayer.h"
#include "Core/Application/Lib/ApplicationWindow.h"
#include "Core/Header/Include/Error.h"
#include "Core/OS/Include/Time.h"
#include "Core/OS/Include/Memory.h"


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

	void Application::Startup(const ApplicationConfig& config)
	{
		LD_DEBUG_ASSERT(sInstance == this);
		LD_DEBUG_ASSERT(config.Layer != nullptr);

		mConfig = config;

		mWindow = new ApplicationWindow();
		mWindow->Startup(mConfig.Window, this);
		mLayer = config.Layer;
		mLayer->OnAttach(*this);

		mHasStartup = true;
	}

	void Application::Cleanup()
	{
		mHasStartup = false;

		mLayer->OnDetach(*this);
		mWindow->Cleanup();
		delete mWindow;
		mWindow = nullptr;

		sInstance = nullptr;
	}

	void* Application::GetWindowHandle() const
	{
		return static_cast<void*>(mWindow->GetHandle());
	}

	void Application::GetWindowSize(u32* width, u32* height) const
	{
		if (width)
			*width = mWindow->GetWidth();
		if (height)
			*height = mWindow->GetHeight();
	}

	void Application::SetWindowCursorNormal()
	{
		mWindow->SetCursorNormal();
	}

	void Application::SetWindowCursorGrabbed()
	{
		mWindow->SetCursorGrabbed();
	}

	bool Application::EventHandler(const Event& event)
	{
		Application& app = Application::Get();

		if (event.IsApplicationEvent())
		{
			switch (event.Type)
			{
			case EventType::ApplicationQuit:
			{
				app.mIsRunning = false;
				return true; // consume event
			}
			case EventType::ApplicationWindowResize:
			{
				auto e = static_cast<const ApplicationWindowResizeEvent&>(event);
				app.mIsMinimized = (e.Width == 0 || e.Height == 0);
				break;
			}
			default:
				break;
			}
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
		LD_DEBUG_ASSERT(mHasStartup);

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
