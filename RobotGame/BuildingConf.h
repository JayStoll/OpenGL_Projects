/**
 * Create buildings to render onto the grid
 * @author Jayden Stoll, Chris Toth
 * @date March 24, 2019
 */

 struct RGBColour {
    GLfloat R, G, B;
};

// create a rectangle
void CreateRect(float height, float width, struct RGBColour rgb);
// create a tall rectangle
void CreateTallBuilding(struct RGBColour rgb);
void CreateShortBuilding(struct RGBColour rgb);

 // Draw cylinder
void drawCylinder(float base = 0.5f, float top = 0.5f, float height = 1.0f,
             float r = 0.5f, float g = 0.0f, float b = 0.5f) {
    glColor3f(r, g, b);
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluCylinder(quad, base, top, height, 100, 20);
}

// this didnt work but im keeping it in anyway
RGBColour RandomColour() {
    return {static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX)};
}

void CreateBlock() {
    glPushMatrix();
    RGBColour rgb = {1.0, 0.0, 0.0};
    RGBColour rgb1 = {1.0, 1.0, 1.0};
    glTranslatef(0, 1.25, 0);
    CreateTallBuilding(rgb);
    glTranslatef(0.7, -1.25, 0.7);
    CreateShortBuilding(rgb1);
    glPushMatrix();
    glTranslatef(-1, 0, -1.70);
    glRotatef(-90, 1, 0, 0);
    drawCylinder();
    glPopMatrix();
    glPopMatrix();
}

 void CreateTallBuilding(struct RGBColour rgb) {
    CreateRect(5, 1, rgb);
}

 void CreateShortBuilding(struct RGBColour rgb) {
    CreateRect(2, 0.50, rgb);
 }

/**
 * used as a controller for the Create(Tall/Short)Building functions
 * @param height, the height of the rectangle
 * @param width, the width of the rectangle
 * @param rgb, the rgb colour of the building
 */
void CreateRect(float height, float width, struct RGBColour rgb) {
    glPushMatrix();
    glColor3f(rgb.R, rgb.G, rgb.B);
    glScalef(width, height, width);
    glutSolidCube(1);
    glPopMatrix();
}
