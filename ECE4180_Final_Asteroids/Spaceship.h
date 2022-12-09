
#pragma once
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <mutex>
#include "Serial_Com.h"
#include "BlasterShot.h"



//has method to draw pacman
class Spaceship {
public:
	//draws pacman as a yellow sphere
	Spaceship();


	Coord getPos() { return pos; };
	int getLives() { return lives; };
	BlasterShot* getBlasterShot(int i);

	void updateDirection();
	void spaceshipDies();
	void exitGame();
	
	void drawSpaceship();
	void move();
	void fire();
	


private:
	int lives;
	Coord pos;
	Coord dir;
	BlasterShot blaster_shots[NUM_BLASTER_SHOTS];

};