#pragma once
class ModelRotationState
{
public:
	bool isXRotation() { return rotateX; }
	bool isYRotation() { return rotateY; }
	bool isZRotation() { return rotateZ; }

	//bool toggleXRotation() { rotateX != rotateX; return rotateX;  }
	//bool toggleYRotation() { rotateY != rotateY; return rotateY;  }
	//bool toggleZRotation() { rotateZ != rotateZ; return rotateZ;  }
	void toggleXRotation() { rotateX = (rotateX == true) ? false : true;  }
	void toggleYRotation() { rotateY = (rotateY == true) ? false : true;  }
	void toggleZRotation() { rotateZ = (rotateZ == true) ? false : true;  }


	void setXRotation(bool state) { rotateX = state; }
	void setYRotation(bool state) { rotateY = state; }
	void setZRotation(bool state) { rotateZ = state; }

	float getXAngle() { return angleX; }
	float getYAngle() { return angleY; }
	float getZAngle() { return angleZ; }
	void  setXAngle(float newAngle) { angleX = newAngle; }
	void  setYAngle(float newAngle) { angleY = newAngle; }
	void  setZAngle(float newAngle) { angleZ = newAngle; }

	void update();
private:
	bool rotateX = false;
	bool rotateY = true;
	bool rotateZ = false;

	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	float dx = 0.5f;
	float dy = 0.5f;
	float dz = 0.5f;

};

