
#include "BlasterShot.h"

BlasterShot::BlasterShot() {
	printf("Blaster shot constructor");
	active = false;
	xv = 0;
	yv = 0;
	zv = 0;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
}

bool BlasterShot::move() {
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
Coord BlasterShot::getPos() {
	return pos;
}

void BlasterShot::destroy() {
	active = false;
}

bool BlasterShot::isActive() {
	return active;
}

void BlasterShot::draw() {
	if (!active) {
		return;
	}

	glColor3f(0.278, 0.035, 0.670);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(BLASTER_SIZE, 40, 40);
	glTranslatef(-pos.x, -pos.y, -pos.z);
	glPopMatrix();
}

void BlasterShot::fire(Coord startPosition, Coord direction) {
	pos = startPosition;
	xv = direction.x / CUBE_SIZE * BLASTER_SHOT_VELOCITY;
	yv = direction.y / CUBE_SIZE * BLASTER_SHOT_VELOCITY;
	zv = direction.z / CUBE_SIZE * BLASTER_SHOT_VELOCITY;
	active = true;
}