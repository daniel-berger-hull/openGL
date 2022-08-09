#pragma once

#include "Model3D.h"
#include "ModelRotationState.h"


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

	void setOpenGLCanvas(OpenGLCanvas* canvas ) {
		this->canvas = canvas;
	}

	int getData() {
		return this->data;
	}

	void setData(int data) {
		this->data = data;
	}

	Model3D* getModel3D() {
		return &(this->model);
	}

	void setModel3D(Model3D data) {
		this->model = data;
	}

	ModelRotationState* getModelRotationState() {
		return &(this->rotationState);
	}

	void setModelRotationState(ModelRotationState state) {
		this->rotationState = state;
	}



private:
	static GeneralModel* instance;

	int data;

	OpenGLCanvas* canvas;

	Model3D model;
	ModelRotationState rotationState;

};



