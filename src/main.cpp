#include "gfx/window.h"
#include <iostream>

int main() {
	try {
		gfx::Window window(800, 800, "Stable Fluids (Stam, 1999)");
		while (!window.should_close()) {
			window.poll_events();
			window.clear(1.0f, 1.0f, 1.0f, 1.0f);
			window.swap_buffers();
		}

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}