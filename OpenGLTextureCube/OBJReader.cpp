// OBJReader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>     /* srand, rand */

#include <iostream>
#include <fstream>
#include <regex>

#include <vector>

#include "GeneralModel.h"
#include "OBJReader.h"
#include "WorldObject3D.h"
#include "Camera3D.h"


using namespace std;


/*************************************************************************************
*		To DO:
* 
*      - Move this code into a Class (instead of being a C like program )
*		       Note: This is creating problem, as the pointers to functions appear to be much
*                     harder than regular c functions
*      - Done! Fix the RegEx mtllib --> Not sure why it is not working...
*      - Done! Start the parsing of the line
*      - Done! Add a vector for the vertex, faces, normals, etc 
*************************************************************************************/


regex formatRegex[REGEX_PATTERNS_COUNT];
string chars_to_trim = " \t";


Point3D POINT_ZERO;
Point3D NO_COLOR_YET;
Point3D RED_COLOR;


string materialFileName;

vector<Point3D>       vertexes;
vector<Point3D>       normals;
vector<Point3D>       color;
vector<Point2D>       textCoords;
vector<Face3D>        faces;
vector<MaterialInfo>  materials;
vector<int>			  parsingErrors;

RGB					  currentMaterialColor;


#define NO_MATERIAL_IN_PROGRESS              0
#define NEW_IDENTIFIED_IN_PROGRESS           1
#define AMBIANT_COLOR_ADDED					 2
#define DIFFUSE_COLOR_ADDED					 4
#define SPECULAR_COLOR_ADDED				 8


int marterialLineStatus = NO_MATERIAL_IN_PROGRESS;

// Will use pointer to functions to invoke those parsing 
// functions, to avoid a large switch case in the main loading loop...
bool (*parseFunctions[REGEX_PATTERNS_COUNT])(string);


static long nextAvailFaceID = 0;

void initParsingVars()
{

	formatRegex[VERTEX_LINE]				 = regex("v .*");
	formatRegex[NORMAL_LINE]				 = regex("vn.*");
	formatRegex[FACE_LINE]					 = regex("f.*");
	formatRegex[TEXTURE_COORDS_LINE]		 = regex("vt.*");
	formatRegex[COMMENT_LINE]				 = regex("#.*");
	formatRegex[EMPTY_LINE]				     = regex(" *");
	formatRegex[MATERIAL_TEMPLATE_FILE_LINE] = regex("mtllib.*");  
	formatRegex[SELECT_MATERIAL_LINE]        = regex("usemtl.*");  

	formatRegex[NEW_MATERIAL_LINE]			 = regex("newmtl.*");     
	formatRegex[AMBIANT_COLOR_LINE]			 = regex("Ka.*");
	formatRegex[DIFFUSE_COLOR_LINE]			 = regex("Kd.*");
	formatRegex[SPECULAR_COLOR_LINE]		 = regex("Ks.*");
	formatRegex[TEXTURE_FILE_LINE]			 = regex("map_Kd.*");


	parseFunctions[VERTEX_LINE]					= &parseVertexString;
	parseFunctions[NORMAL_LINE]					= &parseNormalString;
	parseFunctions[FACE_LINE]					= &parseFaceString;
	parseFunctions[TEXTURE_COORDS_LINE]			= &parseTextureCoordsString;
	parseFunctions[EMPTY_LINE]					= &parseEmptyString;
	parseFunctions[COMMENT_LINE]				= &parseCommentString;
	parseFunctions[MATERIAL_TEMPLATE_FILE_LINE] = &parseMaterialTemplateFileString;
	parseFunctions[SELECT_MATERIAL_LINE]		= &parseSelectMaterialString;

	parseFunctions[NEW_MATERIAL_LINE]           = &parseNewMaterialString;
	parseFunctions[AMBIANT_COLOR_LINE]          = &parseAmbiantColorString;
	parseFunctions[DIFFUSE_COLOR_LINE]          = &parseDiffuseColorString;
	parseFunctions[SPECULAR_COLOR_LINE]         = &parseSpecularColorString;
	parseFunctions[TEXTURE_FILE_LINE]			= &parseTextureFileString;

	POINT_ZERO.x   = 0.0f;    POINT_ZERO.y   = 0.0f; POINT_ZERO.z   = 0.0f;
	NO_COLOR_YET.x = 0.0f;    NO_COLOR_YET.y = 0.0f; NO_COLOR_YET.z = 0.0f;
	RED_COLOR.x    = 255.0f;  RED_COLOR.y    = 0.0f; RED_COLOR.z    = 0.0f;

	materialFileName = " ";

	currentMaterialColor.red   = 55.0f;
	currentMaterialColor.green = 55.0f;
	currentMaterialColor.blue  = 55.0f;

}


