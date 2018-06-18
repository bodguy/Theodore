#include "Application.h"

using namespace Quark;

int main(int argc, char** argv) {
	Application app;
	PlatformContext context;

	if (app.Initialize(context)) {
		app.Run();
	}

	return 0;
}