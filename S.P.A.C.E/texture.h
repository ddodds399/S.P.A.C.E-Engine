//Based on Youtube tutorial from user TheBennyBox
//Playlist - https://www.youtube.com/playlist?list=PLEETnX-uPtBXT9T-hD0Bj31DSnwio-ywh
//Github - https://github.com/BennyQBD/ModernOpenGLTutorial

#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);
	Texture(std::vector<const GLchar*> faces);
	void setTexture(const std::string& fileName);
	void setTexture(std::vector<const GLchar*> faces);
	void Bind();

	virtual ~Texture();
protected:
private:
	Texture(const Texture& other) {}
	void operator=(const Texture& other) {}

	GLuint m_texture;
	bool isCubemap = false;
};

#endif // TEXTURE_H