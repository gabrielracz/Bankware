#ifndef DATA_H
#define DATA_H
#include "GL/gl.h"

#define NUM_TEXTURES 29

enum ObjectTypes : GLuint {
	SHIP=0,
	GRID,
	BACKGROUND1,
	BACKGROUND2,
	BACKGROUND3,
	BACKGROUND4,
	ENEMY,
	BLADE,
	BULLET,
	EXPLOSION,
	SHIELD,
	POWERUP,
	BUOY,
	FIRE,
	PARTICLE_EXPLOSION,
	TEXT,
	DASH,
	BUFF,
	E_BULLET,
	NONE,
	CANNON,
	GUNNER,
	WEAPON_C,
	REDKEY,
	BLUEKEY,
	YELLOWKEY,
	GATE_OPEN,
	GATE_CLOSED
};

#endif
