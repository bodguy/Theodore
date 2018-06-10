#include "Application.h"

using namespace Quark;

int main(int argc, char** argv) {
	Application app;
	PlatformContext pc;
	pc.name = "test";
	if (!app.Initialize(pc)) {
		return 0;
	}
	app.Run();

	return 0;
}