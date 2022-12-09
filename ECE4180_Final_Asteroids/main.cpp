/*
Description:
    main function for a 3D Asteroids
*/

#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>
#include "ECE_Bitmap.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <mutex>
#include "Serial_Com.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include "util.h"

using namespace std;
//Establishing globals
mutex leaveGateMutex;
mutex mazeMutex;
Spaceship spaceship;
Serial_Com serial_com;
vector<Asteroid> asteroids;
vector<thread> invincibleThread;
volatile int score;
volatile bool keypressed;
volatile bool data_out_flag;
volatile char data_out;
int highscoreVals[5];
char highscoreNames[5][4];
bool gameover;
bool highscore;
volatile bool initialsentered;
char initials[4];



void removeInactiveAsteroids() {
    if (asteroids.size() != 0) {
        for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
            if (!(*it).getActive())
            {
                if (asteroids.size() > 1) {
                    if (it == asteroids.begin()) {
                        asteroids.erase(it);
                        it = asteroids.begin();
                    }
                    else {
                        asteroids.erase(it--);
                    }

                }
                else {
                    asteroids.erase(it);
                    break;
                }
            }
        }
    }
}


void Keyboard(unsigned char key, int x, int y) {
    static int initial_num = 0;
    if (highscore) {
        printf("keyboard input and highscore %d\n", initial_num);
        if (initial_num < 3) {
            
            initials[initial_num] = key;
            initials[initial_num+1] = '\0';
            initial_num++;
            if (initial_num == 3) {
                initialsentered = true;
            }
        }
    }
    glutPostRedisplay();
}

//initializes graphics, specular lighting
void init(void)
{

    srand(time(0));
    invincible = false;
    gameover = false;
    highscore = false;
    initialsentered = false;
    score = 0;
    initials[0] = 'A';
    initials[1] = '\0';

    serial_com.start();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Specular Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    setFont(GLUT_BITMAP_TIMES_ROMAN_24);
    
}

//reshapes the screen
void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, (GLfloat)w / (GLfloat)h, 10.0, CUBE_SIZE *3);
    glMatrixMode(GL_MODELVIEW);

}

//displays, draws maze and sets up camera
void display(void) {
    char buffer1[33];

    if (highscore) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glLoadIdentity();
            char text1[40] = "High Score! Please Enter Initials\0";
            drawString(width / 2 - 150, 3 * height / 4 + 50, text1);
            drawString(width / 2 - 50, 3 * height / 4 - 50, initials);
        if (initialsentered) {
            data_out_flag = true;
            highscore = false;
        }

        glLoadIdentity();

        glutSwapBuffers();
        glutPostRedisplay();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return;
    }else if (gameover & keypressed) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        serial_com.stop = true;
        serial_com.getThread()->join();
        exit(0);
    }else if (gameover & !keypressed) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glColor3f(1.0, 1.0, 1.0);
        char text1[26] = "Game Over\0";
        drawString(width / 2 - 50, 3 * height / 4 + 50, text1);
        char text2[14] = "Final Score:\0";
        drawString(width / 2 - 50, 3 * height / 4, text2);
        char buffer1[33];

        _itoa_s(score, buffer1, 10);
        drawString(width / 2 -25, 3 * height / 4 - 50, buffer1);

        char text3[26] = "Press Any Button to Exit\0";
        drawString(width / 2 -100, 3 * height / 4 -100, text3);

        glLoadIdentity();

        glutSwapBuffers();
        glutPostRedisplay();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        
        return;
        
    }else if (!keypressed) {
        //dispaly start screen showing high scores
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glColor3f(1.0, 1.0, 1.0);
        char text1[27] = "Press Any Button To Begin\0";
        drawString(width / 2 - 50, 3 * height / 4 + 50, text1);

        if (GET_HIGH_SCORES) {
            char text2[15] = "High Scores: \0";
            drawString(width / 2 - 50, 3 * height / 4, text2);

            drawString(width / 2 - 50, 3 * height / 4 - 50, highscoreNames[0]);

            _itoa_s(highscoreVals[0], buffer1, 10);
            drawString(width / 2 + 50, 3 * height / 4 - 50, buffer1);

            drawString(width / 2 - 50, 3 * height / 4 - 100, highscoreNames[1]);
            _itoa_s(highscoreVals[1], buffer1, 10);
            drawString(width / 2 + 50, 3 * height / 4 - 100, buffer1);

            drawString(width / 2 - 50, 3 * height / 4 - 150, highscoreNames[2]);
            _itoa_s(highscoreVals[2], buffer1, 10);
            drawString(width / 2 + 50, 3 * height / 4 - 150, buffer1);

            drawString(width / 2 - 50, 3 * height / 4 - 200, highscoreNames[3]);
            _itoa_s(highscoreVals[3], buffer1, 10);
            drawString(width / 2 + 50, 3 * height / 4 - 200, buffer1);

            drawString(width / 2 - 50, 3 * height / 4 - 250, highscoreNames[4]);
            _itoa_s(highscoreVals[4], buffer1, 10);
            drawString(width / 2 + 50, 3 * height / 4 - 250, buffer1);
        }
        glLoadIdentity();

        glutSwapBuffers();
        glutPostRedisplay();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        return;
    }

    //move asteroids
    if (asteroids.size() != 0) {
        for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
            (*it).move();
        }
    }

    removeInactiveAsteroids();

    //move blasteers
    for (int i = 0; i < NUM_BLASTER_SHOTS; i++) {
        if (spaceship.getBlasterShot(i)->isActive()) {
            spaceship.getBlasterShot(i)->move();
       }
    }

    //detect collisions with asteroids
    if(asteroids.size() != 0){
        Coord asteroid_coord;
        bool collision;
        for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
            asteroid_coord = (*it).getPos();
            //detect collision with spaceship
            if (detectCollision(spaceship.getPos(), SPACESHIP_SIZE, asteroid_coord, ASTEROID_SIZE)) {
                //collision occurs
                data_out |= COLLISION;
                data_out_flag = true;
                spaceship.spaceshipDies();
                asteroids.clear();
                break;
            }

            for (int i = 0; i < NUM_BLASTER_SHOTS; i++) {
                if (spaceship.getBlasterShot(i)->isActive()) {
                    if (detectCollision(asteroid_coord, ASTEROID_SIZE, spaceship.getBlasterShot(i)->getPos(), BLASTER_SIZE)) {
                        //asteroid was shot
                        spaceship.getBlasterShot(i)->destroy();
                        (*it).destroy();
                        score++;
                        data_out |= ASTEROID_SHOT;
                        data_out_flag = true;
                    }
                }
            }
        }
    }

    //if dead, exit game
    if (spaceship.getLives() <= 0) {
        data_out |= GAME_OVER;
        data_out_flag = true;
        spaceship.exitGame();

        //stop game
        gameover = true;
        keypressed = false;
        if (score > highscoreVals[4]) {
            highscore = true;
        }
    }



    //spawn new asteroids
    static int cnt = 0;
    cnt++;
    if (cnt % 50 == 0) {
        Asteroid asteroid(spaceship.getPos());
        asteroids.push_back(asteroid);
    }

    //Update direction from mbed serial communication
    spaceship.updateDirection();
    //If thrusters are on, move
    if (serial_com.thrust_button) {
        spaceship.move();
    }

    //if fire button presses, fire
    static int firecooldown = 0;
    if (firecooldown > 0) {
        firecooldown--;
    }
    if (serial_com.fire_button && firecooldown ==0) {
        firecooldown = 10;
        spaceship.fire();
        data_out |= SHOT_FIRED;
        data_out_flag = true;
    }


    //Draw spaceship and asteroids
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    spaceship.drawSpaceship();
    for (auto it = asteroids.begin(); it != asteroids.end(); ++it) { (*it).drawAsteroid(); }
    //move blasteers
    for (int i = 0; i < NUM_BLASTER_SHOTS; i++) {
        if (spaceship.getBlasterShot(i)->isActive()) {
            spaceship.getBlasterShot(i)->draw();
        }
    }



    
