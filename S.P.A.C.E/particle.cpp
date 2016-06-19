#include "particle.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "glm\glm.hpp"
#include "GL/glew.h"



//vectors for position/velocity
	glm::vec3 position;
	glm::vec3 velocity;

	

	
//Constructor
//Initialises the positions, velocity, lifespan, rotation and colour of the particle
	Particle::Particle() {

		//Starting positions x,y,z
		//position y always starts the same and particles will spawn randomly from 0.0 to 0.2f from the centre.
		posX = ((float)rand() / (float)(RAND_MAX)) / 5;
		posY = 0.0f;
		posZ = ((float)rand() / (float)(RAND_MAX)) / 5;

		//Starting velocities x,y,z
		//Velocity of Y always positive so late can act like gravity
		//Velocity of X and Z randomly generate as negative or positive so they can travel in different directions
		velY = ((float)rand() / (float)(RAND_MAX) / 5) + 0.7;
		velX = (((float)rand() / (float)(RAND_MAX)) / 3) - 0.165;
		velZ = (((float)rand() / (float)(RAND_MAX)) / 2) - 0.25;

        //lifespan at object. starts high so it can decrease later
		//lifespan = ((float)rand() / (float)(RAND_MAX)* 2) * 3;
		lifespan = ((float)rand() / (float)(RAND_MAX) / 5) + 0.7f;

		//rotations. Allows randomly rotations in either direction
		rotx = ((float)rand() / (float)(RAND_MAX)) - 0.5;
		roty = ((float)rand() / (float)(RAND_MAX)) - 0.5;
		rotz = ((float)rand() / (float)(RAND_MAX)) - 0.5;

		//colour variable which randomly selects a number between 0 and 2, so particles can have different colours. 
		colour = (int)(rand() % 3);
		if (colour == 0) {
			colourR = 1.0f;
			colourG = 0.0f;
			colourB = 0.0f;
		}
		else if (colour == 1){
			colourR = 0.0f;
			colourG = 1.0f;
			colourB = 0.0f;
	 }
	  else if(colour ==2){
		  colourR = 0.0f;
		  colourG = 0.0f;
		  colourB = 1.0f;
	}
	  }

	//Draw method. Draws 2D Triangle Objects for each particle
	void Particle::Draw() {
		
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		glTexCoord2f(1, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(0, 0);
	    glVertex3f(1, 0, 0);
		glTexCoord2f(1, 1);
		glVertex3f(0, 1, 0);
		glEnd();
		}

	//Update Method that updates constantly.
	void Particle:: Update() {
		//Rotations of particles. RotY doesn't turn as much
		rotx += (float)rand() / (float)(RAND_MAX)/2 - 0.25;
		roty += (float)rand() / (float)(RAND_MAX)/8 - 0.0625;
		rotz += (float)rand() / (float)(RAND_MAX)/2 - 0.25;

		//posX and posZ move out over time
		posX += velX * 0.1;
		posZ += velZ * 0.1;
		//if (testY < 4.0f && testY >= 0.0f) {
		   
		//Decrease velocity over time so that velocity will fall 0 and will act like gravity as it starts to move down again
			velY -= 0.008;
			posY += velY * 0.1;
		//}
		
			//Decrease lifespan over time and reset when it reaches 0
		lifespan -= 0.006;
		if (lifespan <= 0) {
			Reset();
		}
			

	}

	//Reset Method. Resets values back
	void Particle::Reset() {
		
		posX = ((float)rand() / (float)(RAND_MAX)) / 5;	
		posY = 0.0f;
		posZ = (float)rand() / (float)(RAND_MAX) / 5;

		
		velY = ((float)rand() / (float)(RAND_MAX) / 5) + 0.7;		
		velX = (((float)rand() / (float)(RAND_MAX)) / 3) - 0.165;
		velZ = (((float)rand() / (float)(RAND_MAX)) / 2) - 0.25;


		lifespan = ((float)rand() / (float)(RAND_MAX) / 5) + 0.7f;


		rotx = ((float)rand() / (float)(RAND_MAX)) - 0.5;
		roty = ((float)rand() / (float)(RAND_MAX)) - 0.5;
		rotz = ((float)rand() / (float)(RAND_MAX)) - 0.5;
		
	}
	glm::vec3& Particle::Position() {
		return position;
	}

	float Particle::PositionY() {
		return posY;
	}
	

	float Particle::PositionX() {
		return posX;
	}
	
	float Particle::PositionZ() {
		return posZ;
	}
	
	
	float Particle::RotationX() {
		return rotx;
	}
	float Particle::RotationY() {
		return roty;
	}
	float Particle::RotationZ() {
		return rotz;
	}
	float Particle::ColourR() {
		return colourR;
	}
	float Particle::ColourG() {
		return colourG;
	}
	float Particle::ColourB() {
		return colourB;
	}
	int Particle::Colour() {
		return colour;
	}
	float Particle::Lifespan() {
		return lifespan;
	}

	/**void Particle::createParticle() {


	//position.x = rand() % 10;

	// position.y = 1.5f;
	//position.z = rand() % 1;
	//pos.y = 1.5f;
	//position.x = 0 + i;
	//position.x = 2;
	//position.x = i / 100;
	//for (int i = 0; i < sizeof(p1); i++) {
	//p1[i].Position().x = (float)rand() / (float)(RAND_MAX)* 2;
	testX = ((float)rand() / (float)(RAND_MAX)* 2);
	testY =  1.5f;
	testZ = (0.0f);
	//testZ = (float)rand() / (float)(RAND_MAX)* 2;
	//xpos = 0.0f;
	// ypos = 1.5f;
	//zpos = 0.0f;
	//p1[i].Position().y = 1.5f;
	velocity.y = 1;
	;
	}
	*/