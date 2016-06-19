#include "AABBbounds.h"
#include <iostream>
#include <GL\glew.h>

float minimumX, minimumY, minimumZ, maximumX, maximumY, maximumZ;

void AABBbounds::AABBBound(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	minimumX = minX;
	minimumY = minY;
	minimumZ = minZ;
	maximumX = maxX;
	maximumY = maxY;
	maximumZ = maxZ;

}
bool AABBbounds::AABBBoundCheck(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	if (minimumX > maxX || maximumX < minX) {
		return false;
		if (minimumY > maxY || maximumY < minY) {
			return false;
			if (minimumZ > maxZ || maximumZ < minZ)
			{
				return false;
			}
		}
	}
	
	//printf("collision Detected\n");
	return true;

}

float AABBbounds::GetMinimumX()
{

	return minimumX;
}
float AABBbounds::GetMinimumY()
{

	return minimumY;
}
float AABBbounds::GetMinimumZ()
{

	return minimumZ;
}
float AABBbounds::GetMaximumX()
{

	return maximumX;
}
float AABBbounds::GetMaximumY()
{

	return maximumY;
}
float AABBbounds::GetMaximumZ()
{

	return maximumZ;
}
