#pragma once
#include <GL/glut.h>
#include "Globals.h"
#include "cstdlib"
#include <iostream>

#define BLASTER_SHOT_VELOCITY 0.01*CUBE_SIZE

class BlasterShot {
private:
	bool active;
	Coord pos;
	//velocities
	double xv, yv, zv;

public:
	BlasterShot();
	void fire(Coord startPosition, Coord direction);
	bool move();
	Coord getPos();
	void destroy();
	void draw();
	bool isActive();

};