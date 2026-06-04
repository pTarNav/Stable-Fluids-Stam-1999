#include "renderer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <span>

namespace gfx{
	float quad_vertices[] = {
		// X, Y, U, V
		-1.0f, 1.0f, 0.0f, 1.0f, // Top-left
		-1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		1.0f, 1.0f, 1.0f, 1.0f, // Top-right
		1.0f, -1.0f, 1.0f, 0.0f // Bottom-right
	};

	std::string load_shader_from_file(const char* filepath) {
		std::ifstream file(filepath);

		if (!file.is_open()) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filepath << std::endl;
			return "";
		}

		std::stringstream shaderStream;
		shaderStream << file.rdbuf();
		
		file.close();
	
		return shaderStream.str();
	}

	GLuint compile_shader(GLenum type, const std::string& source) {
		GLuint shader = glCreateShader(type);
	
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
	
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return shader;
	}

	Renderer::Renderer(int width, int height): m_width(width), m_height(height)
	{
		// -- 1. Textures --

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RG32F,
			m_width, m_height, 0,
			GL_RG,
			GL_FLOAT, 
			nullptr
		);

		glBindTexture(GL_TEXTURE_2D, 0); //Unbind for safety
		
		// -- 2. Vertex arrays and buffers --

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

		// Position mapping
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Texture mapping
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// -- 3. Shaders --
		
		std::string vertex_code = load_shader_from_file("assets/shaders/fluid.vert");
		std::string ink_fragment_code = load_shader_from_file("assets/shaders/ink.frag");
		std::string velocity_fragment_code = load_shader_from_file("assets/shaders/velocity.frag");

		GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_code);
		GLuint ink_fragment_shader = compile_shader(GL_FRAGMENT_SHADER, ink_fragment_code);
		GLuint velocity_fragment_shader = compile_shader(GL_FRAGMENT_SHADER, velocity_fragment_code);

		// Link ink shader
		ink_shader_program = glCreateProgram();
		glAttachShader(ink_shader_program, vertex_shader);
		glAttachShader(ink_shader_program, ink_fragment_shader);
		glLinkProgram(ink_shader_program);

		// Link velocity shader
		velocity_shader_program = glCreateProgram();
		glAttachShader(velocity_shader_program, vertex_shader);
		glAttachShader(velocity_shader_program, velocity_fragment_shader);
		glLinkProgram(velocity_shader_program);

		glDeleteShader(vertex_shader);
		glDeleteShader(ink_fragment_shader);
		glDeleteShader(velocity_fragment_shader);
	}

	Renderer::~Renderer() {
		glDeleteTextures(1, &texture);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteProgram(ink_shader_program);
		glDeleteProgram(velocity_shader_program);
	}

	void Renderer::draw(std::span<const float> data, RenderMode mode){
		glBindTexture(GL_TEXTURE_2D, texture);

		GLenum data_format = (mode == RenderMode::VELOCITY) ? GL_RG : GL_RED;

		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0, 0, m_width, m_height,
			data_format,
			GL_FLOAT,
			data.data()
		);

		GLuint active_shader = (mode == RenderMode::VELOCITY) ? velocity_shader_program : ink_shader_program;
    	glUseProgram(active_shader);
		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Unbind for safety
		glBindVertexArray(0);
    	glUseProgram(0);
	}
}