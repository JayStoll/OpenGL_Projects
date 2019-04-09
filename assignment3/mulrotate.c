#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Window width and height
int windowID;
int WIDTH = 600;
int HEIGHT = 400;

float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 0.0f;

float rotateAngle = 0.0f;

// 1 = true, 0 = false
int cubeCanRotate    = 1;
int sphereCanRotate  = 1;
int pyramidCanRotate = 1;

void RotateShape() {
    // rotate around z-axis at a speed of rotateAngle
    glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
}

void CreateCube() {
    if (cubeCanRotate == 1) RotateShape();

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

void CreateSphere() {
    // inital translate to move sphere to position
    glTranslatef(0.0f, 0.0f, -2.0f);
    // set rotation around z axis
    if (sphereCanRotate == 1) {
        glTranslatef(1, 1, 0);
        RotateShape();
        glTranslatef(-1, -1, 0);
    }
    glutSolidSphere(1, 20, 50);
}

void CreatePyramid() {
    if (pyramidCanRotate == 1) RotateShape();

    glTranslatef(0.0f, 0.0f, -2.0f);
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
    // Front
    glColor3f(0.0f, 1.0f, 0.0f);     // Red
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Back
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Left
    glColor3f(1.0f,0.0f,0.0f);       // Red
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glColor3f(0.0f,0.0f,1.0f);       // Blue
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glColor3f(0.0f,1.0f,0.0f);       // Green
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid
}

void CallBackRenderScreen(void) {
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    // create lookat camera
    gluLookAt(eyeX, eyeY, eyeZ,
              0.0f, 0.0f, -1.0f,
              0.0f, 1.0f, 0.0f);

    // draw shapes here
    CreateCube();
    CreatePyramid();
    CreateSphere();

    glLoadIdentity();

    glutSwapBuffers();
    glFlush();

    // add 15 degrees to the spin
    rotateAngle += sin(15);
}

///////////////////////////////////////////////////////////////
// Callback routine executed whenever the window is resized. //
//////////////////////////////////////////////////////////////
void CallBackResizeScene(int width, int height) {
    if (height ==0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // create a viewport spanning the entire given width and height
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // setup orthographic projection based on window size
    if (width >= height)
        glOrtho(-3.0 * aspect, 3.0 * aspect, -3.0, 3.0, 0.1, 100);
    else
        glOrtho(-3.0, 3.0, -3.0 / aspect, 3.0 / aspect, 0.1, 100);

    WIDTH = width;
    HEIGHT = height;
}

////////////////////////////////////////////////////////
//   Setup your program before passing the control    //
//   to the main OpenGL event loop.                   //
////////////////////////////////////////////////////////
void MyInit(int Width, int Height) {
    // Color to clear color buffer to.
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    glEnable(GL_DEPTH_TEST);

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
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            printf("lmb pressed!\n");
            eyeX = -5;
            eyeY = 5;
            eyeZ = 5;
        } else {
            printf("lmb released\n");
            eyeX = 0;
            eyeY = 0;
            eyeZ = 0;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            printf("rmb pressed!\n");
            eyeX = 5;
            eyeY = 5;
            eyeZ = 5;
        } else {
            printf("rmb released\n");
            eyeX = 0;
            eyeY = 0;
            eyeZ = 0;
        }
        break;
    }
}

void CBKey(unsigned char key, int x, int y) {
    switch (key) {
    case 122: // z key
        pyramidCanRotate = 0;
        break;
    case 97: // a key
        // only the pyramid and sphere rotate
        cubeCanRotate = 0;
        break;
    case 113: // q key
        cubeCanRotate    = 0;
        sphereCanRotate  = 0;
        pyramidCanRotate = 0;
        break;
    case 32: // space key
        cubeCanRotate    = 1;
        sphereCanRotate  = 1;
        pyramidCanRotate = 1;
        break;
    }
}

int main(int argc, char **argv) {

    //  Initialize GLUT and process user parameters
    glutInit(&argc, argv);

    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(WIDTH, HEIGHT);

    // Create window
    windowID = glutCreateWindow("Assignment 3: mulrotate");

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
