/* Interactive Robot Game
 * CPSC 3710 Final Project
 * Apr. 3, 2019
 * By: Eric Mu, Chris Toth, Jayden Stoll, Qi Guo */

#define GL_GLEXT_PROTOTYPES
#define DISPLAY_KEY_INFO2 "Z: Move Forward | A: Turn Right | Q: Turn Left | P: Pause | O: Toggle Ortho | X: Exit"
#define DISPLAY_KEY_INFO "F1-F3: Turn head | F4: Default LookAt | F5-F12: Change camera position"

#include <stdlib.h>  // Useful for the following includes.
#include <stdio.h>
#include <string.h>  // For string operations.

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

#include "BuildingConf.h"

#include <vector>

using namespace std;

int Window_ID;

int Window_Width = 800;
int Window_Height = 600;

// character starting position variables
enum direction {NEG_Z = 0, POS_X = 1, POS_Z = 2, NEG_X = 3} dir = NEG_Z; // direction the robot body is facing/moving
direction headDir = NEG_Z; // direction the robot head is facing
float charX = 0.0f;
float charY = 0.0f;
float charZ = 0.0f;
float antennaRotate = 30.0;
float antennaSpeed = 30.0;

// camera eye variables
float eyex = -0.5f;
float eyey = 2.0f;
float eyez = 2.0f;

//camera at variables
float atx = 0;
float aty = 1;
float atz = 0;

float xCoord = 2;
float zCoord = -3;

bool paused = 0;
bool ortho = 0;

// Function for string rendering
static void PrintString(void *font, char *str) {
   int i,len=strlen(str);

   for(i=0;i < len; i++)
      glutBitmapCharacter(font,*str++);
}

// helper function to check if robot is standing in an intersection
bool atIntersection(float x, float z) {
  bool xAt = 0;
  bool zAt = 0;

  vector<float> allowedX;
  for (float i = 0; i <= 84; i=i+4.00f) {
    allowedX.push_back(i);
  }
  vector<float> allowedZ;
  for (float i = -1; i >= -84; i=i-4.00) {
    allowedZ.push_back(i);
  }

  for (unsigned int i = 0; i < allowedX.size(); i++) {
    if (x > (allowedX[i] - 0.40) && x < (allowedX[i] + 0.40))
      xAt = true;
  }
  for (unsigned int i = 0; i < allowedZ.size(); i++) {
    if (z > (allowedZ[i] - 0.40) && z < (allowedZ[i] + 0.40))
      zAt = true;
  }
  return xAt && zAt;
}

