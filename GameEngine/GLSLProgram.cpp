#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram() :  m_programID(0), m_vertShaderID(0), m_fragShaderID(0), m_numAttributes(0)
{

}

GLSLProgram::~GLSLProgram()
{

}

void GLSLProgram::compileShaders(const std::string& vertpath, const std::string& fragpath){
	m_programID = glCreateProgram();
	m_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (m_vertShaderID == 0){
		fatalError("Vertex shader failed to be created!");
	}

	m_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (m_fragShaderID == 0){
		fatalError("Fragment shader failed to be created!");
	}

	compileShader(vertpath, m_vertShaderID);
	compileShader(fragpath, m_fragShaderID);
	
}

void GLSLProgram::linkShaders(){
	glAttachShader(m_programID, m_vertShaderID);
	glAttachShader(m_programID, m_fragShaderID);

	glLinkProgram(m_programID);

	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);
		glDeleteProgram(m_programID);
		glDeleteShader(m_vertShaderID);
		glDeleteShader(m_fragShaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shaders failed to link!");
		return;
	}
	glDetachShader(m_programID, m_vertShaderID);
	glDetachShader(m_programID, m_fragShaderID);
	glDeleteShader(m_vertShaderID);
	glDeleteShader(m_fragShaderID);
}

void GLSLProgram::addAttribute(const std::string& attribName){
	glBindAttribLocation(m_programID, m_numAttributes, attribName.c_str());
       	m_numAttributes++;	
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName){
	GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
	if (location == (GLint)GL_INVALID_INDEX){
		fatalError("Uniform "+uniformName+" not found in shader!");
	}
	return location;
}

void GLSLProgram::use(){
	glUseProgram(m_programID);
	for (unsigned int i = 0; i < m_numAttributes; i++){
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse(){
	glUseProgram(0);
	for (unsigned int i = 0; i < m_numAttributes; i++){
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::compileShader(const std::string& filepath, GLuint id){
	std::ifstream myFile(filepath);
	if (myFile.fail()){
		perror(filepath.c_str());
		fatalError("Failed to open "+ filepath);
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(myFile, line)){
		fileContents += line + "\n";
	}

	myFile.close();
	const char* contentptr = fileContents.c_str();
	glShaderSource(id, 1, &contentptr, nullptr);
	glCompileShader(id);
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(id);
		std::printf("%s\n", &(errorLog[0]));
		fatalError("failed to compile shader in file: "+filepath);
		return;
	}	
	
}	


