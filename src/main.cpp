#include <iostream>
#include "gfx/window.h"
#include "gfx/renderer.h"
#include "core/solver.h"
#include <cmath>

int main() {
	try {
		const int WINDOW_SIZE = 1000;
		gfx::Window window(WINDOW_SIZE, WINDOW_SIZE, "Stable Fluids (Stam, 1999)");
		GLFWwindow* glfw_win = window.get_glfw_window();
		
		const int GRID_SIZE = 200;
		const float DT = 0.0167f;
		const float GRID_WINDOW_RATIO = float(GRID_SIZE)/float(WINDOW_SIZE);

		core::Solver solver(GRID_SIZE, GRID_SIZE, DT);
		gfx::Renderer renderer(GRID_SIZE, GRID_SIZE);

		double prev_mouse_x = 0.0;
		double prev_mouse_y = 0.0;
		double curr_mouse_x, curr_mouse_y;

		double prev_time = glfwGetTime();
		int frame_count = 0;

		while (!window.should_close()) {
			window.poll_events();
			double current_time = glfwGetTime();
			frame_count++;
			if (current_time - prev_time >= 1.0) {
				std::string title = "Stable Fluids (Stam, 1999) - FPS: " + std::to_string(frame_count);
				glfwSetWindowTitle(glfw_win, title.c_str());
				frame_count = 0;
				prev_time = current_time;
			}
			
			// TODO: figure out why mouse click holding is being interrupted
			glfwGetCursorPos(glfw_win, &curr_mouse_x, &curr_mouse_y);
			bool is_left_pressed = (glfwGetMouseButton(glfw_win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
			bool is_right_pressed = (glfwGetMouseButton(glfw_win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

			if (is_left_pressed || is_right_pressed) {
				// TODO normalize to just get mouse direction and force will be a constant magnitude
				float force_x = static_cast<float>(curr_mouse_x - prev_mouse_x)*100.0f;
				float force_y = static_cast<float>(-(curr_mouse_y - prev_mouse_y))*100.0f;

				int center_x = (int)round(prev_mouse_x * GRID_WINDOW_RATIO);
				int center_y = (int)round(GRID_SIZE - prev_mouse_y * GRID_WINDOW_RATIO);

				int radius = ceil(0.02f*float(GRID_SIZE));
				for (int j = -radius; j <= radius; j++) {
					for (int i = -radius; i <= radius; i++) {
						int current_x = center_x + i;
						int current_y = center_y + j;
						if (current_x >= 0 && current_x < GRID_SIZE && current_y >= 0 && current_y < GRID_SIZE) {
							float dist_squared = (float)(i*i + j*j);
							float weight = exp(-dist_squared / (float)(radius));
							if (is_left_pressed) {
								solver.add_force_source(current_x, current_y, force_x*weight, force_y*weight);
							} else if (is_right_pressed){
								solver.add_ink_source(current_x, current_y, weight);
							}
						}
					}
				}
			}
			
			prev_mouse_x = curr_mouse_x;
    		prev_mouse_y = curr_mouse_y;
			
			//solver.add_ink_source(GRID_SIZE/2, GRID_SIZE/2, 5.0f);
			solver.swap_buffers();

			solver.step();
			
			window.clear(1.0f, 1.0f, 1.0f, 1.0f);

			//renderer.draw(solver.get_velocity_magnitude()); // TODO: make it so the render draws the velocity in 2 colors based on the magnitude and direction
			renderer.draw(solver.get_ink_density());

			window.swap_buffers();
		}

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}