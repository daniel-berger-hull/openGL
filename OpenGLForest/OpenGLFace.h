#pragma once
#include <vector>

#include "Model3D.h"

class OpenGLFace
{
	public:
		bool colorRender(Face3D face, vector<Point3D>* vertexes);
		bool colorRender(Face3D face, vector<Point3D>* vertexes, float factor);

		bool textureRender(Face3D face, vector<Point3D>* vertexes, vector<Point2D>* textCoords);
		bool textureRender(Face3D face, vector<Point3D>* vertexes, vector<Point2D>* textCoords, float factor);


};

