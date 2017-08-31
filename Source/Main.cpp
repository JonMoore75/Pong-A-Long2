#include <memory>

#include "GameApp.h"
#include "StartMenuState.h"
#include "PongState.h"

int main(int argc, char *argv[])
{
	WindowCreationParams createParam;

	GameApp app("Moving Ball");
	app.Register<StartMenuState>("MENUSTATE");
	app.Register<PongState>("PONGSTATE");

	return app.Execute(createParam, std::make_unique<StartMenuState>());
}