///////////////////////////////////////////////////////////////////////////////////////
//  Utility functions on String...
vector<string> tokenize(const string str, const regex re)
{
	sregex_token_iterator it{ str.begin(),  str.end(), re, -1 };
	vector<string> tokenized{ it, {} };

	// Additional check to remove empty strings
	tokenized.erase(
		 remove_if(tokenized.begin(),
			tokenized.end(),
			[](string const& s) {
				return s.size() == 0;
			}),
		tokenized.end());

	return tokenized;
}



inline void removeStringType(string* nextLine, string charsToRemove)
{
	if (nextLine->find(charsToRemove) == 0)
		nextLine->erase(0, charsToRemove.size());
	
	nextLine->erase(0, nextLine->find_first_not_of(EMPTY_SPACE));
}

int getTextCoordFromToken(string token)
{

	size_t pos = token.find('/');
	if (pos == -1) return TEXTURE_COORDS_VALUE_NOT_FOUND;   // There is only one value, and this is vertex...

	// This one may need a bit more check, as it takes everything after the first /
	// and send it blindly to the stoi function...
	string textCoord = token.substr(pos + 1, token.size() );
	int index = stoi(textCoord);

	return index;
}


int getNormalFromToken(string token)
{

	size_t pos = token.find('/');
	if (pos == -1) return NORMAL_VALUE_NOT_FOUND;   // There is only one value, and this is vertex...

	size_t pos2 = token.find_last_of('/');
	if (pos == pos2)
	{
		// There is only one '/' here, either it is value case
		//  i.e:  '3/3' or a strange invalid case like '3/'	
		// But we are going to extract a second number, just to see...
		return NORMAL_VALUE_NOT_FOUND;
	}

	// If you reach that point, then there must be two '/', so you
	// can start to look for a third value, which is the normal!
	//string colorIndex = nextStr.substr(pos + 1, pos2 - pos - 1);
	string normal = token.substr(pos2 + 1, token.size() - pos2);

	return stoi(normal);
}


///////////////////////////////////////////////////////////////////////////////////////
//  Section of parsing functions: Those functions are invoked via function pointers  //
///////////////////////////////////////////////////////////////////////////////////////

bool parseVertexString(string nextLine)
{
	removeStringType(&nextLine, "v");

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	// We expect 3 floats here, otherwise, it is an invalid string 
	// and it can't be the definition of a vertex in 3D
	if (tokens.size() != VERTEX_NBR_COMPONENTS)  return false;

	
	Point3D newVertex;
	newVertex.x = stof(tokens.at(X_TOKEN_POS));
	newVertex.y = stof(tokens.at(Y_TOKEN_POS));
	newVertex.z = stof(tokens.at(Z_TOKEN_POS));

	vertexes.push_back(newVertex);
	
	return true;
}

bool parseNormalString(string nextLine)
{
	removeStringType(&nextLine, "vn");

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	// We expect 3 floats here, otherwise, it is an invalid string 
	// and can't be the defintion of a normal vector in 3D
	if (tokens.size() != NORMAL_NBR_COMPONENTS)  return false;

	Point3D newVertex;
	newVertex.x = stof(tokens.at(X_TOKEN_POS));
	newVertex.y = stof(tokens.at(Y_TOKEN_POS));
	newVertex.z = stof(tokens.at(Z_TOKEN_POS));

	normals.push_back(newVertex);

	return true;
}

