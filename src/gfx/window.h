#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>

namespace gfx {
	class Window {
		public:
			Window(int width, int height, std::string_view title);
			~Window();

			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;

			bool should_close() const;
			void poll_events();
			void swap_buffers();
			void clear(float r, float g, float b, float a);

			GLFWwindow* get_glfw_window();

		private:
			GLFWwindow* m_window{nullptr};
		};
}