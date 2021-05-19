#include "Shader.h"

Shader::Shader(std::string vertex, std::string fragment) {
	m_program = createProgram(vertex, fragment);
}

Shader::Shader(Shader* shader) {
	m_program = shader->m_program;
}

Shader::~Shader() {
	cleanup();
}


void Shader::loadSampler(const char* location, int sampler){
	glUniform1i(glGetUniformLocation(m_program, location), sampler);
}

//OpenGL specifies matrices as column-major to get row-major just transpose it
void Shader::loadMatrix(const char* location, const Matrix4f matrix, bool trans) {
	glUniformMatrix4fv(glGetUniformLocation(m_program, location), 1, trans, &matrix[0][0]);
}

void Shader::loadVector(const char* location,  Vector3f vector){
	glUniform3fv(glGetUniformLocation(m_program, location), 1, &vector[0]);
}

void Shader::loadVector(const char* location, Vector2f vector) {
	glUniform2fv(glGetUniformLocation(m_program, location), 1, &vector[0]);
}

void Shader::loadFloat(const char* location, float value) {
	glUniform1f(glGetUniformLocation(m_program, location), value);
}

void Shader::loadFloat(const char* location, float value[2]){
	glUniform1fv(glGetUniformLocation(m_program, location), 2, value);
}

void Shader::loadBool(const char* location, bool value){
	glUniform1i(glGetUniformLocation(m_program, location), value);
}

GLuint Shader::createProgram(std::string vertex, std::string fragment) {
	GLuint vshader = loadShaderProgram(GL_VERTEX_SHADER, vertex.c_str());
	GLuint fshader = loadShaderProgram(GL_FRAGMENT_SHADER, fragment.c_str());
	return linkShaders(vshader, fshader);
}

void Shader::readTextFile(const char *pszFilename, std::string &buffer){
	std::ifstream file(pszFilename, std::ios::binary);
	if (file.is_open()){
		file.seekg(0, std::ios::end);
		std::ifstream::pos_type fileSize = file.tellg();
		buffer.resize(static_cast<unsigned int>(fileSize));
		file.seekg(0, std::ios::beg);
		file.read(&buffer[0], fileSize);
	}
}

GLuint Shader::loadShaderProgram(GLenum type, const char *pszFilename){

	GLuint shader = 0;
	std::string buffer;
	readTextFile(pszFilename, buffer);

	if (buffer.length() > 0){
		shader = compileShader(type, reinterpret_cast<const char *>(&buffer[0]));
	}
	return shader;
}

GLuint Shader::compileShader(GLenum type, const char *pszSource){

	GLuint shader = glCreateShader(type);

	if (shader){
		GLint compiled = 0;

		glShaderSource(shader, 1, &pszSource, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled){

			GLsizei infoLogSize = 0;
			std::string infoLog;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);
			infoLog.resize(infoLogSize);
			glGetShaderInfoLog(shader, infoLogSize, &infoLogSize, &infoLog[0]);
			std::cout << "Compile status: \n" << &infoLog << std::endl;
		}
	}
	return shader;
}

GLuint Shader::linkShaders(GLuint vertShader, GLuint fragShader){

	GLuint program = glCreateProgram();

	if (program){
		GLint linked = 0;

		if (vertShader)
			glAttachShader(program, vertShader);

		if (fragShader)
			glAttachShader(program, fragShader);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (!linked){
			GLsizei infoLogSize = 0;
			std::string infoLog;

			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);
			infoLog.resize(infoLogSize);
			glGetShaderInfoLog(program, infoLogSize, &infoLogSize, &infoLog[0]);
			std::cout << "Compile status: \n" << &infoLog << std::endl;
		}

		// Mark the two attached shaders for deletion. These two shaders aren't
		// deleted right now because both are already attached to a shader
		// program. When the shader program is deleted these two shaders will
		// be automatically detached and deleted.
		if (vertShader)
			glDeleteShader(vertShader);

		if (fragShader)
			glDeleteShader(fragShader);

	}
	return program;
}

void Shader::cleanup(){
	if (m_program){
		glDeleteProgram(m_program);
		m_program = 0;
	}
}