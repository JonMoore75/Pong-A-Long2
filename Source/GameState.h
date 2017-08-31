#ifndef GameState_h__
#define GameState_h__
#pragma once

#include <memory>
#include <SDL_assert.h>
#include "EventHandler.h"

class Renderer;
class Window;

class GameState : public EventHandler
{
public:
	GameState() {}
	virtual ~GameState() { }

	enum QUITRESPONSE { QUIT, CANCEL, NEWSTATE};

	virtual bool Initialise() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void CleanUp() = 0;
	virtual QUITRESPONSE QuitDialog() { return QUIT; }

	void SetWindow(Window* pWnd) { m_pWnd = pWnd; }

	std::unique_ptr<GameState>& GetNewState() { return m_newState; }

protected:
	void RequestAppQuit();

	Window* m_pWnd = nullptr;

	std::unique_ptr<GameState> m_newState;
};

#endif GameState_h__
