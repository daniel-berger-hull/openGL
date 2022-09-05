#include "OpenGLFace.h"

#include "wx/glcanvas.h"

//#ifdef __DARWIN__
//#include <OpenGL/glu.h>
//#else
//#include <GL/glu.h>
//#endif




bool OpenGLFace::colorRender(Face3D face, vector<Point3D>* vertexes)
{
	/*Point3D  normal = face.normal;
	RGB      color = face.color;

	if (face.type == TRIANGLE)
	{
		glBegin(GL_TRIANGLES);
		glColor3f(color.red, color.green, color.blue);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < face.type; i++)
		{
			int nextIndex = face.vertex[i];
			Point3D nextPoint = vertexes->at(nextIndex - 1);
			glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
		}
		glEnd();
	}
	else if (face.type == QUAD)
	{
		glBegin(GL_QUADS);
		glColor3f(color.red, color.green, color.blue);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < face.type; i++)
		{
			int nextIndex = face.vertex[i];
			Point3D nextPoint = vertexes->at(nextIndex - 1);
			glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
		}
		glEnd();
	}

	return  true;*/


	return colorRender(face, vertexes, 1.0f);
}

bool OpenGLFace::colorRender(Face3D face, vector<Point3D>* vertexes, float factor)
{
	Point3D  normal = face.normal;
	RGB      color = face.color;

	if (face.type == TRIANGLE)
	{
		glBegin(GL_TRIANGLES);
		glColor3f(color.red, color.green, color.blue);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < face.type; i++)
		{
			int nextIndex = face.vertex[i];
			Point3D nextPoint = vertexes->at(nextIndex - 1);

			nextPoint.x *= factor;
			nextPoint.y *= factor;
			nextPoint.z *= factor;

			glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
		}
		glEnd();
	}
	else if (face.type == QUAD)
	{
		glBegin(GL_QUADS);
		glColor3f(color.red, color.green, color.blue);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < face.type; i++)
		{
			int nextIndex = face.vertex[i];
			Point3D nextPoint = vertexes->at(nextIndex - 1);
			nextPoint.x *= factor;
			nextPoint.y *= factor;
			nextPoint.z *= factor;

			glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
		}
		glEnd();
	}

	return  true;
}



bool OpenGLFace::textureRender(Face3D face, vector<Point3D>* vertexes, vector<Point2D>* textCoords)
{

	Point3D  normal = face.normal;
	RGB      color = face.color;

	if (face.type == TRIANGLE)
	{
		glBegin(GL_TRIANGLES);
		glColor3f(color.red, color.green, color.blue);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < face.type; i++)
		{
			//  !! Code the faces with texture here!!


			int nextIndex = face.vertex[i];
			//					Point3D nextPoint = vertexes.at(nextIndex-1);  // Old
			Point3D nextPoint = vertexes->at(nextIndex - 1);

			glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
		}
		glEnd();
	}
	else if (face.type == QUAD)
	{
		glBegin(GL_QUADS);
		glColor3f(color.red, color.green, color.blue);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < face.type; i++)
		{
			// Add logic to see if a texture or color is used here, instead
			// of assuming that a texture is always used
			int nextTextCoordIndex = face.textCoords[i];
			Point2D nextCoord = textCoords->at(nextTextCoordIndex - 1);
			glTexCoord2f(nextCoord.x, nextCoord.y);

			int nextIndex = face.vertex[i];
			Point3D nextPoint = vertexes->at(nextIndex - 1);
			glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
		}
		glEnd();
	}

	return  true;
}
