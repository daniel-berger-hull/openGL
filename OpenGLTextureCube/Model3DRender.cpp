#include "Model3DRender.h"
#include "WorldObject3D.h"
#include "Model3D.h"
#include "OpenGLFace.h"

#include "OpenGLCanvas.h"


#ifdef __DARWIN__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

bool Model3DRender::render(Model3D* model)
{
	OpenGLFace faceRender;

	vector<Point3D>* vertexes = model->getVertexList();
	vector<Point2D>* textCoords = model->getTextureCoordList();
	vector<Face3D>* faces = model->getFaceList();

	if (model->getModelColorType() == TEXTURE_FACES)
	{
		// Will have to deal at some point with the texture, 
		//who are created in the OpenGLCanvas currently
		//glglBindTexture(GL_TEXTURE_2D, texture[0]);BindTexture(GL_TEXTURE_2D, texture[0]);
	}

	for (Face3D next : *faces)
	{
		switch (model->getModelColorType())
		{
		case(COLOR_FACES):
			faceRender.colorRender(next, vertexes);
			break;
		case(TEXTURE_FACES):
			faceRender.textureRender(next, vertexes, textCoords);
			break;
		}
	}
		


	return true;
}


bool Model3DRender::render(WorldObject3D* object3D)
{
	OpenGLFace faceRender;
	

	Model3D* model = object3D->getModel();


	vector<Point3D>* vertexes = model->getVertexList();
	vector<Point2D>* textCoords = model->getTextureCoordList();
	vector<Face3D>* faces = model->getFaceList();


	Point3D pos = object3D->getPosition();
	Point3D rot = object3D->getRotation();




	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.x, 1.0f, 0.0f , 0.0f);
	glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rot.z, 0.0f, 0.0f, 1.0f);



	if (model->getModelColorType() == TEXTURE_FACES)
	{
		// Will have to deal at some point with the texture, 
		//who are created in the OpenGLCanvas currently
		//glglBindTexture(GL_TEXTURE_2D, texture[0]);BindTexture(GL_TEXTURE_2D, texture[0]);
	}

	for (Face3D next : *faces)
	{
		switch (model->getModelColorType())
		{
		case(COLOR_FACES):
			//faceRender.colorRender(next, vertexes);
			faceRender.colorRender(next, vertexes, object3D->getFactor());

			break;
		case(TEXTURE_FACES):
			faceRender.textureRender(next, vertexes, textCoords);
			//faceRender.textureRender(next, vertexes, textCoords, object3D->getFactor());

			break;
		}
	}


	glPopMatrix();

	return true;
}

