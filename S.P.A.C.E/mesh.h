//Based on Youtube tutorial from user TheBennyBox
//Playlist - https://www.youtube.com/playlist?list=PLEETnX-uPtBXT9T-hD0Bj31DSnwio-ywh
//Github - https://github.com/BennyQBD/ModernOpenGLTutorial

#ifndef MESH_H
#define MESH_H

#include <GL\glew.h>
#include "vertex.h"
#include "obj_loader.h"

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int * indices, unsigned int numIndices);
	Mesh(const std::string& fileName);
	Mesh(const Mesh& other) {}
	void operator=(const Mesh& other) {}
	void Draw();
	void SetMesh(const std::string& fileName);
	virtual ~Mesh();
protected:
private:
	


	void InitMesh(const IndexedModel& model);

	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
};
#endif // MESH_H