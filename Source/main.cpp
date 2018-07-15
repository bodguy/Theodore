#include "Application.h"

using namespace Theodore;

int main(int argc, char** argv) {
	Application app;
	PlatformContext context;
	context.multisample = 16;

	if (app.Initialize(context)) {
		app.Run();
	}

	return 0;
}