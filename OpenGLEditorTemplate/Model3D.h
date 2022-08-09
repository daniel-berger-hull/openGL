#pragma once

#include <iostream>
#include <iomanip>      // std::setprecision

#include <list>

#include <stdlib.h>
#include <stdio.h>


#define	SUCCESS  true
#define FAILURE  false

#define	INVALID_FACE_ID     -10

#define HEADER_MAGIC_NUMBER 0X1010
#define	MAX_NUMBER_RANGE    100000

#define TRIANGLE   1
#define QUAD       2


typedef struct FileHeader {
	int magicNumber;
	int nbrPoints;
	int nbrFaces;
};

typedef struct Point3D {
	float x, y, z;
};

typedef struct Face3D {
	long id;
	int type;
	Point3D  normal;
	Point3D  color;
	Point3D  vertex[4];
};



class Model3D
{
	public:

		Model3D();

		bool save(const char* file_name);
		bool writeHeader(FILE* fp);
		bool writeFace(FILE* fp, int type, Point3D* ptrNormal, Point3D* color, Point3D* ptrVertex);
		
		bool load(const char* file_name);
		bool readHeader(FILE* fp);
		bool readFace(FILE* fp, Face3D* destination);

		bool addFace(int type, Point3D* ptrNormal, Point3D* color, Point3D* ptrVertex);


		void displayHeaderDetails();
		void displayFaceDetails(Face3D* destination);

		int getFacesCount()       { return fileHeader.nbrFaces; }
		int getPointsCount()      { return fileHeader.nbrPoints; }

		Face3D*  getFace(long id);


		std::list<Face3D>* getFaceList() { return &facesList; }

		bool isValidNumber(int number);
private:
	int a;
	FileHeader fileHeader;
	std::list<Face3D> facesList;
	long nextFaceIDAvailabe = 0;

	Face3D selected;

};

