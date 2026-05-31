#include <iostream>
#include "gfx/window.h"
#include "core/solver.h"

int main() {
	try {
		gfx::Window window(800, 800, "Stable Fluids (Stam, 1999)");
		
		const int GRID_SIZE = 100;
		const float DT = 0.1f;

		core::Solver solver(GRID_SIZE, GRID_SIZE);

		while (!window.should_close()) {
			window.poll_events();

			solver.solve();
			solver.swap_buffers();

			window.clear(1.0f, 1.0f, 1.0f, 1.0f);

			// TODO: render.draw()

			window.swap_buffers();
		}

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}