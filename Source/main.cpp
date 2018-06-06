#include "Application.h"

int main(int argc, char** argv) {
	Quark::Application app;
	Quark::PlatformContext pc;
	pc.name = "test";
	if (!app.Initialize(pc)) {
		return 0;
	}
	app.Run();

	return 0;
}