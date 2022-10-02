#pragma once

#include "Constant3D.h"

class Light3D
{

public:
	Light3D() : location({0.0f, 0.0f, 0.0f})  {};
	Light3D(Point3D loc) :location(loc) {};

	Point3D  getLocation() { return location; }
	void     setLocation(Point3D  l) { location = l; }
	
	void moveLocation(Point3D  l) { location.x += l.x;  location.y += l.y; location.z += l.z; }

	float getAngle() { return angle; }
	void setAngle(float a) { angle = a; }

private:
	Point3D  location;
	float    angle;   // Angle may be removed, as it was only for a test...
};

