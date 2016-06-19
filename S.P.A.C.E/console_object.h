#ifndef CONSOLE_OBJECT_H
#define CONSOLE_OBJECT_H

#include "transform.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "time.h"
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>

using namespace std;

//Console Object
class ConsoleObject
{
public:
	/*
	Constructor 
	initialising meshes, textures and shaders used to draw 
	the console object. 
	*/
	ConsoleObject() {
		transformC.Reset();
		transformS.Reset();
		stand.SetMesh("./res/Models/console.obj");
		screen.SetMesh("./res/Models/consolescreen.obj");
		consoleTex.setTexture("./res/textures/console.jpg");
		screenTex.setTexture("./res/textures/screen.jpg");

		shader2D.SetShader("./res/consoleShader");
		basicShader.SetShader("./res/basicShader");

	}
	/*
	Draw method that draws both meshes that comprise the console object, 
	binding shaders before drawing the meshes
	*/
	void Draw(glm::vec3 lightPos, Camera camera) {
		glDisable(GL_CULL_FACE);
		shader2D.Bind();
		GLint objectColorLoc2 = glGetUniformLocation(shader2D.GetProg(), "objectColor");
		GLint lightColorLoc2 = glGetUniformLocation(shader2D.GetProg(), "lightColor");
		GLint lightPosLoc2 = glGetUniformLocation(shader2D.GetProg(), "lightPos");
		GLint viewPosLoc2 = glGetUniformLocation(shader2D.GetProg(), "viewPos");
		glUniform3f(objectColorLoc2, 0.184f, 0.310f, 0.310f);
		glUniform3f(lightColorLoc2, 0.1f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc2, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc2, camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);
		// Get the uniform locations
		GLint modelLoc2 = glGetUniformLocation(shader2D.GetProg(), "model");
		GLint viewLoc2 = glGetUniformLocation(shader2D.GetProg(), "view");
		GLint projLoc2 = glGetUniformLocation(shader2D.GetProg(), "projection");
		// Pass the matrices to the shader
		glm::mat4 model2;
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection()));
		glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(glm::perspective(70.0f, (float)800 / (float)600, 0.1f, 100.0f)));
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
		glUniform1i(glGetUniformLocation(shader2D.GetProg(), "cubeMap"), 0);
		shader2D.Update(transformC, camera);
		consoleTex.Bind();
		stand.Draw();
		basicShader.Bind();
		screenTex.Bind();
		//Applying the transformation to rotate the screen mesh every update
		transformS.GetRot().y -= 0.00075;
		basicShader.Update(transformS, camera);
		screen.Draw();
	}
	//Gets transformation matrix of each mesh
	inline Transform &GetTransformObjectC() { return transformC; }
	inline Transform &GetTransformObjectS() { return transformS; }

	//Sets x position
	void SetPosx(float x) {
		transformC.GetPos().x = x;
		transformS.GetPos().x = x;
	}
	//Sets y position
	void SetPosy(float y) {
		transformC.GetPos().y = y;
		transformS.GetPos().y = y;
	}
	//Sets z position
	void SetPosz(float z) {
		transformC.GetPos().z = z;
		transformS.GetPos().z = z;
	}
protected:
private:
	ConsoleObject(const ConsoleObject& other) {}
	void operator=(const ConsoleObject& other) {}
	Mesh stand, screen;
	Transform transformC, transformS;
	Texture consoleTex, screenTex;
	Shader  shader2D, basicShader;
};

#endif //CONSOLE_OBJECT