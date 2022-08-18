#pragma once

#include <string>
#include <regex>

#define OK					         0
#define ERROR						-1
#define NORMAL_VALUE_NOT_FOUND      -1


#define REGEX_PATTERNS_COUNT    12

#define EMPTY_SPACE             ' '

#define VERTEX_NBR_COMPONENTS    3
#define NORMAL_NBR_COMPONENTS    3
#define COLOR_NBR_COMPONENTS     3


#define X_TOKEN_POS				 0
#define Y_TOKEN_POS				 1
#define Z_TOKEN_POS              2


#define RED_TOKEN_POS			 0
#define GREEN_TOKEN_POS			 1
#define BLUE_TOKEN_POS           2


#define NOT_DEFINED   -1
#define TRIANGLE       3
#define QUAD           4
#define	PENTAGON       5
#define	HEXAGON        6



using namespace std;


typedef struct Point3D {
	float x, y, z;
};

typedef struct RGB {
	float red,green, blue;
};


typedef struct Face3D {
	long id;
	int type;
	Point3D  normal;
	//Point3D  color;
	RGB  color;
	int  vertex[4];
	//	int      vertex[6];
};

typedef struct MaterialInfo {
	string key;
	RGB  ambiant;
	RGB  diffuse;
	RGB  specular;
};



void init();
int  processLine(const int lineCount, string nextLine);
int  loadFile(string objFileName, string materialFileName);
int  loadObjFile(string objFileName);
int  loadMaterialFile(string materialFileName);
bool copy3DModel(vector<Point3D>*& destiVertexes,
				 vector<Point3D>*& destiNormals,
				 vector<Face3D>*& destiFaces);

inline void removeStringType(string* nextLine, string charsToRemove);
int  getNormalFromToken(string token);

bool parseVertexString(string nextLine);
bool parseNormalString(string nextLine);
bool parseFaceString(string nextLine);
bool parseTextureCoordsString(string nextLine);
bool parseEmptyString(string nextLine);
bool parseCommentString(string nextLine);
bool parseMaterialTemplateFileString(string nextLine);
bool parseSelectMaterialString(string nextLine);

bool parseNewMaterialString(string nextLine);
bool parseAmbiantColorString(string nextLine);
bool parseDiffuseColorString(string nextLine);
bool parseSpecularColorString(string nextLine);




enum
{
	VERTEX_LINE,
	NORMAL_LINE,
	FACE_LINE,
	TEXTURE_COORDS_LINE,
	EMPTY_LINE,
	COMMENT_LINE,
	MATERIAL_TEMPLATE_FILE_LINE,
	SELECT_MATERIAL_LINE,
	NEW_MATERIAL_LINE,
	AMBIANT_COLOR_LINE,
	DIFFUSE_COLOR_LINE,
	SPECULAR_COLOR_LINE
};




const regex SPACE_REGEX(" ");


