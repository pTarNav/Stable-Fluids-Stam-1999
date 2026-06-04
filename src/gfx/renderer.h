#pragma once
#include <glad/glad.h>
#include <span>
#include <string>


namespace gfx{
	enum class RenderMode {
		INK,
		VELOCITY
	};

	class Renderer{
		public:
			Renderer(int width, int height);
			~Renderer();

			void draw(std::span<const float> data, RenderMode mode);
			
		private:
			int m_width;
			int m_height;

			GLuint vao;
			GLuint vbo;
			GLuint texture;

			GLuint ink_shader_program;
    		GLuint velocity_shader_program;

	};
}