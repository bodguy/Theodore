#include "Application.h"

using namespace Theodore;

int main(int argc, char** argv) {
	Application app;
	PlatformContext context;

	if (app.Initialize(context)) {
		app.Run();
	}

	return 0;
}