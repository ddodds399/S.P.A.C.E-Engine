#ifndef OBJECTBOUNDS_H
#define OBJECTBOUNDS_H

class ObjectBounds {
public: void SphereBound(float p1x, float p1y, float p1z, float p1r);
		bool SphereBoundCheck(float p1x, float p1y, float p1z, float p1r);
		bool DoorBoundCheck(float p1x, float p1y, float p1z, float p1r);
		void SphereSimpleResolution(float xDis, float yDis, float zDis, float radD);
		void SetBounds(float p1x, float p1y, float p1z, float p1r);
		float GetCentreX();
		float GetCentreY();
		float GetCentreZ();
		float GetRadius();

};

#endif // OBJECTBOUNDS_H
