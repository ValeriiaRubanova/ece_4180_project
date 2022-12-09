#pragma once
#include <GL/glut.h>

volatile bool invincible;
GLuint height, width;
void* currentFont;

/**
 * Set the current font.
 *
 * @param font   The font
 */
void setFont(void* font) {
    currentFont = font;
}



//[beginText
/**
 * Setup the state for text.
 */
void beginText() {
    glMatrixMode(GL_PROJECTION);

    // Save the current projection matrix
    glPushMatrix();

    // Make the current matrix the identity matrix
    glLoadIdentity();

    // Set the projection (to 2D orthographic)
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
}
//]beginText

//[endText
/**
 * End the state for text.
 */
void endText() {
    glMatrixMode(GL_PROJECTION);

    // Restore the original projection matrix
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}
//]endText



//[drawString
/**
 * Draw a character string.
 *
 * @param x        The x position
 * @param y        The y position
 * @param string   The character string
 */
void drawString(float x, float y, char* string) {

    beginText();
    glRasterPos2f(x, y);
    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(currentFont, *c);
    }
    endText();

    // Other useful functions:
    //    int glutBitmapWidth(void *font, int character)
    //    int glutBitmapLength(void *font, char *string)
}
//]drawString


bool detectCollision(Coord pos1, int size1, Coord pos2, int size2) {
    //Collision Detection

    double upperx_bound = pos1.x + size1 + size2;
    double lowerx_bound = pos1.x - (size1 + size2);
    double uppery_bound = pos1.y + size1 + size2;
    double lowery_bound = pos1.y - (size1 + size2);
    double upperz_bound = pos1.z + size1 + size2;
    double lowerz_bound = pos1.z - (size1 + size2);

    if (!invincible && pos2.x < upperx_bound && pos2.x > lowerx_bound && pos2.y < uppery_bound && pos2.y > lowery_bound && pos2.z < upperz_bound && pos2.z > lowerz_bound) {
        return true;

    }
    return false;
}

#if DEBUG
void drawColoredCircles() {
    //white: (0,0,0)
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glutSolidSphere(3.0, 20, 20);
    glPopMatrix();

    //Yellow: -x
    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(-10.0, 0.0, 0.0);
    glutSolidSphere(3.0, 20, 20);
    glPopMatrix();

    //Magenta: +x
    glColor3f(1.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(10.0, 0.0, 0.0);
    glutSolidSphere(3.0, 20, 20);
    glPopMatrix();

    //Red: -y
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0, -10.0, 0.0);
    glutSolidSphere(3.0, 20, 20);
    glPopMatrix();

    //Cyan: +y
    glColor3f(0.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0, 10.0, 0.0);
    glutSolidSphere(3.0, 20, 20);
    glPopMatrix();

    //Green: -z
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, -10.0);
    glutSolidSphere(3.0, 20, 20);
    glPopMatrix();

    //Blue: +z
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 10.0);
    glutSolidSphere(3.0, 20, 20);
    glPopMatrix();
}

void drawColoredWalls() {
    //yellow, -x
    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(-CUBE_SIZE, 0.0, 0.0f);
    glRotatef(-90, 0.0, 1.0, 0.0);
    glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
    glPopMatrix();

    //pink, +x
    glColor3f(1.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(CUBE_SIZE, 0.0, 0.0f);
    glRotatef(90, 0.0, 1.0, 0.0);
    glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
    glPopMatrix();

    //red, -y
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0f, -CUBE_SIZE, 0.0f);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
    glTranslatef(0.0f, -CUBE_SIZE, 0.0f);
    glPopMatrix();

    //cyan, +y
    glColor3f(0.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0f, CUBE_SIZE, 0.0f);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
    glTranslatef(0.0f, -CUBE_SIZE, 0.0f);
    glPopMatrix();

    //green, -z
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -CUBE_SIZE);
    glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
    glTranslatef(0.0f, 0.0f, -CUBE_SIZE);
    glPopMatrix();

    //blue, +z
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, CUBE_SIZE);
    glRectf(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
    glTranslatef(0.0f, 0.0f, -CUBE_SIZE);
    glPopMatrix();
}
#endif
