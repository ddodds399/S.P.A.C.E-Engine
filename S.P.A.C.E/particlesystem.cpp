#include <iostream>
#include "particlesystem.h"
#include "particle.h"
using namespace std;

 Particle particles[50];
 int arraySize;
 float count;

 particlesystem::particlesystem() {

 }
 //Particle Systen constructor originally made to create and hold an array of particles
 particlesystem::particlesystem(int size)
	{
		arraySize = size;
		//particles[50];
		//particles[size];
		//particles[1].;
		for (int i = 0; i < 50; i++) {
			//particles[i].createParticle(particles, i);
			//particles[i].createParticle();
		}
			//particles[i].Position().z = (float)rand() / (float)(RAND_MAX)* 2;
			//particles[i].Position().x = (float)rand() / (float)(RAND_MAX)* 2;
	//	}
		//}
}


void particlesystem::Draw(int i) {
	
		particles[i].Draw();
	
}
void particlesystem::Update(float count) {
	for (int i = 0; i < arraySize; i++) {
		//.current
		//particles[i].pos.y += count;
		//if (particles[i].Position().y < 4.0f) {
			//particles[i].Position().z = ;
	//	if (particles[i].Position().y < 30.0f) {
			//particles[i].Position().y = i + 1.5f + (count * 20);
		//particles[i].SetPositionY(0.0003);
		//}
		////else {
			//particles[i].Position().y = 0;
		//}//+ ((float)rand() / (float)(RAND_MAX)* 2);
			//particles[i].Position().x = (float)rand() / (float)(RAND_MAX) * 2;
		//}
		//else{
		//	particles[i].Position().y = 1.5f;
	//}
		//particles[i].test
		//particles[i].Update(count);
		//particles[i].SetVelocityY(particles[i].VelocityY() - (5 * count));
		//particles[i].SetPositionY(count*50); //+ ( count));
		//particles[i].SetVelocityY(particles[i].VelocityY() - count/1000);
		//particles[i].PositionY += count;
		//particles[i].SetPositionX(((double)rand() / (RAND_MAX)) + 1);
		//particles[i].SetPositionY(particles[i].PositionY() - 1);
		//particles[i].SetPositionY(1.5* (count*60));
		//cout<< "test"particles[50].PositionY() << end1;
		
		//particles[i].SetPositionY(2 + (sinf(count * 30) * 2));
		//particles[i].SetPositionZ(((double)rand() / (RAND_MAX)) + 1);
	}
}

int particlesystem::Size() {
	return arraySize;
}
Particle particlesystem::getArray(int test) {
	return particles[test];
}

particlesystem::~particlesystem()
{
}


