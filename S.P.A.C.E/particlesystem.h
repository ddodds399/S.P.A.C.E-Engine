#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "particle.h"
class particlesystem :public Particle
{
	
public:
	Particle particles[50];
	particlesystem();
	particlesystem(int size);
	void Draw(int i);
	~particlesystem();
	void Update(float count);
	int Size();
	Particle getArray(int test);
private: 
	//Particle particles[];
	
};

#endif