//******************************************************//
//                   DRAW OBJECTS HERE                  //
//******************************************************//
// single road tile
void drawDashedLine() {
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    // dashed line
    glBegin(GL_QUADS);
        glVertex3f(1.0f,0.0f,4.0f);
        glVertex3f(1.1f,0.0f,4.0f);
        glVertex3f(1.1f,0.0f,3.00f);
        glVertex3f(1.0f,0.0f,3.00f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(1.0f,0.0f,2.50f);
        glVertex3f(1.1f,0.0f,2.50f);
        glVertex3f(1.1f,0.0f,1.50f);
        glVertex3f(1.0f,0.0f,1.50f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(1.0f,0.0f,1.00f);
        glVertex3f(1.1f,0.0f,1.00f);
        glVertex3f(1.1f,0.0f,0.00f);
        glVertex3f(1.0f,0.0f,0.00f);
    glEnd();
}

void drawRoadSurface() {
    glColor3f(0.25,0.25,0.25);
    // road surface
    glBegin(GL_QUADS);
        glVertex3f(0.5f,-0.01f,4.00f);
        glVertex3f(1.7f,-0.01f,4.00f);
        glVertex3f(1.7f,-0.01f,0.00f);
        glVertex3f(0.5f,-0.01f,0.00f);
    glEnd();
    glPopMatrix();
}

void drawOuterRoad() {
    glColor3f(0.25,0.25,0.25);
    // road surface
    glBegin(GL_QUADS);
        glVertex3f(0.0f,-0.02f,4.00f);
        glVertex3f(2.2f,-0.02f,4.00f);
        glVertex3f(2.2f,-0.02f,0.00f);
        glVertex3f(0.0f,-0.02f,0.00f);
    glEnd();
    glPopMatrix();
}

void drawGrass() {
    glColor3f(0.0,0.25,0.0);
    // road surface
    glBegin(GL_QUADS);
        glVertex3f(0.0f,-0.03f,80.00f);
        glVertex3f(80.0f,-0.03f,80.00f);
        glVertex3f(80.0f,-0.03f,0.00f);
        glVertex3f(0.0f,-0.03f,0.00f);
    glEnd();
    glPopMatrix();
}

// Draw street
void drawStreet() {
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);

    glPushMatrix();
    for (float i = 0.0f; i <= 20; i++) {
        glPushMatrix();
        glTranslatef(4*i, 0.0f, 0.0f);
        for (float j = 0.0f; j < 20; j++) {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, 4*j);
            drawDashedLine();
            drawRoadSurface();
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    for (float i = 0.0f; i <= 20; i++) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 4*i);
        glRotatef(90,0.0,1.0,0.0);
        for (float j = 0.0f; j < 20; j++) {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, 4*j);
            drawDashedLine();
            drawRoadSurface();
            glPopMatrix();
        }
        glPopMatrix();
    }
    drawGrass();

    glPushMatrix();
    glColor3f(0.20f,0.25f,0.10f);
    glBegin(GL_QUADS);
        glVertex3f(-180.0f,-0.1f,180.00f);
        glVertex3f(180.0f,-0.1f,180.00f);
        glVertex3f(180.0f,-0.1f,-180.00f);
        glVertex3f(-180.0f,-0.1f,-180.00f);
    glEnd();
    glPopMatrix();

    glPopMatrix();

    // outer roads
    glPushMatrix();
    glTranslatef(0.0f, 0.00f, -4);
    for (float i = 0.0f; i <= 20; i++) {
        glPushMatrix();
        glTranslatef(0.0f, -0.03f, -4*i);
        drawOuterRoad();
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.00f, -4);
    for (float i = 0.0f; i <= 20; i++) {
        glPushMatrix();
        glTranslatef(80.0f, 0.01f, -4*i);
        drawOuterRoad();
        glPopMatrix();
    }
    glPopMatrix();

    for (float i = 0.0f; i < 20; i++) {
        glPushMatrix();
        glTranslatef(4*i, 0.0f, 0.0f);
        glRotatef(90,0.0,1.0,0.0);
        drawOuterRoad();
        glPopMatrix();
    }

    for (float i = 0.0f; i <= 20; i++) {
        glPushMatrix();
        glTranslatef((i*4.0f)-1, 0.0f, -80.0f);
        glRotatef(90,0.0,1.0,0.0);
        drawOuterRoad();
        glPopMatrix();
    }
}

// Draw buildings
void drawBuildings() {
	    glPushMatrix();
	    glTranslatef(2, 0, -3);
    for (int i = 0; i < 80; i+=4) {
	    glPushMatrix();
        for (int j = 0; j < 80; j+=4) {
             glPushMatrix();
	     glTranslatef(j, 0, -i);
	     CreateBlock();
	     //glutSolidCube(1);
             glPopMatrix();
	}
	     glPopMatrix();
    }
	glPopMatrix();
}

