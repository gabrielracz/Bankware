#ifndef DATA_H
#define DATA_H
#include "GL/gl.h"
#define NUM_TEXTURES 9

enum ObjectTypes : GLuint {
	SHIP=0,
	BACKGROUND,
	ENEMY,
	BLADE,
	BULLET,
	EXPLOSION,
	SHIELD,
	POWERUP,
	BUOY
};

#endif
