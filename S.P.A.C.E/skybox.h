#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL\glew.h>

class Skybox
{

	static const GLfloat const skyboxVertices[];

public:
	Skybox();
	void Draw(GLuint shader, glm::mat4 view, glm::mat4 proj);
	virtual ~Skybox();
protected:
private:
	Skybox(const Skybox& other) {}
	void operator=(const Skybox& other) {}

	GLuint skyboxVAO, skyboxVBO;
};

#endif // SKYBOX_H
