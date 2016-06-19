#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm\glm.hpp>


class Particle
{

private: 
	float posX;
	float posY;
	float posZ;
	float lifespan;
	float velX;
	float velY;
	float velZ;
	float rotx, roty, rotz;
	int colour;
	float colourR, colourG, colourB;
	

public:
	Particle();
	glm::vec3 pos;
	void createParticle();
	void Draw();
	void Update();
	void Reset();
	glm::vec3& Particle::Position();
	float PositionY();
	float PositionX();
	float PositionZ();
	float VelocityX();
	float VelocityY();
	float VelocityZ();
	float RotationX();
	float RotationY();
	float RotationZ();
	float ColourR();
	float ColourG();
	float ColourB();
	float Lifespan();
	int Colour();
	void SetPositionY(float posY);
	void SetPositionX(float posX);
	void SetPositionZ(float posZ);
	void SetVelocityX(float velX);
	void SetVelocityY(float velY);
	void SetVelocityZ(float velZ);
protected:
	glm::vec3 test;
	
};

#endif // SKYBOX_H