bool parseFaceString(string nextLine)
{
	removeStringType(&nextLine, "f");

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	int vertexCount = tokens.size();
	Face3D newFace;

	
	// We only accept triangles and Quads for now...
	// May be expendeted a bit later?
	if (vertexCount < TRIANGLE || vertexCount > QUAD)   return false;


	newFace.id     = nextAvailFaceID++;
	newFace.type   = vertexCount;
	
	// Here, assign a values in face array, as we can read them...
	// But for the facing with less than 4 points, the next for-loop
	// will cover what it left in the newFace array with 0s.  
	int i = 0;
	for ( ; i < vertexCount; i++)
	{
		int vertexIndex = stoi(tokens.at(i));

		// We need to make sure here that the index is valid
		// There must a an index of an existing Vertex in the list of 
		// vertexed already recorded!
		if (vertexIndex <0 || vertexIndex > vertexes.size())  return false;

		newFace.vertex[i] = vertexIndex;
	}
	for (; i < 4; i++)				    // This is to make sure that if no vertex available
		newFace.vertex[i] = 0;          // to put at end,  put a clean 0 index instead..
	

	string firstToken = tokens.at(0);
	int indexNormal = getNormalFromToken(firstToken);
	if (indexNormal != NORMAL_VALUE_NOT_FOUND)
	{
		Point3D foundNormal  = normals.at(indexNormal - 1);
		newFace.normal.x = foundNormal.x;
		newFace.normal.y = foundNormal.y;
		newFace.normal.z = foundNormal.z;
	}
	else
		newFace.normal = POINT_ZERO;
	
	
	i = 0;
	for (; i < vertexCount; i++)
	{
		//int textCoordIndex = stoi(tokens.at(i));

		int textCoordIndex = getTextCoordFromToken(tokens.at(i));

		// We need to make sure here that the index is valid
		// There must a an index of an existing Texture Coords in the list of 
		// Coords already recorded!
		if (textCoordIndex <0 || textCoordIndex > textCoords.size())  return false;

		newFace.textCoords[i] = textCoordIndex;
	}
	for (; i < 4; i++)				    // This is to make sure that if no vertex available
		newFace.textCoords[i] = 0;


	newFace.color.red   = currentMaterialColor.red;
	newFace.color.green = currentMaterialColor.green;
	newFace.color.blue  = currentMaterialColor.blue;

	faces.push_back(newFace);

	return true;
}

bool parseTextureCoordsString(string nextLine)
{
	removeStringType(&nextLine, "vt");
	

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	// We expect 3 floats here for a valid coords in the texture file...
	// The coords x and y will strech from 0.0 and 1.0 
	if (tokens.size() != TEXTURE_COORDS_NBR_COMPONENTS)  return false;

	Point2D newVertex;
	newVertex.x = stof(tokens.at(X_TOKEN_POS));
	newVertex.y = stof(tokens.at(Y_TOKEN_POS));

	textCoords.push_back(newVertex);

	return true;



	return true;
}

// See note #1 at the end of this file...
bool parseEmptyString(string nextLine)
{
	return true;
}

// See note #1 at the end of this file...
bool parseCommentString(string nextLine)
{
	return true;
}

bool parseMaterialTemplateFileString(string nextLine)
{
	removeStringType(&nextLine, "mtllib");
	materialFileName = nextLine;

	return true;
}

// The use material command requires use to find in the list of recorded
// materials (The vector materials), the Material struct record that 
// has a matching key with the token provied as parameter of this method (nextLine)
bool parseSelectMaterialString(string nextLine)
{
	removeStringType(&nextLine, "usemtl");

	for (MaterialInfo nextMaterial : materials)
	{
		if (nextLine.compare(nextMaterial.key) == 0)
		{
			// If we find a match, put the color in the currentMaterialColor
			// which is the color to be used for the parsing of all the next
			// faces line to be parsed...
			currentMaterialColor.red   = nextMaterial.diffuse.red;
			currentMaterialColor.green = nextMaterial.diffuse.green;
			currentMaterialColor.blue  = nextMaterial.diffuse.blue;

			return true;
		}
	}

	// Unhappy scenario here: We found no material record that matches...
	// default to a gray color...
	currentMaterialColor.red  =  55.0f;
	currentMaterialColor.green = 55.0f;
	currentMaterialColor.blue  = 55.0f;

	return false;
}

bool parseNewMaterialString(string nextLine)
{
	removeStringType(&nextLine, "newmtl");

	MaterialInfo newMaterial;
	newMaterial.key = nextLine;

	materials.push_back(newMaterial);
	marterialLineStatus |= NEW_IDENTIFIED_IN_PROGRESS;

	return true;
}



bool parseAmbiantColorString(string nextLine)
{
	removeStringType(&nextLine, "Ka");

	MaterialInfo* lastMaterial = &materials.back();

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	// We expect 3 floats here, otherwise, it is an invalid string 
	// and can't be the defintion of a normal vector in 3D
	if (tokens.size() != COLOR_NBR_COMPONENTS)  return false;

	
	lastMaterial->ambiant.red   = stof(tokens.at(RED_TOKEN_POS));
	lastMaterial->ambiant.green = stof(tokens.at(GREEN_TOKEN_POS));
	lastMaterial->ambiant.blue  = stof(tokens.at(BLUE_TOKEN_POS));

	return true;
}

