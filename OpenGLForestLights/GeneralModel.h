#pragma once

#include <string>
#include <list>
#include <vector>

#include "Model3D.h"
#include "WorldObject3D.h"
#include "Camera3D.h"
#include "Light3D.h"

using namespace std;

class OpenGLCanvas;
class ControlPanel;

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
	

	ControlPanel* getControlPanel() {
		return this->controlPanel;
	}

	void setControlPanel(ControlPanel* control) {
		this->controlPanel = control;
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


	Model3D* getModel3D() 
	{
		return &(this->model);
	}

	void setModel3D(Model3D data) 
	{
		this->model = data;
	}

	Camera3D* getCamera3D() 
	{
		return &(this->camera3D);
	}

	void setCamera3D(Camera3D cam) 
	{
		this->camera3D = cam;
	}

	Light3D* getLight3D()
	{
		return &(this->light3D);
	}

	void setLight3D(Light3D cam)
	{
		this->light3D = cam;
	}

	string getModelPath() 
	{
		return this->modelPath;
	}

	void setModelPath(string path) 
	{
		this->modelPath = path;
	 }

	Point3D* getLightLocation()   
	{ 
		return &(this->lightLocation); 
	}

	void setLightLocation(Point3D location)
	{
		this->lightLocation = location;
	}
	
	void moveLightLocation(Point3D delta)
	{
		this->lightLocation.x += delta.x;
		this->lightLocation.y += delta.y;
		this->lightLocation.z += delta.z;

	}

	void refreshControlPanel();

private:

	static GeneralModel* instance;

	OpenGLCanvas* canvas;
	ControlPanel* controlPanel;

	list<WorldObject3D> objectsList;
	list<Model3D*>       modelsList;
	vector<Model3D*>     modelsVector;


	Model3D             model;
	Camera3D            camera3D;
	Light3D				light3D;
	Point3D				lightLocation;


	string modelPath;
};

