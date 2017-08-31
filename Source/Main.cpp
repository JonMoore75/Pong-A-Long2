#include <memory>

#include "GameApp.h"
#include "StartMenuState.h"

int main(int argc, char *argv[])
{
	WindowCreationParams createParam;

	GameApp app("Moving Ball");
	return app.Execute(createParam, std::make_unique<StartMenuState>());
}
