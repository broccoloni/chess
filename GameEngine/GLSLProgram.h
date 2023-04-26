#pragma once
#include <string>
#include <GL/glew.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

class GLSLProgram
{
	public:
		GLSLProgram();
		~GLSLProgram();
		
		void compileShaders(const std::string& vertpath, const std::string& fragpath);

		void linkShaders();

		void addAttribute(const std::string& attribName);

		void use();

		void unuse();

		GLint getUniformLocation(const std::string& uniformName);
	private: 
		void compileShader(const std::string& filepath, GLuint id);
		GLuint m_programID;
		GLuint m_vertShaderID;
		GLuint m_fragShaderID;
		unsigned int m_numAttributes;
		
};