#if DEBUG
    //drawColoredCircles();
    drawColoredWalls();
#endif
    //TODO: Draw walls if spaceship is close to them (debug this isnt working)
    if (spaceship.getPos().x < -CUBE_SIZE + 50) {
        glColor3f(1.0, 1.0, 0.0);
        glPushMatrix();
        glTranslatef(-CUBE_SIZE, 0.0, 0.0f);
        glRotatef(-90, 0.0, 1.0, 0.0);
        glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
        glPopMatrix();
    }
    else if (spaceship.getPos().x > CUBE_SIZE - 50) {
        glColor3f(1.0, 0.0, 1.0);
        glPushMatrix();
        glTranslatef(CUBE_SIZE, 0.0, 0.0f);
        glRotatef(90, 0.0, 1.0, 0.0);
        glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
        glPopMatrix();
    }
    else if (spaceship.getPos().y < -CUBE_SIZE + 50) {
        glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(0.0f, -CUBE_SIZE, 0.0f);
        glRotatef(90, 1.0, 0.0, 0.0);
        glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
        glTranslatef(0.0f, -CUBE_SIZE, 0.0f);
        glPopMatrix();
    }
    else if (spaceship.getPos().y > CUBE_SIZE - 50) {
        glColor3f(0.0, 1.0, 1.0);
        glPushMatrix();
        glTranslatef(0.0f, CUBE_SIZE, 0.0f);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
        glTranslatef(0.0f, -CUBE_SIZE, 0.0f);
        glPopMatrix();
    }
    else if (spaceship.getPos().z < -CUBE_SIZE + 50) {
        glColor3f(0.0, 1.0, 0.0);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -CUBE_SIZE);
        glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
        glTranslatef(0.0f, 0.0f, -CUBE_SIZE);
        glPopMatrix();
    }
    else if (spaceship.getPos().z > CUBE_SIZE - 50) {
        glColor3f(0.0, 0.0, 1.0);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, CUBE_SIZE);
        glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
        glTranslatef(0.0f, 0.0f, -CUBE_SIZE);
        glPopMatrix();
    }
    glLoadIdentity();

    glutSwapBuffers();
    glutPostRedisplay();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}


int main(int argc, char** argv)
{  
    keypressed = false;
    // GLUT Window Initialization:
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);

    glutCreateWindow("Asteroids");

    // Initialize OpenGL graphics state and other modules
    init();

    // Register callbacks:
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();

    for (thread& th : invincibleThread) {
        th.join();
    }
    serial_com.getThread()->join();

    return 0;
}