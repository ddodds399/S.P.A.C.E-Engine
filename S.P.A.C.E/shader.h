//Based on Youtube tutorial from user TheBennyBox
//Playlist - https://www.youtube.com/playlist?list=PLEETnX-uPtBXT9T-hD0Bj31DSnwio-ywh
//Github - https://github.com/BennyQBD/ModernOpenGLTutorial

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"

class Shader
{
public:
	Shader();
	Shader(const std::string& filename);
	void SetShader(const std::string& filename);
	void Bind();
	void Update(const Transform& transform, const Camera& camera);
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}
	inline GLuint GetProg() const { return m_program; }

	virtual ~Shader();

protected:
private:
	static const unsigned int NUM_SHADERS = 2;


	std::string LoadShader(const std::string& fileName);
	void CheckError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];


};

#endif // SHADER_H