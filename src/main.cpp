#include <iostream>
#include "gfx/window.h"
#include "gfx/renderer.h"
#include "core/solver.h"
#include <cmath>

int main() {
	try {
		const int WINDOW_SIZE = 800;
		gfx::Window window(WINDOW_SIZE, WINDOW_SIZE, "Stable Fluids (Stam, 1999)");
		GLFWwindow* glfw_win = window.get_glfw_window();
		
		const int GRID_SIZE = 200;
		const float DT = 0.1f;
		const float GRID_WINDOW_RATIO = GRID_SIZE/WINDOW_SIZE;

		core::Solver solver(GRID_SIZE, GRID_SIZE, DT);
		gfx::Renderer renderer(GRID_SIZE, GRID_SIZE);

		solver.add_ink_source(25, 25, 5.0f);

		double prev_mouse_x = 0.0;
		double prev_mouse_y = 0.0;
		bool is_first_click = true;

		while (!window.should_close()) {
			window.poll_events();

			double curr_mouse_x, curr_mouse_y;
			glfwGetCursorPos(glfw_win, &curr_mouse_x, &curr_mouse_y);

			if (glfwGetMouseButton(glfw_win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				if (is_first_click) {
					prev_mouse_x = curr_mouse_x;
					prev_mouse_y = curr_mouse_y;
					is_first_click = false;
				}

				float force_x = static_cast<float>(curr_mouse_x - prev_mouse_x);
				float force_y = static_cast<float>(curr_mouse_y - prev_mouse_y);

				int grid_x = (int)round(prev_mouse_x*GRID_WINDOW_RATIO);
				int grid_y = (int)round(prev_mouse_y*GRID_WINDOW_RATIO);
				
				solver.add_force_source(grid_x, grid_y, force_x, force_y);
			} else {
				is_first_click = true;
			}

			prev_mouse_x = curr_mouse_x;
    		prev_mouse_y = curr_mouse_y;
			
			solver.step();
			solver.swap_buffers();

			window.clear(1.0f, 1.0f, 1.0f, 1.0f);

			renderer.draw(solver.get_ink_density());

			window.swap_buffers();
		}

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}