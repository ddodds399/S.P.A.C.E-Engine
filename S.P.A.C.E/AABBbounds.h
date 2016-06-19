#ifndef AABBBOUNDS_H
#define AABBBOUNDS_H

class AABBbounds {
public:	void AABBBound(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		bool AABBBoundCheck(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		float GetMinimumX();
		float GetMinimumY();
		float GetMinimumZ();
		float GetMaximumX();
		float GetMaximumY();
		float GetMaximumZ();
};

#endif