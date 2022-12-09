/*

*/
#pragma once
#include "Spaceship.h"
//#define M_PI       3.14159265358979323846   // pi

using namespace std;

extern mutex invincibleMutex;
extern volatile bool invincible;
extern vector<thread> invincibleThread;
extern Serial_Com serial_com;

//default constructor
Spaceship::Spaceship() {
    pos.x = 0.0;
    pos.y = 0.0;
    pos.z = 0.0;

    //Start facing the -y direction
    dir.x = 1.0;
    dir.y = -CUBE_SIZE;
    dir.z = 0.0;

    lives = 3;
    
}


//this doesnt actually draw the spaceship, it just changes the camera for a first person POV
void Spaceship::drawSpaceship() {

    //set camera correctly
    gluLookAt(pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, 0.0, 0.0, 1.0);
}

//moves spaceship based on user input
void Spaceship::move() {
    pos.x += dir.x / CUBE_SIZE * SPACESHIP_VELOCITY;
    pos.y += dir.y / CUBE_SIZE * SPACESHIP_VELOCITY;
    pos.z += dir.z / CUBE_SIZE * SPACESHIP_VELOCITY;

    if (pos.x > CUBE_SIZE || pos.x < -CUBE_SIZE || pos.y > CUBE_SIZE || pos.y < -CUBE_SIZE || pos.z > CUBE_SIZE || pos.z < -CUBE_SIZE) {
        spaceshipDies();
    }
}




//function that the invincible threads use, enters invincible phase for 5 seconds
void invincibleTimer() {
    invincible = true;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    invincible = false;
}

//spaceship dies
void Spaceship::spaceshipDies() {
    lives--;
    //become invincible for a few seconds
    invincibleThread.push_back(std::thread(invincibleTimer));
    pos.x = 0; 
    pos.y = 0;
    pos.z = 0;
    dir.x = 0;
    dir.y = -CUBE_SIZE;
    dir.z = 0;
    glutPostRedisplay();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

}

//exits game, closes all threads
void Spaceship::exitGame() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //join invincible threads
    for (int i = 0; i < invincibleThread.size(); i++) {
        if (invincibleThread.at(i).joinable()) {
            invincibleThread.at(i).join();
        }
    }

    glutPostRedisplay();
}

void Spaceship::updateDirection() {
    dir.x = serial_com.xdir;
    dir.y = serial_com.ydir;
    dir.z = serial_com.zdir;
}

BlasterShot* Spaceship::getBlasterShot(int i) {
    if (i >= 0 && i < NUM_BLASTER_SHOTS) {
        return &blaster_shots[i];
    }
    return NULL;
}

void Spaceship::fire() {
    for (int i = 0; i < NUM_BLASTER_SHOTS; i++) {
        if (!blaster_shots[i].isActive()) {
            blaster_shots[i].fire(pos, dir);
            break;
        }
    }
}