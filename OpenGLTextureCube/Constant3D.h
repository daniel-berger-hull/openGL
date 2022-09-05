#pragma once

#define WORLD_WIDTH    80
#define WORLD_HEIGHT   80
#define WORLD_DEPTH    80


#define CAMERA_X_POS       0
#define CAMERA_Y_POS       0
#define CAMERA_Z_POS     -65

#define CAMERA_X_ROT      10
#define CAMERA_Y_ROT       0
#define CAMERA_Z_ROT       0


typedef struct Point2D {
	float x, y;
};

typedef struct Point3D {
	float x, y, z;
};