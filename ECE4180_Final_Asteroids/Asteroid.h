#pragma once
#include <GL/glut.h>
#include "Globals.h"
#include "cstdlib"



class Asteroid {
private:
	bool active;
	Coord pos;
	//velocities
	double xv, yv, zv;
	//0 to 3, how many times it breaks apart into smaller asteroids
	//for simplicity starting the game with they are all small and dont break apart, add additional fuctionality later if there is time
	int size;
public:


	Asteroid(Coord spawnCenter);
	bool move();
	Coord getPos();
	void destroy();
	void drawAsteroid();
	bool getActive() { return active; };

};