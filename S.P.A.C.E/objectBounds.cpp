#include "objectBounds.h"
#include <iostream>
#include <GL\glew.h>

GLfloat centreX, centreY, centreZ, radius;

void ObjectBounds::SphereBound(GLfloat p1x, GLfloat p1y, GLfloat p1z, GLfloat p1r)
{
	radius = p1r;
	centreX = p1x;
	centreY = p1y;
	centreZ = p1z;
}

bool ObjectBounds::SphereBoundCheck(GLfloat p1x, GLfloat p1y, GLfloat p1z, GLfloat p1r)
{
	float radD = radius + p1r;
	float xDis = centreX - p1x;
	float yDis = centreY - p1y;
	float zDis = centreZ - p1z;

	float xDis2 = xDis * xDis;
	float yDis2 = yDis * yDis;
	float zDis2 = zDis * zDis;
	float radD2 = radD * radD;

	if ((xDis2 <= radD2) && (yDis2 <= radD2) && (zDis2 <= radD2)) {
		//printf("Collision detected\n");
		SphereSimpleResolution(xDis, yDis, zDis, radD);
		return true;
	}
	else {
		return false;
	}
}

// Method to check if the given co-ordinates are overlapping with the bound
bool ObjectBounds::DoorBoundCheck(GLfloat p1x, GLfloat p1y, GLfloat p1z, GLfloat p1r)
{
	float radD = radius + p1r;
	float xDis = centreX - p1x;
	float yDis = centreY - p1y;
	float zDis = centreZ - p1z;

	float xDis2 = xDis * xDis;
	float yDis2 = yDis * yDis;
	float zDis2 = zDis * zDis;
	float radD2 = radD * radD;

	if ((xDis2 <= radD2) && (yDis2 <= radD2) && (zDis2 <= radD2)) {
		return true;
	}
	else {
		return false;
	}
}

void ObjectBounds::SetBounds(GLfloat p1x, GLfloat p1y, GLfloat p1z, GLfloat p1r) {
	radius = p1r;
	centreX = p1x;
	centreY = p1y;
	centreZ = p1z;
}

void ObjectBounds::SphereSimpleResolution(float xDis, float yDis, float zDis, float radD)
{
	float xChange = radD - xDis;
	float yChange = radD - yDis;
	float zChange = radD - zDis;

	centreX += xChange;
	centreY += yChange;
	centreZ += zChange;
}

float ObjectBounds::GetCentreX()
{

	return centreX;
}

float ObjectBounds::GetCentreY()
{
	return centreY;
}

float ObjectBounds::GetCentreZ()
{
	return centreZ;
}

float ObjectBounds::GetRadius()
{
	return radius;
}
