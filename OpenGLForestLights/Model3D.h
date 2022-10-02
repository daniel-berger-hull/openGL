#pragma once

#include <string>
#include <vector>

#include "Constant3D.h"


#define NOT_DEFINED   -1
#define TRIANGLE       3
#define QUAD           4
#define	PENTAGON       5
#define	HEXAGON        6

#define COLOR_FACES    1
#define TEXTURE_FACES  2


using namespace std;


typedef struct RGB {
	float red, green, blue;
};

// A face should reference index in an array of Vertex, Normal, Color or TextCoords
//   and not contains the full values, as if there is a repetition of Vertex (and others)
//  this is really not efficient!
typedef struct Face3D {
	long id;
	int type;
	Point3D  normal;
	RGB      color;
	int      vertex[4];        // This could be a bit more flexible! A point on an int array instead
	int      textCoords[4];	   // Here again, a index instead of something better
};

typedef struct MaterialInfo {
	string key;
	RGB  ambiant;
	RGB  diffuse;
	RGB  specular;
	string textureFile;
};


class Model3D
{
	public:	
		vector<Point3D>*      getVertexList()         { return &vertexes;      }
		vector<Point3D>*      getNormalList()         { return &normals;       }
		vector<Point2D>*      getTextureCoordList()   { return &textureCoords; }
		vector<Face3D>*		  getFaceList()           { return &faces;         }
		vector<MaterialInfo>* getMaterialList()       { return &materials;     }

		int  getModelColorType()				      { return modelColorType; }
		void setModelColorType(int type)		      { modelColorType = type; }   //More robustness required here!

	private:
		int modelColorType = COLOR_FACES;		//Default to color

		vector<Point3D>		  vertexes;
		vector<Point3D>		  normals;
		vector<Point2D>		  textureCoords;
		vector<Face3D>		  faces;
		vector<MaterialInfo>  materials;
};

