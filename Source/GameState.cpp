#include "GameState.h"


void GameState::RequestAppQuit()
{
	SDL_Event Event;
	Event.type = SDL_QUIT;
	SDL_PushEvent(&Event);
}
