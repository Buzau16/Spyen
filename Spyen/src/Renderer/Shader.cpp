//
// Created by gpro9 on 5/8/2025.
//

#include "Renderer/Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <Math/gtc/type_ptr.hpp>

#include "glad/glad.h"

namespace Spyen
{
	Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
		m_Name(name),
		m_VertexSrc(vertexSrc),
		m_FragmentSrc(fragmentSrc)
	{
		const std::string vertexShader = ReadFile(vertexSrc);
		const std::string fragmentShader = ReadFile(fragmentSrc);

		m_RendererID = glCreateProgram();

		const uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(m_RendererID, vs);
		glAttachShader(m_RendererID, fs);

		glLinkProgram(m_RendererID);

		GLint result;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
			const auto message = static_cast<char*>(alloca(length * sizeof(char)));
			glGetProgramInfoLog(m_RendererID, length, &length, message);
			std::cout << "Failed to link shader program!" << std::endl;
			std::cout << message << std::endl;
			glDeleteProgram(m_RendererID);
			return;
		}

		glValidateProgram(m_RendererID);
		glGetProgramiv(m_RendererID, GL_VALIDATE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
			const auto message = static_cast<char*>(alloca(length * sizeof(char)));
			glGetProgramInfoLog(m_RendererID, length, &length, message);
			std::cout << "Failed to validate shader program!" << std::endl;
			std::cout << message << std::endl;
			glDeleteProgram(m_RendererID);
			return;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() {
		glUseProgram(0);
	}

	uint32_t Shader::CompileShader(uint32_t type, const std::string& source)
	{
		const uint32_t id = glCreateShader(type);
		const char* src = source.c_str();

		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			const auto message = static_cast<char*>(alloca(length * sizeof(char)));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		const std::ifstream file(filepath);
		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filepath << std::endl;
			return "";
		}
		std::stringstream buff;
		buff << file.rdbuf();
		return buff.str();
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		return std::make_shared<Shader>(name, vertexSrc, fragmentSrc);
	}

	void Shader::SetUniform1i(const std::string& name, const int value)
	{
		const GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetUniform1iv(const std::string& name, const int count, const int* value)
	{
		const GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, value);
	}

	void Shader::SetUniform1f(const std::string& name, const float value)
	{
		const GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
	{
		const GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
	{
		const GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
	{
		const GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value)
	{
		const GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}


}
