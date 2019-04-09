#define DISPLAY_INFO "RMB - zoom out, z - move the cube and camera"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

int windowID;
int WIDTH = 600;
int HEIGHT = 400;

float  lookAtX = 0.0f, lookAtY = 0.0f;
float cubeZLocation = 0.0f;

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
float rotateY = 0.0f;

void MainView() {
    glBegin(GL_QUADS);

    //Multi-colored side - FRONT
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);     // P1 is red
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);      // P2 is green
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);     // P3 is blue
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);    // P4 is purple

    // White side - BACK
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Purple side - RIGHT
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Green side - LEFT
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Blue side - TOP
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Red side - BOTTOM
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glEnd();
}

//////////////////////////////////////////////////////////
// String rendering routine; leverages on GLUT routine. //
//////////////////////////////////////////////////////////
static void PrintString(void *font, char *str) {
    int i,len=strlen(str);

    for(i=0; i < len; i++)
        glutBitmapCharacter(font,*str++);
}

void CallBackRenderScreen(void) {
    char buf[80];

    // Area 1
    glViewport(50, 350, 550, 25);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();

    glOrtho(0,WIDTH,0,HEIGHT,-1.0,1.0);
    glColor4f(0.6,1.0,0.6,1.00);
    sprintf(buf,"%s", DISPLAY_INFO); // Print the string into a buffer
    glRasterPos2i(2,2);                         // Set the coordinate
    PrintString(GLUT_BITMAP_HELVETICA_12, buf);  // Display the string.DisplayText(DISPLAY_INFO);

    glPopMatrix();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_DECAL);

    //glutSwapBuffers(); // having this in multiple spots creates seizure mode

    // Area 2
    glViewport(0, 0, 50, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();

    glOrtho(0,WIDTH,0,HEIGHT,-1.0,1.0);
    glColor4f(0.6,1.0,0.6,1.00);
    sprintf(buf,"%0.1f", cubeZLocation); // Print the string into a buffer
    glRasterPos2i(50,200);                         // Set the coordinate
    PrintString(GLUT_BITMAP_HELVETICA_12, buf);

    glPopMatrix();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glutSwapBuffers();

    // Area 3
    glViewport(50, 0, WIDTH-50, HEIGHT-25);
    glMatrixMode(GL_MODELVIEW);

    glOrtho(1.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f);

    glLoadIdentity();
    // move the cube center to (0, 0, 0)
    glTranslatef(0.0f, 0.0f, 0.0f);

    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // create a LookAt camera
    gluLookAt(lookAtX, lookAtY, cubeZLocation + 5.0f, 0.0f, 0.0f, cubeZLocation, 0.0f, 1.0f, 0.0f);

    glTranslatef(0.0f, 0.0f, cubeZLocation);

    // Rotate around the
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

    // encapsulate the main program to clean this function up
    // also allows for easy to read placement for viewports
    MainView();

    glLoadIdentity();

    glFlush();

    rotateY += sin(15);
}

///////////////////////////////////////////////////////////////
// Callback routine executed whenever the window is resized. //
//////////////////////////////////////////////////////////////
void CallBackResizeScene(int Width, int Height) {
    // Let's not core dump, no matter what.
    if (Height == 0)
        Height = 1;

    glViewport(0, 0, Width, Height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);

    WIDTH  = Width;
    HEIGHT = Height;
}

////////////////////////////////////////////////////////
//   Setup your program before passing the control    //
//   to the main OpenGL event loop.                   //
////////////////////////////////////////////////////////
void MyInit(int Width, int Height) {
    // Color to clear color buffer to.
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    // Depth to clear depth buffer to; type of test.
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);

    // Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
    glShadeModel(GL_SMOOTH);

    // Load up the correct perspective matrix; using a callback directly.
    CallBackResizeScene(Width,Height);
}

void MouseRotate(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            lookAtX = 5;
            lookAtY = 5;
            printf("right m button pressed!\n");
        } else {
            lookAtX = 0;
            lookAtY = 0;
            printf("right m button released\n");
        }
        break;
    }
}

void CBKey(unsigned char key, int x, int y) {
    switch (key) {
    case 122:
        printf("z key pressed - moving camera and cube\n");
        cubeZLocation += 2;
        break;
    }
}

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char **argv) {

    //  Initialize GLUT and process user parameters
    glutInit(&argc, argv);

    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(WIDTH, HEIGHT);

    // Create window
    windowID = glutCreateWindow("Assignment 2: Push");

    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    glutIdleFunc(&CallBackRenderScreen);

    // It's a good idea to know when our window's resized.
    glutReshapeFunc(&CallBackResizeScene);

    // Callback functions
    glutMouseFunc(&MouseRotate);
    glutKeyboardFunc(&CBKey);

    MyInit(WIDTH, HEIGHT);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;

}
