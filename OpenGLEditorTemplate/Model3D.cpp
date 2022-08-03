#include "Model3D.h"


using namespace std;

Model3D::Model3D()
{
	fileHeader.magicNumber = HEADER_MAGIC_NUMBER;
	fileHeader.nbrFaces = 0;
	fileHeader.nbrPoints = 0;

}


bool Model3D::save(const char* file_name)
{
	if (file_name == NULL)  return FAILURE;


	FILE* fp = fopen(file_name, "wb");
	if (fp == NULL)  /* If an error occurs during the file creation */
	{
		return FAILURE;
	}

	writeHeader(fp);

	for (std::list<Face3D>::iterator it = facesList.begin(); it != facesList.end(); ++it)
	{
		Face3D nextFace = *it;
		writeFace(fp, nextFace.type, 
				   &nextFace.normal, 
				   &nextFace.color, 
				   &nextFace.vertex[0]);
	}

	fclose(fp);
	return SUCCESS;
}

bool Model3D::writeHeader(FILE* fp)
{
	
	size_t elements_written = fwrite(&fileHeader, sizeof(FileHeader), 1, fp);

	if (elements_written == 1)
		return SUCCESS;
	else
		return FAILURE;
}


bool Model3D::writeFace(FILE* fp, int type, Point3D* ptrNormal, Point3D* color, Point3D* ptrVertex)
{
	if (type != TRIANGLE && type != QUAD)  return FAILURE;

	size_t elements_written;
	size_t nbr_points;

	if (type == TRIANGLE) nbr_points = 3;
	else if (type == QUAD) nbr_points = 4;

	//Write the type of face (1 = Triangle, 2 = Quad)
	elements_written = fwrite(&type, sizeof(int), 1, fp);
	//Write the normal vector of the face
	elements_written = fwrite(ptrNormal, sizeof(Point3D), 1, fp);
	//Write the RGB value of color 
	elements_written = fwrite(color, sizeof(Point3D), 1, fp);
	//Write the points of the face
	elements_written = fwrite(ptrVertex, sizeof(Point3D), nbr_points, fp);

	return SUCCESS;
}


bool Model3D::load(const char* file_name)
{
	if (file_name == NULL)  return FAILURE;

	FILE* fp = fopen(file_name, "rb");
	if (fp == NULL)  /* If an error occurs during the file creation */
	{
		return FAILURE;
	}

	Face3D  nextFace;

	if (!readHeader(fp))   return FAILURE;

	for (int i=0; i< fileHeader.nbrFaces;i++)
	{
		readFace(fp, &nextFace);
		addFace(nextFace.type, &nextFace.normal, &nextFace.color, &nextFace.vertex[0]);
	}


	fclose(fp);
	return SUCCESS;
}

bool Model3D::readHeader(FILE* fp)
{
	size_t elements_read = fread(&fileHeader, sizeof(FileHeader), 1, fp);

	if (fileHeader.magicNumber != HEADER_MAGIC_NUMBER) return FAILURE;
	if (!isValidNumber(fileHeader.nbrFaces))         return FAILURE;
	if (!isValidNumber(fileHeader.nbrPoints))          return FAILURE;

	return SUCCESS;
}

bool Model3D::readFace(FILE* fp, Face3D* destination)
{
	//A pointer on a valid instance is a must here...
	if (destination == NULL)  return FAILURE;


	size_t elements_read;
	int type;
	Point3D normal;
	Point3D vertex[4];
	size_t nbr_points;


	//Read the type of face (1 = Triangle, 2 = Quad)
	elements_read = fread(&(destination->type), sizeof(int), 1, fp);
	// the normal vector ... 
	elements_read = fread(&(destination->normal), sizeof(Point3D), 1, fp);
	//  the color ... 
	elements_read = fread(&(destination->color), sizeof(Point3D), 1, fp);
	// and finally, Read all the points of the face

	if (destination->type == TRIANGLE) nbr_points = 3;
	else if (destination->type == QUAD) nbr_points = 4;

	elements_read = fread(&destination->vertex[0], sizeof(Point3D), nbr_points, fp);

	return SUCCESS;
}

bool Model3D::addFace(int type, Point3D* ptrNormal, Point3D* color, Point3D* ptrVertex)
{
	if (type != TRIANGLE && type != QUAD)  return FAILURE;
	if (ptrNormal == NULL )                return FAILURE;
	if (color == NULL)                     return FAILURE;
	if (ptrVertex == NULL)                 return FAILURE;

	int nbr_points;
	if (type == TRIANGLE) nbr_points = 3;
	else if (type == QUAD) nbr_points = 4;


	Face3D newFace;
	newFace.type = type;
	newFace.normal = *ptrNormal;
	newFace.color = *color;
	for (int i = 0; i < nbr_points; i++)
	{
		newFace.vertex[i] = *ptrVertex;
		ptrVertex++;
		fileHeader.nbrPoints++;
	}

	facesList.push_back(newFace);
	//fileHeader.nbrFaces++;
}


void Model3D::displayHeaderDetails()
{

	cout << "File Header:\n\tMagic Number --> " << fileHeader.magicNumber
		<< "\n\tNumber of Faces --> " << fileHeader.nbrFaces
		<< "\n\tTotal Number of points --> " << fileHeader.nbrPoints
		<< "\n";
}



void Model3D::displayFaceDetails(Face3D* destination)
{
	if (destination == NULL)  return;

	size_t nbr_points;

	cout << "Type: ";
	switch (destination->type)
	{
	case TRIANGLE:
		cout << "TRIANGLE\n";
		nbr_points = 3;
		break;
	case QUAD:
		cout << "QUAD\n";
		nbr_points = 4;
		break;
	default:
		cout << "Not Defined!\n";
		nbr_points = 0;
		break;
	}
	cout << "Normal [" << fixed << setprecision(3);
	cout << destination->normal.x << "," << destination->normal.y << "," << destination->normal.z << "]\n";

	cout << "Color [" << fixed << setprecision(1);
	cout << destination->color.x << "," << destination->color.y << "," << destination->color.z << "]\n";


	for (int i = 0; i < nbr_points; i++)
	{
		cout << fixed << setprecision(3);
		cout << i << "   -   [" << destination->vertex[i].x << "," << destination->vertex[i].y << "," << destination->vertex[i].z << "]\n";
	}

}

bool Model3D::isValidNumber(int number)
{
	if (number < 0)  return FAILURE;
	if (number > MAX_NUMBER_RANGE) return FAILURE;

	return SUCCESS;
}

