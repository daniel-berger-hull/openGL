#include "Camera3D.h"


Camera3D::Camera3D()
{
	setLocation({0.0f, 0.0f, 0.0f});
    setRotation({ 0.0f, 0.0f, 0.0f });
}

Camera3D::Camera3D(Point3D l, Point3D r)
{
	setLocation({l.x, l.y, l.z});
	setRotation({r.x, r.y, r.z });
}
