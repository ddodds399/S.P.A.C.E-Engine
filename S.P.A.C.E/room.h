#ifndef ROOM_H
#define ROOM_H

#include "transform.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "time.h"
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "objectBounds.h"

using namespace std;

//Room Object
class Room
{
public:
	//Creates a room with a mesh based on the filepath
	Room(const GLchar* meshpath) {
		transformW, transformD;
		room.SetMesh(meshpath);
		door.SetMesh("./res/Models/door.obj");
		transformD.GetPos().z = 0.0f;
		doorOpen = false;
		wallTex.setTexture("./res/textures/roomtexture.jpg");
		doorTex.setTexture("./res/textures/door.jpg");
		shader2D.SetShader("./res/shader2D");

	}
	//Calls mesh draw method
	void Draw(glm::vec3 lightPos, Camera camera) {
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glDisable(GL_CULL_FACE);
		shader2D.Bind();
		GLint objectColorLoc = glGetUniformLocation(shader2D.GetProg(), "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shader2D.GetProg(), "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shader2D.GetProg(), "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shader2D.GetProg(), "viewPos");
		GLint tintLoc = glGetUniformLocation(shader2D.GetProg(), "tint");
		glUniform3f(objectColorLoc, 0.184, 0.310, 0.310);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 0.7f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);
		glUniform4f(tintLoc, 0.0f, 0.0f, 0.0f, 0.0f); // Apply room tint
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(shader2D.GetProg(), "model");
		GLint viewLoc = glGetUniformLocation(shader2D.GetProg(), "view");
		GLint projLoc = glGetUniformLocation(shader2D.GetProg(), "projection");
		// Pass the matrices to the shader
		glm::mat4 model;
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(glm::perspective(70.0f, (float)800 / (float)600, 0.1f, 100.0f)));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(shader2D.GetProg(), "cubeMap"), 0);
		wallTex.Bind();
		shader2D.Update(transformW, camera);
		room.Draw();
		shader2D.Update(transformD, camera);
		doorTex.Bind();
		door.Draw();
	
	}
	//Called when a room needs to be tinted green in the mapview, this represents the current position of the player.
	void Draw(glm::vec3 lightPos, Camera camera, int index){
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glDisable(GL_CULL_FACE);
		shader2D.Bind();
		GLint objectColorLoc = glGetUniformLocation(shader2D.GetProg(), "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shader2D.GetProg(), "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shader2D.GetProg(), "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shader2D.GetProg(), "viewPos");
		GLint tintLoc = glGetUniformLocation(shader2D.GetProg(), "tint");
		glUniform3f(objectColorLoc, 0.184, 0.310, 0.310);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 0.7f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);
		glUniform4f(tintLoc, 0.0f, 1.0f, 0.0f, 0.0f); // Apply room tint
													  // Get the uniform locations
		GLint modelLoc = glGetUniformLocation(shader2D.GetProg(), "model");
		GLint viewLoc = glGetUniformLocation(shader2D.GetProg(), "view");
		GLint projLoc = glGetUniformLocation(shader2D.GetProg(), "projection");
		// Pass the matrices to the shader
		glm::mat4 model;
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(glm::perspective(70.0f, (float)800 / (float)600, 0.1f, 100.0f)));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(shader2D.GetProg(), "cubeMap"), 0);
		wallTex.Bind();
		shader2D.Update(transformW, camera);
		room.Draw();

		shader2D.Update(transformD, camera);
		doorTex.Bind();
		door.Draw();

	}
	void moveDoor(float movement, char orientation, float speed)
	{
			switch (orientation)
			{
			case 'f':
				transformD.GetPos().x = movement * speed;
				break;
			case 'l':
				transformD.GetPos().z = -movement * speed;
				break;
			case 'r':
				transformD.GetPos().z = movement * speed;
				break;
			}

	}

	//Gets transformation matrix of each mesh
	inline Transform &GetTransformMatWall() { return transformW; }
	inline Transform &GetTransformMatDoor() { return transformD; }
	//Gets size variable of room
	inline bool &GetDoor() { return doorOpen; }
	//Sets global position in world co-ordinates
	void SetPos(float x, float y, float z) {
		transformW.SetPos(glm::vec3(x, y, z));
		transformD.SetPos(glm::vec3(x, y, z));
	}
	void SetPos(glm::vec3 position) {
		transformW.SetPos(position);
		transformD.SetPos(position);
	}
	//Sets x position
	void SetPosx(float x) {
		transformW.GetPos().x = x;
		transformD.GetPos().x = x;
	}
	//Sets y position
	void SetPosy(float y) {
		transformW.GetPos().y = y;
		transformD.GetPos().y = y;
	}
	//Sets z position
	void SetPosz(float z) {
		transformW.GetPos().z = z;
		transformD.GetPos().z = z;
	}
	void SetRot(float rot) {
		transformW.SetRot(glm::vec3(0, rot, 0));
		transformD.SetRot(glm::vec3(0, rot, 0));
	}
	void Reset() {
		transformW.ResetRot();
		transformD.ResetRot();
	}
	//Sets x position
	void SetDoorPosx(float x) {
		transformD.GetPos().x = x;
	}
	//Sets y position
	void SetDoorPosy(float y) {
		transformD.GetPos().y = y;
	}
	//Sets z position
	void SetDoorPosz(float z) {
		transformD.GetPos().z = z;
	}
protected:
private:
	Room(const Room& other) {}
	void operator=(const Room& other) {}
	Mesh room, door;
	Transform transformW, transformD;
	bool doorOpen;
	vector<const GLchar*> wfaces;
	Texture wallTex, doorTex;
	Shader shader2D;
};

#endif //ROOM_H