bool parseDiffuseColorString(string nextLine)
{
	removeStringType(&nextLine, "Kd");

	MaterialInfo* lastMaterial = &materials.back();

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	// We expect 3 floats here, otherwise, it is an invalid string 
	// and can't be the defintion of a normal vector in 3D
	if (tokens.size() != COLOR_NBR_COMPONENTS)  return false;


	lastMaterial->diffuse.red = stof(tokens.at(RED_TOKEN_POS));
	lastMaterial->diffuse.green = stof(tokens.at(GREEN_TOKEN_POS));
	lastMaterial->diffuse.blue = stof(tokens.at(BLUE_TOKEN_POS));

	return true;
}

bool parseSpecularColorString(string nextLine)
{
	removeStringType(&nextLine, "Ks");

	MaterialInfo* lastMaterial = &materials.back();

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	// We expect 3 floats here, otherwise, it is an invalid string 
	// and can't be the defintion of a normal vector in 3D
	if (tokens.size() != COLOR_NBR_COMPONENTS)  return false;


	lastMaterial->specular.red = stof(tokens.at(RED_TOKEN_POS));
	lastMaterial->specular.green = stof(tokens.at(GREEN_TOKEN_POS));
	lastMaterial->specular.blue = stof(tokens.at(BLUE_TOKEN_POS));

	return true;
}


bool parseTextureFileString(string nextLine)
{
	removeStringType(&nextLine, "map_Kd");

	MaterialInfo* lastMaterial = &materials.back();

	const vector<string> tokens = tokenize(nextLine, SPACE_REGEX);
	// We expect only string with the name of the texture file here...
	if (tokens.size() != TEXTURE_FILE_NBR_COMPONENTS)  return false;

	regex  reg("\\\\+");   // Matches one or more backslashes
	string newString = regex_replace(tokens.at(0), reg, "/");	

	//newString.insert(0, "./");
	lastMaterial->textureFile = newString;

	return true;
}



int processLine(const int lineCount,string nextLine)
{
	bool matchFound = false;
	smatch sm;

	string charsTrim = " \t";


	bool (*parseMethod_ptr)(string);
	
	// Eliminate any spacing or tab at the end of any string 
	// given to this method, as this will ease the parsing to come later...
	nextLine.erase(0, nextLine.find_first_not_of(charsTrim));

	for (int i = 0; i < REGEX_PATTERNS_COUNT; i++)
	{
		if (regex_match(nextLine, sm, formatRegex[i]))
		{
		
			if (!parseFunctions[i](nextLine))    
				parsingErrors.push_back(lineCount);
			matchFound = true;
			break;
		}
	}

	return OK;
}


int loadMaterialFile(string materialFileName)
{
	fstream materialFileStream;

	if (materialFileName.size() == 0)  return ERROR;
	
	materialFileStream.open(materialFileName, ios::in); //open a file to perform read operation using file object
	int lineCount = 1;

	if (!materialFileStream.is_open())
	{
		cout << "Error, can't open file " << materialFileName << "!\n";
		return ERROR;
	}

	string tp;
	while (getline(materialFileStream, tp)) { //read data from file object and put it into string.
		processLine(lineCount++, tp);
	}
	materialFileStream.close(); //close the file object.


	//vector<MaterialInfo>  materials;
	return OK;
}

int  loadObjFile(string objFileName)
{
	fstream objFileStream;

	
	objFileStream.open(objFileName, ios::in); //open a file to perform read operation using file object
	int lineCount = 1;

	if (!objFileStream.is_open())
	{
		cout << "Error, can't open file " << objFileName << "!\n";
		return ERROR;
	}


	string tp;
	while (getline(objFileStream, tp)) { //read data from file object and put it into string.
		processLine(lineCount++, tp);
	}
	objFileStream.close(); //close the file object.

	
	return OK;
}




int loadFile(string objFileName, string materialFileName)
{
	fstream objFileStream;
	
	// It is better to get all the material first, as the obj
	// files appears to 'work' by section of faces using a common
	// materials. So once a new section is detected to use a specific
	// material, it is easy to fetch it during the parsing of every faces...
	if (loadMaterialFile(materialFileName) != OK)
	{
		return false;
	}

	if (loadObjFile(objFileName) != OK)
	{
		return false;
	}

	// This next section may not be relevent anymore inside a GUI app 
	// (used to be initially into a Consol app), as the error results
	//  of the parsing are not displayed anywhere...
	if (parsingErrors.size() > 0)
	{
		cout << "\nError Report:\n";

		for (int errorLine : parsingErrors)
			cout << "\tLine # " << errorLine << "\n";

		cout << "Total Errors:  " << parsingErrors.size() << "\n";
	}
	else
		cout << "No parsing error did happen\n";


	/*Model3D* model = GeneralModel::getInstance()->getModel3D();
	copy3DModel(model);*/


	Model3D*  model = new Model3D();
	copy3DModel(model);
	GeneralModel::getInstance()->addModel3D(model);

	cleanUp();

	//model->setModelColorType(TEXTURE_FACES); // Old
	model->setModelColorType(COLOR_FACES);
		
	return OK;
}

