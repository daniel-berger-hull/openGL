#include "WorldObject3D.h"



WorldObject3D::WorldObject3D()
{
	setPosition({ 0.0f, 0.0f, 0.0f });
	setRotation({ 0.0f, 0.0f, 0.0f });
}

WorldObject3D::WorldObject3D(Point3D pos)
{
	setPosition(pos);
	setRotation({ 0.0f, 0.0f, 0.0f });
}

void WorldObject3D::setPosition(Point3D pos)
{
	const float maxX = (float)WORLD_WIDTH / 2;
	const float minX = -maxX;
	const float maxY = (float)WORLD_HEIGHT / 2;
	const float minY = -maxY;
	const float maxZ = (float)WORLD_DEPTH / 2;
	const float minZ = -maxX;


	if (pos.x < minX) pos.x = minX;
	else if (pos.x > maxX) pos.x = maxX;
	if (pos.y < minY) pos.y = minY;
	else if (pos.y > maxY) pos.y = maxY;
	if (pos.z < minZ) pos.z = minZ;
	else if (pos.z > maxZ) pos.z = maxZ;

	position.x = pos.x;  position.y = pos.y;  position.z = pos.z;
}

void WorldObject3D::setRotation(Point3D rot)
{
	rotation.x = rot.x;  rotation.y = rot.y;  rotation.z = rot.z;
}