// Draw Cube
void drawCube(float size = 0.5f) {
    glBegin(GL_QUADS);

    // Far face.  Green
    glNormal3f( 0.0f, 0.0f,-1.0f);
    glColor4f(0.0, 1.0, 0.0, 0.0); // no opacity

    glTexCoord2f(0.995f, 0.005f); glVertex3f(-size, -size, -size); // TL
    glTexCoord2f(0.995f, 0.995f); glVertex3f(-size,  size, -size); // TR
    glTexCoord2f(0.005f, 0.995f); glVertex3f( size,  size, -size); // BR
    glTexCoord2f(0.005f, 0.005f); glVertex3f( size, -size, -size); // BL

    // Right face.  Blue
    glNormal3f( 1.0f, 0.0f, 0.0f);
    glColor4f(0.0, 0.0, 1.0, 0.0); // no opacity

    glTexCoord2f(0.005f, 0.995f); glVertex3f( size,  size,  size); // TL
    glTexCoord2f(0.005f, 0.005f); glVertex3f( size, -size,  size); // BL
    glTexCoord2f(0.995f, 0.005f); glVertex3f( size, -size, -size); // BR
    glTexCoord2f(0.995f, 0.995f); glVertex3f( size,  size, -size); // TR

    // Front face; offset. Red
    glNormal3f( 0.0f, 0.0f, 1.0f);
    glColor4f( 1.0f, 0.0f, 0.0f, 0.0f);

    glTexCoord2f( 0.995f, 0.005f); glVertex3f( size, -size,  size); // BR
    glTexCoord2f( 0.995f, 0.995f); glVertex3f( size,  size,  size); // TR
    glTexCoord2f( 0.005f, 0.995f); glVertex3f(-size,  size,  size); // TL
    glTexCoord2f( 0.005f, 0.005f); glVertex3f(-size, -size,  size); // BL

    // Left Face; offset.  Yellow
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glColor4f(0.9,0.9,0.2,0.0);

    glTexCoord2f(0.995f, 0.005f); glVertex3f(-size, -size,  size); // BR
    glTexCoord2f(0.995f, 0.995f); glVertex3f(-size,  size,  size); // TR
    glTexCoord2f(0.005f, 0.995f); glVertex3f(-size,  size, -size); // TL
    glTexCoord2f(0.005f, 0.005f); glVertex3f(-size, -size, -size); // BL

    // Top Face. Orange
    glNormal3f(0.0f, 1.0f, 0.0f);
    glColor4f(1.0f, 0.5f, 0.0f, 0.0f);

    glTexCoord2f(0.005f, 0.005f); glVertex3f( -size, size,  size); // front left - BL
    glTexCoord2f(0.995f, 0.005f); glVertex3f( size, size, size); // front right - BR
    glTexCoord2f(0.995f, 0.995f); glVertex3f( size, size,  -size); // rear right - TR
    glTexCoord2f(0.005f, 0.995f); glVertex3f( -size, size, -size); // rear left - TL

    // Bottom Face. Purple
    glNormal3f(0.0f, -1.0f, 0.0f);
    glColor4f(1.0f, 0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.005f, 0.995f); glVertex3f( -size, -size, -size); // rear left - TL
    glTexCoord2f(0.995f, 0.995f); glVertex3f( size, -size, -size); // rear left - TR
    glTexCoord2f(0.995f, 0.005f); glVertex3f( size, -size, size); // front right - BR
    glTexCoord2f(0.005f, 0.005f); glVertex3f( -size, -size, size); // front left - BL

    // All polygons have been drawn.
    glEnd();
}

void drawPyramid(float size = 0.5f) {
glBegin(GL_TRIANGLES);
    //Triangle 1
    glColor3f(1.0f,0.0f,0.0f); // Red face
    glVertex3f( 0.0f, size, 0.0f);   //V0
    glVertex3f(-size,-size, size);   //V1
    glVertex3f( size,-size, size);   //V2
    //Triangle 2
    glColor3f(0.0f,1.0f,0.0f); // Green face
    glVertex3f( 0.0f, size, 0.0f);   //V0
    glVertex3f( size,-size, size);   //V2
    glVertex3f( size,-size,-size);   //V3
    //Triangle 3
    glColor3f(0.0f,0.0f, 1.0f); // Blue face
    glVertex3f( 0.0f, size, 0.0f);   //V0
    glVertex3f( size,-size,-size);   //V3
    glVertex3f(-size,-size,-size);   //V4
    //Triangle 4
    glColor3f(0.5f,0.0f,0.5f);  // Purple face
    glVertex3f( 0.0f, size, 0.0f);   //V0
    glVertex3f(-size,-size,-size);   //V4
    glVertex3f(-size,-size, size);   //V1
    //Triangle 5
    glColor3f(1.0f,1.0f,0.0f);  // Yellow face
    glVertex3f( size, -size, size); //BR
    glVertex3f(-size,-size, -size);   //TL
    glVertex3f(size,-size,-size);  //TR
    //Triangle 6
    glColor3f(1.0f,1.0f,0.0f);  // Yellow face
    glVertex3f( -size, -size, size);   //BL
    glVertex3f(-size,-size, -size);   //TL
    glVertex3f(size,-size, size);   //BR
glEnd();
}

