
#include "Asteroid.h"

Asteroid::Asteroid(Coord spawnCenter) {

	active = true;
	//make this a random number 0-3 once functionality of breaking apart is implemented
	size = 0;

	xv = MAX_ASTEROID_VELOCITY /2 - MAX_ASTEROID_VELOCITY *((double)rand() / (double)RAND_MAX);
	yv = MAX_ASTEROID_VELOCITY/2 - MAX_ASTEROID_VELOCITY * ((double)rand() / (double)RAND_MAX);
	zv = MAX_ASTEROID_VELOCITY/2 - MAX_ASTEROID_VELOCITY * ((double)rand() / (double)RAND_MAX);
	
	int randNum = (rand() % SPAWN_DISTANCE*12);
	//pink wall
	if (randNum <= SPAWN_DISTANCE *2) {
		pos.x = SPAWN_DISTANCE;
		pos.y= SPAWN_DISTANCE -((randNum) % (SPAWN_DISTANCE *2));
		pos.z = SPAWN_DISTANCE -(rand() % SPAWN_DISTANCE *2);

		if (xv > 0) {
			xv = -xv;
		}
	}
	//yellow wall
	if (randNum > SPAWN_DISTANCE *2 && randNum <= SPAWN_DISTANCE *4) {
		pos.x = -SPAWN_DISTANCE;
		pos.y = SPAWN_DISTANCE - ((randNum) % (SPAWN_DISTANCE *2));
		pos.z = SPAWN_DISTANCE -(rand() % SPAWN_DISTANCE *2);
		if (xv < 0) {
			xv = -xv;
		}
	}

	//red wall
	if (randNum > SPAWN_DISTANCE *4 && randNum <= SPAWN_DISTANCE *6) {
		pos.y = -SPAWN_DISTANCE;
		pos.x = SPAWN_DISTANCE -((randNum) % (SPAWN_DISTANCE *2));
		pos.z = (rand() % SPAWN_DISTANCE *2);
		if (yv < 0) {
			yv = -yv;
		}
	}

	//cyan wall
	if (randNum > SPAWN_DISTANCE *6 && randNum <= SPAWN_DISTANCE *8) {
		pos.y = SPAWN_DISTANCE;
		pos.x = SPAWN_DISTANCE - ((randNum) % (SPAWN_DISTANCE *2));
		pos.z = (rand() % (SPAWN_DISTANCE *2));
		if (yv > 0) {
			yv = -yv;
		}
	}

	//green wall
	if (randNum > SPAWN_DISTANCE *8 && randNum <= SPAWN_DISTANCE *10) {

		pos.z = -SPAWN_DISTANCE;
		pos.x = SPAWN_DISTANCE - ((randNum) % (SPAWN_DISTANCE *2));
		pos.y = (rand() % (SPAWN_DISTANCE *2));
		if (zv < 0) {
			zv = -zv;
		}
	}

	//blue wall
	if (randNum > SPAWN_DISTANCE *10) {
		pos.z = SPAWN_DISTANCE;
		pos.x = SPAWN_DISTANCE - ((randNum) % (2* SPAWN_DISTANCE));
		pos.y = (rand() % (SPAWN_DISTANCE *2));
		if (zv > 0) {
			zv = -zv;
		}
	}

	pos.x += spawnCenter.x;
	pos.y += spawnCenter.y;
	pos.z += spawnCenter.z;
}

bool Asteroid::move() {
	if (!active) {
		return false;
	}
	pos.x += xv;
	pos.y += yv;
	pos.z += zv;

	//if it goes out of bounds it disappears and goes away
	if (pos.x > CUBE_SIZE || pos.x < -CUBE_SIZE || pos.y >CUBE_SIZE || pos.y < -CUBE_SIZE || pos.z >CUBE_SIZE || pos.z < -CUBE_SIZE) {
		destroy();
		return false;
	}
	return true;

}
Coord Asteroid::getPos() {
	return pos;
}

void Asteroid::destroy() {
	active = false;
}

void Asteroid::drawAsteroid() {
	if (!active) {
		return;
	}
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(ASTEROID_SIZE, 40, 40);
	glTranslatef(-pos.x, -pos.y, -pos.z);
	glPopMatrix();
}