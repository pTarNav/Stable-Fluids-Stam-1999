#pragma once
#include <glad/glad.h>
#include <span>
#include <string>


namespace gfx{
	class Renderer{
		public:
			Renderer(int width, int height);
			~Renderer();

			void draw(std::span<const float> data);
			
		private:
			int m_width;
			int m_height;

			GLuint vao;
			GLuint vbo;
			GLuint texture;
			GLuint shader_program;

	};
}