// draws a solid sphere
void drawSphere(float size = 0.5f,
                float r = 0.5f, float g = 0.5f, float b = 0.5f) {
    glColor3f(r, g, b);
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluSphere(quad,size,100,20);
}

// Draw robot
void drawRobot() {
    /*
    * Drawing the robot from the bottom up starting with the body.
    * Adding in parts from the bottom up and translating
    * parts to the proper location.
    */
    glPushMatrix();
    glTranslatef(charX, charY, charZ);
    switch (dir) {
        case NEG_Z:
            glRotatef(0, 0.0, 1.0, 0.0);
            break;
        case POS_X:
            glRotatef(90, 0.0, 1.0, 0.0);
            break;
        case POS_Z:
            glRotatef(180, 0.0, 1.0, 0.0);
            break;
        case NEG_X:
            glRotatef(270, 0.0, 1.0, 0.0);
            break;
    }
    // draw body
    glPushMatrix();
    glTranslatef(0, 0.5f, 0);
    glScalef(0.5f, 1.0f, 0.5f);
    drawCube(0.5f);
    glPopMatrix();

    // draw chest square
    glBegin(GL_POLYGON);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glVertex3f(0.2f, 0.8f, -0.26f);
    glVertex3f(-0.2f, 0.8f, -0.26f);
    glVertex3f(-0.2f, 0.2f, -0.26f);
    glVertex3f(0.2f, 0.2f, -0.26f);
    glEnd();

    // draw back triangles
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    // green indicates OK to turn
    if (atIntersection(charX, charZ))
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    else
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.85f , 0.26f);
    glVertex3f(0.2f, 0.55f, 0.26f);
    glVertex3f(-0.2f, 0.55f, 0.26f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.55f , 0.26f);
    glVertex3f(0.2f, 0.25f, 0.26f);
    glVertex3f(-0.2f, 0.25f, 0.26f);
    glEnd();
    glPopMatrix();

    // draw neck
    glPushMatrix();
    glTranslatef(0.0, 1.0f, 0.0);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.15f, 0.15f, 0.09f, 1.0f, 0.5f, 1.0f);
    glPopMatrix();

    // draw head
    glPushMatrix();
    switch (headDir) {
        case NEG_Z:
            glRotatef(0, 0.0, 1.0, 0.0);
            break;
        case POS_X:
            glRotatef(-90, 0.0, 1.0, 0.0);
            break;
        case POS_Z:
            glRotatef(180, 0.0, 1.0, 0.0);
            break;
        case NEG_X:
            glRotatef(-270, 0.0, 1.0, 0.0);
            break;
    }
    glPushMatrix();
    glTranslatef(0.0f, 1.25f, 0.0f);
    drawCube(0.16f);
    glPopMatrix();

    // draw eyes
    glPushMatrix();
    glTranslatef(-0.07f, 1.26f, -0.18f);
    glutSolidSphere(0.04, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.07f, 1.26f, -0.18f);
    glutSolidSphere(0.04, 20, 20);
    glPopMatrix();

    // draw antenna
    glPushMatrix();
    glTranslatef(0.0, 1.28f, 0.0);
    glRotatef(-90, 1, 0, 0);
    glRotatef(antennaRotate, 0, 0, 1);
    glTranslatef(0.05, 0.0, 0.0);
    drawCylinder(0.05f, 0.05f, 0.3f, 0.0f, 1.0f, 1.0f);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

