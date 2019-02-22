#pragma once
#ifndef H_SHADER
#define H_SHADER

#include "context.h"

class ShaderManager
{
public:
	ShaderManager(const char* name) : fstream_(name), src_(nullptr), ID_(glCreateProgram())
	{
		//shader compile-time error check section
		int  success;
		char infoLog[512];

		//read shader code to src_
		unsigned int length = fstream_.getFileBytesize();
		src_ = new char[length + 1]; //+1 for '\0'
		fstream_.read(src_, length);
		src_[length] = '\0';

		const int types[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
		const char* info[2][2] {
		{ "#version 330 core\n#define VERTEX\n#line 0\n", src_ },
		{ "#version 330 core\n#define FRAGMENT\n#line 0\n", src_ }};

		for (size_t i = 0; i < 2; ++i)
		{
			GLuint shaderObj = glCreateShader(types[i]);
			glShaderSource(shaderObj, 2, info[i], NULL);
			glCompileShader(shaderObj);

			//shader compile-time errors check
			glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderObj, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info[i] << '\n' << infoLog << std::endl;
			}
			
			glAttachShader(ID_, shaderObj);
			glDeleteShader(shaderObj);
		}

		delete[] src_; //TODO move to ~?

		glLinkProgram(ID_);

		//linked shader program compile-time errors check
		glGetProgramiv(ID_, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	~ShaderManager() { glDeleteProgram(ID_); };

	void use() const { glUseProgram(ID_); }

	void setInt(const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID_, name), value);
	}
	const GLuint getShaderID() const { return ID_; };
	
private:
	GLuint ID_;
	FStream fstream_;
	char* src_;
};
#endif //H_SHADER