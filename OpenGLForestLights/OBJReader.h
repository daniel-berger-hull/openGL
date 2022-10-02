#pragma once

#include <string>
#include <regex>

#include "Model3D.h"


#define OK									 0
#define ERROR								-1
#define TEXTURE_COORDS_VALUE_NOT_FOUND      -2
#define NORMAL_VALUE_NOT_FOUND				-3


#define REGEX_PATTERNS_COUNT				 13

#define EMPTY_SPACE							' '

#define VERTEX_NBR_COMPONENTS				3
#define NORMAL_NBR_COMPONENTS				3
#define COLOR_NBR_COMPONENTS				3
#define TEXTURE_COORDS_NBR_COMPONENTS		2


#define X_TOKEN_POS					0
#define Y_TOKEN_POS					1
#define Z_TOKEN_POS					2


#define RED_TOKEN_POS				0
#define GREEN_TOKEN_POS				1
#define BLUE_TOKEN_POS				2

#define	TEXTURE_FILE_NBR_COMPONENTS 1



using namespace std;



void initParsingVars();
void cleanUp();
int  processLine(const int lineCount, string nextLine);
int  loadFile(string objFileName, string materialFileName);
int  loadObjFile(string objFileName);
int  loadMaterialFile(string materialFileName);

// Aug 30: This LoadBMP method should be in the OBJReader and not in the OpenGLCanvas!!!
//         For some reason, when transfer here, the compilation fails
//         as it doesn't seem to recognize the Windows Type like  byte And DWORD
//AUX_RGBImageRec* LoadBMP(char* Filename);    // New

bool copy3DModel(vector<Point3D>*& destiVertexes,
				 vector<Point3D>*& destiNormals,
				 vector<Face3D>*& destiFaces);

//bool copy3DModel(vector<Point3D>*& destiVertexes,
//				vector<Point3D>*& destiNormals,
//				vector<Point2D>*& destiTextCoords,
//				vector<Face3D>*& destiFaces);
bool copy3DModel(vector<Point3D>*&       destiVertexes,
				 vector<Point3D>*&       destiNormals,
				 vector<Point2D>*&       destiTextCoords,
				 vector<Face3D>*&        destiFaces,
				 vector<MaterialInfo>*&  destiMaterials);


bool copy3DModel(Model3D* destiModel);



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
bool parseTextureFileString(string nextLine);



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
	SPECULAR_COLOR_LINE,
	TEXTURE_FILE_LINE
};


const regex SPACE_REGEX(" ");


