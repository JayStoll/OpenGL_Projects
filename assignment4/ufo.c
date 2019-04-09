#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Window width and height
int windowID;
int WIDTH = 800;
int HEIGHT = 600;

float rotateDegrees = 0.0;

float eyeX = 1.0f, eyeY = 1.0f, eyeZ = 1.0f;
float zPos = 2.0f, yPos = -2.0f;
int moveDirection = 0; // 0 - dont move, 1 - move up, 2 - move down, 3 - move forward, 4 - move back

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

void DrawPointyThing(float s) {
    glScalef(s, s, s);
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
    // Front
    glColor3f(0.0f, 1.0f, 0.0f);     // Red
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Back
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Left
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid
    
    glTranslatef(0, 1, 0);
    glRotatef(180, 1, 0, 1);
    glScalef(s, s, s);
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
    // Front
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Back
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Left
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid
    glPopMatrix();
}

void DrawT(float w, float h) {
    // draw rect
    glBegin(GL_QUADS);
        glVertex3f( w, h, -1.0f);
        glVertex3f(-w, h, -1.0f);
        glVertex3f(-w, h,  1.0f);
        glVertex3f( w, h,  1.0f);

        glVertex3f( w, -h,  1.0f);
        glVertex3f(-w, -h,  1.0f);
        glVertex3f(-w, -h, -1.0f);
        glVertex3f( w, -h, -1.0f);

        glVertex3f( w,  h, 1.0f);
        glVertex3f(-w,  h, 1.0f);
        glVertex3f(-w, -h, 1.0f);
        glVertex3f( w, -h, 1.0f);
        glVertex3f( w, -h, -1.0f);
        glVertex3f(-w, -h, -1.0f);
        glVertex3f(-w,  h, -1.0f);
        glVertex3f( w,  h, -1.0f);

        glVertex3f(-w,  h,  1.0f);
        glVertex3f(-w,  h, -1.0f);
        glVertex3f(-w, -h, -1.0f);
        glVertex3f(-w, -h,  1.0f);

        glVertex3f(w,  h, -1.0f);
        glVertex3f(w,  h,  1.0f);
        glVertex3f(w, -h,  1.0f);
        glVertex3f(w, -h, -1.0f);
    glEnd();

    // rotate buffer 90 degrees
    glRotatef(90, 1, 0, 0);
    // change the colour of the new rect
    glColor4f(1.0, 1.0, 1.0, 0.0f);
    // draw another rect
    glBegin(GL_QUADS);
        glVertex3f( w, h, -1.0f);
        glVertex3f(-w, h, -1.0f);
        glVertex3f(-w, h,  1.0f);
        glVertex3f( w, h,  1.0f);

        glVertex3f( w, -h,  1.0f);
        glVertex3f(-w, -h,  1.0f);
        glVertex3f(-w, -h, -1.0f);
        glVertex3f( w, -h, -1.0f);

        glVertex3f( w,  h, 1.0f);
        glVertex3f(-w,  h, 1.0f);
        glVertex3f(-w, -h, 1.0f);
        glVertex3f( w, -h, 1.0f);
        glVertex3f( w, -h, -1.0f);
        glVertex3f(-w, -h, -1.0f);
        glVertex3f(-w,  h, -1.0f);
        glVertex3f( w,  h, -1.0f);

        glVertex3f(-w,  h,  1.0f);
        glVertex3f(-w,  h, -1.0f);
        glVertex3f(-w, -h, -1.0f);
        glVertex3f(-w, -h,  1.0f);

        glVertex3f(w,  h, -1.0f);
        glVertex3f(w,  h,  1.0f);
        glVertex3f(w, -h,  1.0f);
        glVertex3f(w, -h, -1.0f);
    glEnd();
    glPopMatrix();
}

void DrawWeirdThing() {
    DrawT(0.5, 0.25);
    DrawPointyThing(0.9);
}

