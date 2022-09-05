#pragma once

#include <string>
#include <list>
#include <vector>

#include "Model3D.h"
#include "WorldObject3D.h"
#include "Camera3D.h"

using namespace std;

class OpenGLCanvas;

class GeneralModel
{
public:
	static GeneralModel* getInstance() {
		if (!instance)
			instance = new GeneralModel;
		return instance;
	}


	OpenGLCanvas* getOpenGLCanvas() {
		return this->canvas;
	}

	void setOpenGLCanvas(OpenGLCanvas* canvas) {
		this->canvas = canvas;
	}


	list<WorldObject3D>* getWorldObjectsList() 
	{
		return &(this->objectsList);
	}

	void addWorldObject3D(WorldObject3D newObject)
	{
		objectsList.push_back(newObject);
	}

	int sizeWorldObjects()
	{
		return objectsList.size();
	}


	list<Model3D*>* getModelsList()
	{
		return &(this->modelsList);
	}

	/*void addModel3D(Model3D* newModel)
	{
		modelsList.push_back(newModel);
	}*/

	// old
	/*int sizeModel3D()
	{
		return modelsList.size();
	}*/


	vector<Model3D*>* getModelsVector()
	{
		return &(this->modelsVector);
	}

	void addModel3D(Model3D* newModel)
	{
		modelsVector.push_back(newModel);
	}

	int sizeModel3D()
	{
		return modelsVector.size();
	}





	Model3D* getModel3D() {
		return &(this->model);
	}

	void setModel3D(Model3D data) {
		this->model = data;
	}

	Camera3D* getCamera3D() {
		return &(this->camera3D);
	}

	void setCamera3D(Camera3D cam) {
		this->camera3D = cam;
	}

	string getModelPath() {
		return this->modelPath;
	}

	void setModelPath(string path) {
		this->modelPath = path;
	 }




private:

	//GeneralModel();


	static GeneralModel* instance;

	OpenGLCanvas* canvas;

	list<WorldObject3D> objectsList;
	list<Model3D*>       modelsList;
	vector<Model3D*>     modelsVector;


	Model3D             model;
	Camera3D            camera3D;


	string modelPath;
};

