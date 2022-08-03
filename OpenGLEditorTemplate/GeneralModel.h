#pragma once

#include "Model3D.h"

class GeneralModel
{
public:
	static GeneralModel* getInstance() {
		if (!instance)
			instance = new GeneralModel;
		return instance;
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



private:
	static GeneralModel* instance;
	int data;
	Model3D model;

};



