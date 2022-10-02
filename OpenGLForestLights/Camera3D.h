#pragma once

#include <assert.h>     /* assert */

#include "Constant3D.h"

class Camera3D
{
public:
	Camera3D();
	Camera3D(Point3D l, Point3D r);

	Point3D  getLocation() { return location; }
	Point3D  getRotation() { return rotation; }

	void  setLocation(Point3D  l)   {  location = l; }
	void  setRotation(Point3D r)    {  rotation = r; }

	void moveLocation(Point3D  l)   {  location.x += l.x;  location.y += l.y; location.z += l.z; }
	void changeLocation(Point3D  r) {  rotation.x += r.x;  rotation.y += r.y; rotation.z += r.z; }

	
		
	private: 	
		Point3D  location;
		Point3D  rotation;
		
};

