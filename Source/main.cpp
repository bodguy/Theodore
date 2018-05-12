#include "Application.h"

int main(int argc, char** argv) {
	Quark::Application app;
	if (!app.Initialize("Engine Test", 800, 600, false)) {
		return 0;
	}
	app.Run();

	return 0;
}