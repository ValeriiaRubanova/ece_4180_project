#pragma once

#define DEBUG 0

#define CUBE_SIZE 1000
#define SPAWN_DISTANCE	50
#define DIR_SCALING_FACTOR	0.1

#define ASTEROID_SIZE 6
#define SPACESHIP_SIZE	3
#define BLASTER_SIZE	1

#define NUM_BLASTER_SHOTS	5

#define MAX_ASTEROID_VELOCITY 0.0002*CUBE_SIZE
#define SPACESHIP_VELOCITY 0.0003*CUBE_SIZE

#define GET_HIGH_SCORES 1

#define SHOT_FIRED			0b00000001
#define ASTEROID_SHOT		0b00000010
#define COLLISION			0b00000100
#define GAME_OVER			0b00001000
#define NEW_HIGH_SCORE		0b00100000
#define REQUEST_HIGH_SCORES	0b10000000

struct Coord {
	double x;
	double y;
	double z;
};

