#include "window.h"
#include <iostream>

namespace gfx {
	Window::Window(int width, int height, std::string_view title) {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if (!m_window) {
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW Window");
		}
		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize GLAD/OpenGL context");
		}

		glViewport(0, 0, width, height);
		glfwSwapInterval(1); // Enable VSync
	}

	Window::~Window() {
		if (m_window) {
			glfwDestroyWindow(m_window);
		}
		glfwTerminate();
	}

	bool Window::should_close() const {
		return glfwWindowShouldClose(m_window);
	}

	void Window::poll_events() {
		glfwPollEvents();
	}

	void Window::swap_buffers() {
		glfwSwapBuffers(m_window);
	}

	void Window::clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}