bool copy3DModel(vector<Point3D>*& destiVertexes,
			     vector<Point3D>*& destiNormals,
				 vector<Face3D>*& destiFaces)
{

	for (Point3D nextVertex : vertexes)
	{
		Point3D copy;
		copy.x = nextVertex.x; copy.y = nextVertex.y; copy.z = nextVertex.z;
		destiVertexes->push_back(copy);
	}

	for (Point3D nextNormal : normals)
	{
		Point3D copy;
		copy.x = nextNormal.x; copy.y = nextNormal.y; copy.z = nextNormal.z;
		destiNormals->push_back(copy);
	}

 	for (Face3D nextFace : faces)
	{
		Face3D copy;
		copy.id = nextFace.id;
		copy.type = nextFace.type;

		copy.normal.x = nextFace.normal.x;
		copy.normal.y = nextFace.normal.y;
		copy.normal.z = nextFace.normal.z;

		copy.color.red   = nextFace.color.red;
		copy.color.green = nextFace.color.green;
		copy.color.blue  = nextFace.color.blue;

		for (int i = 0; i < nextFace.type; i++)
			copy.vertex[i] = nextFace.vertex[i];

		for (int i = 0; i < nextFace.type; i++)
			copy.textCoords[i] = nextFace.textCoords[i];

		
		destiFaces->push_back(copy);
	}

	return true;
}


//bool copy3DModel(vector<Point3D>*& destiVertexes,
//	vector<Point3D>*& destiNormals,
//	vector<Point2D>*& destiTextCoords,
//	vector<Face3D>*& destiFaces)

bool copy3DModel(vector<Point3D>*& destiVertexes,
				 vector<Point3D>*& destiNormals,
				 vector<Point2D>*& destiTextCoords,
				 vector<Face3D>*& destiFaces,
				 vector<MaterialInfo>*& destiMaterials)
{
	copy3DModel(destiVertexes, destiNormals, destiFaces);

	for (Point2D nextCoord : textCoords)
	{
		Point2D copy;
		copy.x = nextCoord.x; copy.y = nextCoord.y;
		destiTextCoords->push_back(copy);
	}

	for (MaterialInfo nextMaterial : materials)
	{
		MaterialInfo copy;
		copy.key = nextMaterial.key;

		copy.ambiant.red   = nextMaterial.ambiant.red;
		copy.ambiant.green = nextMaterial.ambiant.green;
		copy.ambiant.blue  = nextMaterial.ambiant.blue;
		
		copy.diffuse.red   = nextMaterial.diffuse.red;
		copy.diffuse.green = nextMaterial.diffuse.green;
		copy.diffuse.blue  = nextMaterial.diffuse.blue;

		copy.specular.red   = nextMaterial.specular.red;
		copy.specular.green = nextMaterial.specular.green;
		copy.specular.blue  = nextMaterial.specular.blue;


		copy.textureFile = nextMaterial.textureFile;

		//copy.x = nextCoord.x; copy.y = nextCoord.y;
		destiMaterials->push_back(copy);
	}



	return true;
}

bool copy3DModel(Model3D* destiModel)
{
	vector<Point3D>*      vertexes   = destiModel->getVertexList();
	vector<Point3D>*      normals    = destiModel->getNormalList();
	vector<Point2D>*      textCoords = destiModel->getTextureCoordList();
	vector<Face3D>*       faces      = destiModel->getFaceList();
	vector<MaterialInfo>* materials  = destiModel->getMaterialList();
	copy3DModel(vertexes, normals, textCoords, faces, materials);

	return true;
}



void cleanUp()
{
	vertexes.clear();    
	normals.clear();
	textCoords.clear();
	faces.clear();
}







// *** Note #1 ***
// There exists a parsing method of empty and comment line,
// even as it has no work to be done...
// It exists to make sure use of an array of functions pointers
// with no exemptions (it calls a function pointers, all the time
// regardless of what is to be parsed, included an empty or comment line)

