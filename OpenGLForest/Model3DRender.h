#pragma once


/*  To keep the Model3D class agnostic of the graphics and rendering details (i.e OpenGL details here),
*   the rendering work is extracted into this class...
*/

class Model3D;
class WorldObject3D;


class Model3DRender
{
	public:
		bool render(Model3D* model);
		bool render(WorldObject3D* object3D);



};

