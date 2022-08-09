#include "ModelRotationState.h"


void ModelRotationState::update()
{

	if (rotateX) { angleX += dx; }
	if (rotateY) { angleY += dy; }
	if (rotateZ) { angleZ += dz; }
}
