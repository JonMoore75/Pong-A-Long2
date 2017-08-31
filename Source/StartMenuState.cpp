#include "StartMenuState.h"

#include <memory>
#include "Window.h"
#include "Font_TTF.h"
#include "PongState.h"

bool StartMenuState::Initialise()
{
	SDL_assert(m_pWnd);
	Renderer& renderer = m_pWnd->GetRenderer();

	FontTTF arialFont;
	if (!arialFont.LoadFont("C:\\Windows\\Fonts\\ARIAL.TTF", 50, SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF }))
		return false;

	m_MenuText.CreateFromText(renderer, "Start Menu", arialFont);
	m_Choice1Text.CreateFromText(renderer, "Press 1 to Start Game", arialFont);
	m_Choice2Text.CreateFromText(renderer, "Press 2 to Quit", arialFont);

	return true;
}

void StartMenuState::CleanUp()
{

}

void StartMenuState::Render(Renderer& renderer)
{
	m_MenuText.Render(renderer, m_pWnd->GetWidth()/2 - m_MenuText.GetWidth()/2, 100);
	m_Choice1Text.Render(renderer, m_pWnd->GetWidth() / 2 - m_Choice1Text.GetWidth() / 2, 150);
	m_Choice2Text.Render(renderer, m_pWnd->GetWidth() / 2 - m_Choice1Text.GetWidth() / 2, 200);
}

void StartMenuState::Update(double dt)
{

}

GameState::QUITRESPONSE StartMenuState::QuitDialog()
{
	int buttonid = -1;

	const SDL_MessageBoxButtonData buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "No" },
	};

	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL,						/* .window */
		"Quit",						/* .title */
		"Would you like to Quit?",	/* .message */
		SDL_arraysize(buttons),		/* .numbuttons */
		buttons,					/* .buttons */
		NULL						/* .colorScheme */
	};

	int ret = SDL_ShowMessageBox(&messageboxdata, &buttonid);

	if (buttonid == 1)
		return GameState::QUIT;

	return GameState::CANCEL;
}

bool StartMenuState::OnKeyDown(SDL_Scancode scan, SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_1:
		m_newState = std::make_unique<PongState>();
		break;
	case SDLK_2:
		RequestAppQuit();
		break;
	}

	return true;
}
