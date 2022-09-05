#pragma once

/***************************************************************************
   This class represent a specifi instance of an object in the 3D world,
   like the player, enemies, surrounding object of the backgroupd
   Aside having it specific location and current state (what is currently happening),
   It takes is visual representition from a Model3D class...
*/

#include <assert.h>

#include "Constant3D.h"
#include "Model3D.h"

class WorldObject3D
{
    public: 
		WorldObject3D();
		WorldObject3D(Point3D pos);

    	Point3D               getPosition() { return position; }
		Point3D               getRotation() { return rotation; }

		void                  setPosition(Point3D pos);
		void                  setRotation(Point3D rot);

		float				  getFactor()		 { return factor; }
		void				  setFactor(float f) { assert(f > 0.0f); factor = f; }

		Model3D*			  getModel()		   { return model; }
		void                  setModel(Model3D* m) { assert(m != NULL); model = m; }

    private: 
    	Point3D position;
		Point3D rotation;

		Model3D* model;

		float factor = 1.0f;

};