/**
* Draw lines to help visualize the origin and each axis
* By default,
* Z axis line = green
* X axis line = red
* y axis line = blue
*/
void drawAxisLines(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
    // draw Z axis
    glPushMatrix();
    glBegin(GL_LINES);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // GREEN
    glVertex3f(0.0f, 0.0f, zMin);
    glVertex3f(0.0f, 0.0f, zMax);
    glEnd();
    glPopMatrix();

    // Draw X axis
    glPushMatrix();
    glBegin(GL_LINES);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // RED
    glVertex3f(xMin, 0.0f, 0.0f);
    glVertex3f(xMax, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();

    // Draw Y axis
    glPushMatrix();
    glBegin(GL_LINES);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f); // BLUE
    glVertex3f(0.0f, yMin, 0.0f);
    glVertex3f(0.0f, yMax, 0.0f);
    glEnd();
    glPopMatrix();
}
//******************************************************//
//              CALLBACK FUNCTIONS HERE                 //
//******************************************************//
// mouse bindings
void mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // destroy a building

    } else {

    }
}

// keyboard bindings
void keyboardCallback(unsigned char key, int x, int y) {
  if (paused == 0) {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (key == 'z') { // push the robot forward
        switch (dir) {
	    case NEG_Z:
            if (charZ >= -81) {
                charZ -= 0.2;
                eyex = charX - 0.5;
                eyey = charY + 2.0;
                eyez = charZ + 2.0;
                atz = charZ;
            }
            break;
        case POS_Z:
            if (charZ <= 0) {
                charZ += 0.2;
                eyex = charX - 0.5;
                eyey = charY + 2.0;
                eyez = charZ + 2.0;
                atz = charZ;
            }
            break;
        case NEG_X:
            if (charX <= 81) {
                charX += 0.2;
                eyex = charX - 0.5;
                eyey = charY + 2.0;
                eyez = charZ + 2.0;
                atx = charX;
            }
            break;
        case POS_X:
            if (charX >= -.5) {
                charX -= 0.2;
                eyex = charX - 0.5;
                eyey = charY + 2.0;
                eyez = charZ + 2.0;
                atx = charX;
            }
            break;
        }
    }
    else if (key == 'a') { // turn robot right if at an intersection
      if (atIntersection(charX,charZ)){
        if (dir == 0)
            dir = NEG_X;
        else
            dir = direction((int)dir - 1);
      }
    }
    else if (key == 'q') { // turn robot left if at an intersection
      if (atIntersection(charX,charZ)){
        if (dir == 3)
            dir = NEG_Z;
        else
            dir = direction((int)dir + 1);
      }
    }
    else if (key == 'r') { // return the robot to origin
        charX = 0.0;
        charY = 0.0;
        charZ = 0.0;
        eyex = - 0.5;
        eyey = 2.0;
        eyez = 2.0;
        atx = 0;
        aty = 0;
        atz = 0;
        dir = NEG_Z;
    }
    else if (key == 'o') { // toggle orthographic view
        if (ortho == 1)
          ortho = 0;
        else
          ortho = 1;
    }
  }
    if (key == 'p') { // pause the game
        if (paused == 1) {
          paused = 0;
          antennaSpeed = 30;
        }
        else {
          paused = 1;
          antennaSpeed = 0;
        }
    }
    if (key == 'x'){
        glutDestroyWindow(Window_ID);
        exit(1);
    }
}

