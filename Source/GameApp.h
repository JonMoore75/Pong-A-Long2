#ifndef GameApp_h__
#define GameApp_h__
#pragma once

#include <memory>
#include "Window.h"
#include "TimeKeeper.h"

struct WindowCreationParams
{
								// Resolution, only applies for windowed not fullscreen
	int iWidth = 1024;			//< Required width of the window.
	int iHeight = 768;			//< Required height of the window.

								// Window Params
	bool bFullscreen = false;	//< Run full-screen.
	bool Resizeable = false;	//< Whether window can be resized
	bool bOpenGL = false;		//< Create OpenGL context
	bool bBorderless = false;	//< Create window with no border
	bool bMouseGrab = false;	//< Limits mouse within bounds of window
	bool bMouseCapture = false;	//< Captures the mouse input

									// Renderer Params
	bool bSoftwareRender = false;	//< Whether use software rendering instead of GPU
	bool bVSync = false;			//< Whether to wait for Vsync or present immediately to the user.
	bool bTextureRender = false;	//< Renderer can render to a texture as well as screen

	Uint32 SetRendererCreateFlags();

	Uint32 SetWindowCreateFlags();
};

class GameState;

class GameApp 
{
public:
	GameApp(std::string appname);
	virtual ~GameApp();

	int Execute(WindowCreationParams& createParam, std::unique_ptr<GameState> initial_state);

	Window& GetWindow() { return m_Window; }
	bool IsRunning() { return m_Running; }
	void AppQuit() { m_Running = false; }

	template <class State>
	bool ChangeState();

	bool ChangeState(std::unique_ptr<GameState> new_state);


protected:

	void Cleanup();

	bool Init(WindowCreationParams& createParam, std::unique_ptr<GameState> initial_state);

	void HandleEvents();

	void MainLoop();

	void Render();

	void DrawFramesPerSecond();

protected:
	std::string m_AppName;
	bool	m_Running = false;

	bool	m_ShowFPS = true;

	Window	m_Window;

	TimeKeeper m_Timer;

	std::unique_ptr<GameState>		m_pState;
};

template <class State>
bool GameApp::ChangeState()
{
	return ChangeState(std::make_unique<State>());
}

#endif // GameApp_h__
