#include "renderer.h"
#include <iostream>

namespace gfx{
	Renderer::Renderer(int width, int height): m_width(width), m_height(height)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_R32F,
			m_width, m_height, 0,
			GL_RED,
			GL_FLOAT, 
			nullptr
		);

		glBindTexture(GL_TEXTURE_2D, 0); //Unbind for safety
	}

	Renderer::~Renderer() {
		glDeleteTextures(1, &texture);
	}

	void Renderer::draw(std::span<const float> density_data) {
	
	}
}