// function bindings
void functionCallback(int key, int x, int y) {
  if (paused == 0) {
   if (key == GLUT_KEY_F1){ // turn robot head to face forward (default)
        headDir = dir;
    }
    else if (key == GLUT_KEY_F2) { // HOLD to turn head right
        {
            if (headDir == 3)
                headDir = NEG_Z;
            else
                headDir = direction((int)dir + 1);
        }
    }
    else if (key == GLUT_KEY_F3) { // HOLD to turn head left
        if (headDir == 0)
            headDir = NEG_X;
        else
            headDir = direction((int)dir - 1);
    }
    else if (key == GLUT_KEY_F4) { // return to default LookAt setting
       eyex = charX - 0.5;
       eyey = charY + 2.0;
       eyez = charZ + 2.0;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F5) { // back left LookAt
       eyex = charX + 1;
       eyey = charY + 2;
       eyez = charZ - 1;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F6) { // back right LookAt
        eyex = charX - 1;
       eyey = charY + 2;
       eyez = charZ - 1;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F7) { // front left LookAt
       eyex = charX + 1;
       eyey = charY + 2;
       eyez = charZ + 1;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F8) { // front right LookAt
        eyex = charX - 1;
       eyey = charY + 2;
       eyez = charZ + 1;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F9) { // further back left LookAt
       eyex = charX + 2;
       eyey = charY + 2;
       eyez = charZ - 2;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F10) { // further back right LookAt
        eyex = charX - 2;
       eyey = charY + 2;
       eyez = charZ - 2;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F11) { // further front left LookAt
       eyex = charX + 2;
       eyey = charY + 2;
       eyez = charZ + 2;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
    else if (key == GLUT_KEY_F12) { // further front right LookAt
       eyex = charX - 2;
       eyey = charY + 2;
       eyez = charZ + 2;
       atx = charX;
       aty = charY;
       atz = charZ;
    }
  }
}

void functionUpCallback(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F2: // release F2 : return head facing with body
            headDir = dir;
            break;
        case GLUT_KEY_F3: // release F3
            headDir = dir;
            break;
        default:
            printf("No action for %d", key);
            break;
    }
}

void display(void) {
    char buf[160];

    // set clear color to gray and clear window
    glClearColor(0.2f, 0.4f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Set up ortho view
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

    if (ortho)
        glOrtho(-5.0, 5.0, -5.0, 5.0, -50.0, 50.0);
    else
        gluPerspective(90.0f,(GLfloat)Window_Width/(GLfloat)Window_Height,0.1f,100.0f);

    // Switch to modelview for drawing
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    // Set up camera
	gluLookAt(eyex,eyey,eyez,atx,aty,atz,0.0,1.0,0.0);

	drawAxisLines(-1.0f, 5.0f, -1.0f, 5.0f, -1.0f, 5.0f); // Draw axis lines to help visualize 3D space

    // draw street
    drawStreet();

    // Draw Robot Character
    glPushMatrix();
    drawRobot();
    glPopMatrix();

    glPushMatrix();
    // draw the buildings
    drawBuildings();
    glPopMatrix();

    // Display help string
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
    glColor4f(0.6,1.0,0.6,1.00);

    sprintf(buf,"%s", DISPLAY_KEY_INFO2); 
    glWindowPos2i(3,5);                     
    PrintString(GLUT_BITMAP_HELVETICA_12, buf);  

    sprintf(buf,"Character location: (%.2f,%.2f,%.2f)", charX, charY, charZ); // Print the string into a buffer
    glWindowPos2i(3,580);                         
    PrintString(GLUT_BITMAP_HELVETICA_12, buf); 

    sprintf(buf,"%s", DISPLAY_KEY_INFO); 
    glWindowPos2i(3,20);                     
    PrintString(GLUT_BITMAP_HELVETICA_12, buf); 
    
    // display pause
    if (paused) {
        glColor3f(0.0f, 1.0f, 0.0f);
        sprintf(buf,"GAME IS PAUSED"); 
        glWindowPos2i(300,500);                      
        PrintString(GLUT_BITMAP_TIMES_ROMAN_24, buf);  
    }
    glPopMatrix();

    antennaRotate += antennaSpeed;
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(Window_Width,Window_Height);
	glutInitWindowPosition(0,0);
	Window_ID = glutCreateWindow("Interactive Robot Game");
	glutDisplayFunc(display);
    glutIdleFunc(display);

    glutMouseFunc(mouseCallback);
    glutKeyboardFunc(keyboardCallback);

    //glutKeyboardUpFunc(); // checks for when keys are released
    glutSpecialFunc(functionCallback);
    glutSpecialUpFunc(functionUpCallback); // checks for when Function keys are released

	glEnable(GL_DEPTH_TEST); // enable depth testing
    //glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE); // back face culling enabled
	glutMainLoop();
}