void DrawUFO(float s) {
    // draw the bottom of the ufo
    // set what colour to make the ufo
    glRotatef(-rotateDegrees, 0, 1, 0);  // cancel out any rotation added to the bottom
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    // draw the points in a triangle fan
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, -1, 0);
        glVertex3f(0.0f*s, 0.0f*s, 0.0f*s);
        glVertex3f(1.0f*s, 1.0f*s, 0.0f*s);
        glVertex3f(0.5f*s, 1.0f*s, 1.0f*s);
        glVertex3f(-0.5f*s, 1.0f*s, 1.0f*s);
        glVertex3f(-1.0f*s, 1.0f*s, 0.0f*s);
        glVertex3f(-0.5f*s, 1.0f*s, -1.0f*s);
        glVertex3f(0.5f*s, 1.0f*s, -1.0f*s);
        glVertex3f(1.0f*s, 1.0f*s, 0.0f*s);
    glEnd();
    
    // draw the top of the ufo
    glRotatef(rotateDegrees, 0.0f, 1.0f, 0.0f);
    glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 1, 0);
        glVertex3f(0.0f*s, 2.0f*s, 0.0f*s);      // change to 2 to rotate the point to be up
        glVertex3f(1.0f*s, 1.0f*s, 0.0f*s);
        glVertex3f(0.5f*s, 1.0f*s, 1.0f*s);
        glVertex3f(-0.5f*s, 1.0f*s, 1.0f*s);
        glVertex3f(-1.0f*s, 1.0f*s, 0.0f*s);
        glVertex3f(-0.5f*s, 1.0f*s, -1.0f*s);
        glVertex3f(0.5f*s, 1.0f*s, -1.0f*s);
        glVertex3f(1.0f*s, 1.0f*s, 0.0f*s);
    glEnd();
    glPopMatrix();
 }

void CallBackRenderScreen(void) {
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-5, 5, -5, 5, -5, 5);

    gluLookAt(eyeX, eyeY, eyeZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);


    glMatrixMode(GL_MODELVIEW);
    drawAxisLines(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
    
    glPushMatrix(); 
    // draw polyhedra here
    glTranslatef(3, 2, 0);
    DrawT(0.5, 0.25);
    glTranslatef(-3.75, -2, 0);
    DrawPointyThing(1);
    glTranslatef(0, 2, 3);
    DrawWeirdThing();
    glLoadIdentity();

    // draw the 't' ufo
    glPushMatrix();
    // draw ufo here
    glTranslatef(2.5, 1.5, 0);
    glRotatef(rotateDegrees, 0, 1, 0);
    glTranslatef(2, 0, 0);
    DrawUFO(1);

    // draw the pointy thing ufo
    glPushMatrix();
    glTranslatef(-3.25, -1.5, 0);
    glRotatef(-rotateDegrees, 0, 1, 0);
    glTranslatef(1.5, 0, 0);
    DrawUFO(0.5);

     // draw the controllable ufo
    glPushMatrix();
    glTranslatef(3, yPos, zPos); // move to inital position
    //glRotatef(rotateDegrees, 0, 1, 0); // cancel out the bottom spin
    DrawUFO(0.25);


    // draw the two just sitting and rotating
    glPushMatrix();
    glTranslatef(1, 4, 0);
    glRotatef(rotateDegrees, 0, 1, 0);
    DrawUFO(1.2);
    glTranslatef(0, 0, 0);
    glRotatef(rotateDegrees, 0, 1, 0);
    DrawUFO(0.66);

    glLoadIdentity();
    glutSwapBuffers();
    glFlush();

    switch(moveDirection) {
    case 1:
        yPos += 0.05;
        break;
    case 2:
        yPos -= 0.05;
        break;
    case 3:
        zPos += 0.05;
        break;
    case 4:
        zPos -= 0.05;
         break; 
    }
    rotateDegrees += 5;
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

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

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
            eyeX = -1;
            eyeY = 0.01;
            //eyeZ = 5;
        } else {
            printf("lmb released\n");
            eyeX = 1;
            eyeY = 1;
            eyeZ = 1;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            printf("rmb pressed!\n");
            eyeX = 0.01;
            eyeY = 2;
            eyeZ = 0.01;
        } else {
            printf("rmb released\n");
            eyeX = 1;
            eyeY = 1;
            eyeZ = 1;
        }
        break;
    }
}

void CBKey(unsigned char key, int x, int y) {
    switch(key) {
    case 117:
        if (moveDirection == 1) moveDirection = 2;
        else moveDirection = 1;        
        break;
    case 102:
        if (moveDirection == 3) moveDirection = 4;
        else moveDirection = 3;
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
    windowID = glutCreateWindow("Assignment 4: UFO");
    
    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);
    // Enable culling
    //glFrontFace(GL_CW);      // we draw are shapes clock wise 
    //glEnable(GL_CULL_FACE);  // Enable culling
    //glCullFace(GL_BACK);     // cull the back face

    glutIdleFunc(&CallBackRenderScreen);

    // It's a good idea to know when our window's resized.
    glutReshapeFunc(&CallBackResizeScene);

    // Callback functions
    //glutMouseFunc(&MouseRotate);
    glutKeyboardFunc(&CBKey);

    MyInit(WIDTH, HEIGHT);
    
    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;

}
