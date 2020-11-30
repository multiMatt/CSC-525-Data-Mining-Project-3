﻿/*==================================================================================================
 COURSE:                CSC 525/625
 ASSIGNMENT:            Project 3
 DATE:                  12/7/20
 TEAM MEMBERS (CONTRIBUTION PERCENTAGE; SUBTASKS):
 -David Engleman        ()
 -Matthew Glenn         (16.67%; created code)
 -Benjamin Pottebaum    ()
 -Niko Robbins          (16.67%; created code)
 -Tristan Tyler         (16.67%; created code)
 -Alicia Willard        ()
 DESCRIPTION:           An interactive 3D rendering of Spongebob's neighborhood in Bikini Bottom
                        where all residents are advertising to sell their houses. The user can move
                        around using keyboard input.
 NOTES:                 
 FILES:                 project3_team1.cpp, labProject.sln, freeglut.dll, glut.h, freeglut.lib
 IDE/COMPILER:          Microsoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
    1.      Double click on labProject.sln      to OPEN the project
    2.      Press Ctrl+F7                       to COMPILE
    3.      Press Ctrl+F5                       to EXECUTE
    4.      Press and hold the W key            to MOVE forward
    OR      Press and hold the A key            to MOVE left
    OR      Press and hold the S key            to MOVE back
    OR      Press and hold the D key            to MOVE right
    OR      Press the spacebar                  to JUMP
    5.      Press and hold the 🡹 key           to LOOK up
    OR      Press and hold the 🡸 key           to LOOK left
    OR      Press and hold the 🡻 key           to LOOK down
    OR      Press and hold the 🡺 key           to LOOK right
    6.      Press the 'Escape' key              to TERMINATE the program
==================================================================================================*/
#include <iostream>
#include <vector>
#include <string>
#include <GL/glut.h>
#include <math.h>

using namespace std;

// Actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, y = 1.0f, z = 5.0f;
// Angle of camera
float angle = 0.0f;
// Camera x sensitivity
float x_sens = 0.03f;
// Camera y sensitivity
float y_sens = 0.02f;
// Camera move speed
float speed = 0.25f;

// Movement variables
bool is_left, is_right, is_forward, is_backward, is_look_left, is_look_right, is_look_down, is_look_up = false;
bool is_jumping = false;
bool on_ground = true;
int jump_energy, energy_max = 25;
double jump_force = 0.053;
double jump_speed = 0.15;

// Plant variables
vector<vector<vector<float>>> plantMatrix;
unsigned int number_of_plants = 200;

// Text and font Variables
std::string draw_text;
void* helvetica = GLUT_BITMAP_HELVETICA_18;
void* roman = GLUT_BITMAP_TIMES_ROMAN_24;
void* font = GLUT_STROKE_ROMAN;

// Color and alpha key variable
int blackColor[3] = { 0, 0, 0 };

// Window variables
int windowWidth = 1000;
int windowHeight = 600;

// Prototypes
void changeSize(int w, int h);
void mainDisplayCallback(void);
void drawStuff();
void drawPatrickHouse();
void drawSquidwardHouse();
void drawSpongebobHouse();
void timerCallback(int value);
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialDown(int key, int x, int y);
void specialUp(int key, int x, int y);
void cameraHandler();
void renderText();
void drawText(std::string text, int x, int y, int rgb[3], void* font);
void drawLeaf(float size, float rotx, float roty, float rotz, int seed);
void drawPlantLeaf(float size, float rotx, float roty, float rotz, int parent_index, int index);
void drawPlant(float size, int index);
void createPlant(int seed);
void drawRock(GLfloat angle, double size, GLint smoothness);


int main(int argc, char** argv) {
    // Initialize GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Project 3 - Team 1");

    // Register callbacks
    glutDisplayFunc(mainDisplayCallback);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutTimerFunc(8, timerCallback, 123);

    // OpenGL initilization
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.047f, 0.64f, 1.0f, 0);  // Background/sky color

    // Plant initilization
    for (size_t i = 0; i < number_of_plants; i++)
        createPlant(i * rand());

    glutMainLoop();
    return 1;
}


void timerCallback(int value) {
    mainDisplayCallback();
    glutTimerFunc(8, timerCallback, 123);
}


void drawPatrickHouse() {
    glColor3ub(102, 18, 40);
    glutSolidSphere(5, 20, 20);
}


void drawSquidwardHouse() {
    GLUquadricObj* baseObj = gluNewQuadric();
    gluQuadricDrawStyle(baseObj, GLU_FILL);

    // Base of house
    glPushMatrix();
        glColor3ub(8, 33, 93);
        glRotatef(-90.0f, 1.0f, 0, 0);
        gluCylinder(baseObj, 5, 4.0f, 15, 15, 15);
    glPopMatrix();

    // Left ear
    glPushMatrix();
        glColor3ub(37, 53, 103);
        glTranslatef(4.8f, 7.5f, 0.0f);
        glRotatef(4.0f, 0, 0, 1.0f);
        glScalef(1.2f, 5.0f, 1.5f);
        glutSolidCube(1);
    glPopMatrix();

    // Right ear
    glPushMatrix();
        glColor3ub(37, 53, 103);
        glTranslatef(-4.8f, 7.5f, 0.0f);
        glRotatef(-4.0f, 0, 0, 1.0f);
        glScalef(1.2f, 5.0f, 1.5f);
        glutSolidCube(1);
    glPopMatrix();

    // Door
    glPushMatrix();
        glColor3ub(175, 128, 58);
        glTranslatef(0.0f, -1.0f, 4.75f);
        glRotatef(-90.0f, 0, 1.0f, 0.0f);
        glScalef(0.2f, 2.0f, 0.8f);
        glutSolidSphere(2.3f, 30, 30);
    glPopMatrix();

    // Nose
    glPushMatrix();
        glColor3ub(37, 53, 103);
        glTranslatef(0, 8.0f, 4.1f);
        glScalef(1.8f, 7.0f, 2.5f);
        glutSolidCube(1);
    glPopMatrix();

    // Eyebrow
    glPushMatrix();
        glColor3ub(37, 53, 89);
        glTranslatef(0, 11.0f, 4.2f);
        glRotatef(90.0f, 0, 0.0f, 1.0f);
        glScalef(1.8f, 7.0f, 2.5f);
        glutSolidCube(1);
    glPopMatrix();

    // Left eye
    glPushMatrix();
        glColor3ub(37, 53, 140);
        glTranslatef(2.0f, 8.5f, 4.0f);
        glRotatef(5.0f, 0, 1.0f, 0.0f);
        glScalef(1.0f, 1.0f, 2.0f);
        glutSolidTorus(0.25,0.95,20,20);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(40, 60, 170);
        glTranslatef(2.0f, 8.5f, 4.0f);
        glRotatef(15.0f, 0, 1.0f, 0.0f);
        gluDisk(baseObj,0,1,20,20);
    glPopMatrix();

    // Right Eye
    glPushMatrix();
        glColor3ub(37, 53, 140);
        glTranslatef(-2.0f, 8.5f, 4.0f);
        glRotatef(-5.0f, 0, 1.0f, 0.0f);
        glScalef(1.0f, 1.0f, 2.0f);
        glutSolidTorus(0.25, 0.95, 20, 20);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(40, 60, 170);
        glTranslatef(-2.0f, 8.5f, 4.0f);
        glRotatef(-15.0f, 0, 1.0f, 0.0f);
        gluDisk(baseObj, 0, 1, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(baseObj);
}


void drawLeaf(float size, float rotx, float roty, float rotz, int seed) {
    std::srand(seed);
    glPushMatrix();
        glColor3ub(150 - (rand() % 25), 215 - (rand() % 25), 40 - (rand() % 25));
        glTranslatef(0.0f, 10.0f, 0.0f);
        glRotatef(rotx, 1.0f, 0.0f, 0.0f);
        glRotatef(roty, 0.0f, 1.0f, 0.0f);
        glRotatef(rotz, 0.0f, 0.0f, 1.0f);
        glScalef(.7f, 5.0f, 0.7f);
        glutSolidSphere(size, 10, 10);
    glPopMatrix();
}


void drawSpongebobHouse() { 
    GLUquadricObj* baseObj = gluNewQuadric();
    gluQuadricDrawStyle(baseObj, GLU_FILL);

    // Structure
    glPushMatrix();
        glColor3ub(190, 60, 35);
        glTranslatef(0.0f, 1.0f, 0.0f);
        glScalef(1.0f, 2.0f, 1.0f);
        glutSolidSphere(5, 30, 30);
    glPopMatrix();

    // Door
    glPushMatrix();
        glColor3ub(100, 130, 200);
        glTranslatef(0.0f, -1.0f, 4.75f);
        glRotatef(-90.0f, 0, 1.0f, 0.0f);
        glScalef(0.2f, 2.0f, 0.8f);
        glutSolidSphere(2.3f, 30, 30);
    glPopMatrix();

    // Window 1
    glPushMatrix();
        glColor3ub(37, 53, 140);
        glTranslatef(-2.1f, 6.5f, 3.7f);
        glRotatef(-20.0f, 2.5f, 2.5f, 0.0f);
        glScalef(1.0f, 1.0f, 1.0f);
        glutSolidTorus(0.25, 0.95, 20, 20);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(40, 60, 170);
        glTranslatef(-2.1f, 6.5f, 3.75f);
        glRotatef(-20.0f, 3.5f, 5.5f, 0.0f);
        gluDisk(baseObj, 0, 1, 20, 20);
    glPopMatrix();

    // Window 2
    glPushMatrix();
        glColor3ub(37, 53, 140);
        glTranslatef(2.7f, 2.5f, 4.35f);
        glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
        glScalef(1.0f, 1.0f, 1.0f);
        glutSolidTorus(0.25, 0.95, 20, 20);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(40, 60, 170);
        glTranslatef(2.7f, 2.5f, 4.50f);
        glRotatef(30.0f, 0, 1.0f, 0);
        gluDisk(baseObj, 0, 1, 20, 20);
    glPopMatrix();

    // Leaf
    drawLeaf(1.5, 0, 0, 0, 1);
    drawLeaf(1.5, 30.0f, 0, 0, 2);
    drawLeaf(1.5, -30.0f, 0, 0, 3);
    drawLeaf(1.5, 0, 0, 30.0f, 4);
    drawLeaf(1.5, 0, 0, -30.0f, 5);
    drawLeaf(1.5, 24.0f, 0, 30.0f, 6);
    drawLeaf(1.5, -24.0f, 0, -30.0f, 7);
    drawLeaf(1.5, 24.0f, 0, -30.0f, 8);
    drawLeaf(1.5, -24.0f, 0, 30.0f, 9);
}


void drawSign(string text) {
    glPushMatrix();
        glColor3ub(50, 50, 50);
        glTranslatef(0.0f, -0.5f, 0.0f);
        glScalef(0.3f, 3.5f, 0.3f);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(170, 140, 65);
        glTranslatef(0.0f, 1.0f, 0.05f);
        glScalef(1.75f, 0.5f, 0.3f);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(0, 0, 0);
        glLineWidth(3);
        glTranslatef(-0.8f, 0.85f, 0.22f);
        glScalef(0.003f, 0.003f, 0.3f);
        for (size_t i = 0; i < text.length(); i++)
            glutStrokeCharacter(font, text[i]);
    glPopMatrix();
}


void mainDisplayCallback(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cameraHandler();
    drawStuff();
    renderText();
    glutSwapBuffers();
}


void drawStuff() {
    // Reset transformations
    glLoadIdentity();

    // Set the camera
    if (is_jumping)
        gluLookAt(x, y, z, x + lx, y, z + lz, 0.0f, 1.0f, 0.0f);
    else
        gluLookAt(x, 1.0f, z, x + lx, y, z + lz, 0.0f, 1.0f, 0.0f);    

    // Draw ground
    glColor3ub(249, 213, 187);
    glBegin(GL_QUADS);
        glVertex3f(-1000.0f, 0.0f, -1000.0f);
        glVertex3f(-1000.0f, 0.0f, 1000.0f);
        glVertex3f(1000.0f, 0.0f, 1000.0f);
        glVertex3f(1000.0f, 0.0f, -1000.0f);
    glEnd();

    // Draw road
    glColor3ub(160, 160, 160);
    glBegin(GL_QUADS);
        glVertex3f(-1000.0f, 0.01f, -4.0f);
        glVertex3f(-1000.0f, 0.01f, 4.0f);
        glVertex3f(1000.0f, 0.01f, 4.0f);
        glVertex3f(1000.0f, 0.01f, -4.0f);
    glEnd();
    
    // Draw Patrick's house
    glPushMatrix();
        glTranslatef(-15, 0, -40);
        drawPatrickHouse();
    glPopMatrix();

    // Draw Squidward's house
    glPushMatrix();
        glTranslatef(5, 0, -40);
        drawSquidwardHouse();
    glPopMatrix();

    // Draw Spongebob's house
    glPushMatrix();
        glTranslatef(25, 0, -40);
        drawSpongebobHouse();
    glPopMatrix();

    // "For Sale" signs
    glPushMatrix();
        glTranslatef(-15, 0, -10);
        drawSign("For Sale");
    glPopMatrix();
    glPushMatrix();
        glTranslatef(5, 0, -10);
        drawSign("For Sale");
    glPopMatrix();
    glPushMatrix();
        glTranslatef(25, 0, -10);
        drawSign("For Sale");
    glPopMatrix();
    glPushMatrix();
        glTranslatef(25, 0, -10);
        drawSign("For Sale");
    glPopMatrix();

    // Plants
    for (size_t i = 0; i < number_of_plants; i++) {
        glPushMatrix();
            glTranslatef(plantMatrix[i][1][0], 0, plantMatrix[i][1][1]);
            drawPlant(0.25, i);
        glPopMatrix();
    }

    // Rocks
    glPushMatrix();
        glTranslatef(35, 0, 10);
        drawRock(0, 0.33, 5);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-25, 0, 5.25);
        drawRock(20.0f, 0.5, 8);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-1, 0, -40);
        drawRock(40.0f, 0.33, 5);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(18.5, 0, -20);
        drawRock(60.0f, 0.25, 5);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(18, 0, -18);
        drawRock(80.0f, 0.33, 5);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(11.5, 0, -50);
        drawRock(100.0f, 0.25, 8);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-8, 0, -5);
        drawRock(120.0f, 0.25, 5);
    glPopMatrix();
}


void keyDown(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);

    if (key == 32) {
        if (!is_jumping) {
            is_jumping = true;
            on_ground = false;
        }
    }

    if (key == 'a')		    // Look left
        is_left = true;
    else if (key == 'd')	// Look right
        is_right = true;
    else if (key == 'w')	// Move forward
        is_forward = true;
    else if (key == 's')    // Move backward
        is_backward = true;
}


void keyUp(unsigned char key, int x, int y) {
    if (key == 'a')		    // Stop looking left
        is_left = false;
    else if (key == 'd')	// Stop looking right
        is_right = false;
    else if (key == 'w')	// Stop moving forward
        is_forward = false;
    else if (key == 's')    // Stop moving backward
        is_backward = false;
}


void specialDown(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        is_look_left = true;    // Look left
        break;
    case GLUT_KEY_RIGHT:
        is_look_right = true;   // Look right
        break;
    case GLUT_KEY_UP:
        is_look_up = true;      // Move forward
        break;
    case GLUT_KEY_DOWN:
        is_look_down = true;    // Move backward
        break;
    }
}


void specialUp(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        is_look_left = false;   // Stop looking left
        break;
    case GLUT_KEY_RIGHT:
        is_look_right = false;  // Stop looking right
        break;
    case GLUT_KEY_UP:
        is_look_up = false;     // Stop moving forward
        break;
    case GLUT_KEY_DOWN:
        is_look_down = false;   // Stop moving backward
        break;
    }
}


void jump() {
	if (jump_energy > 0) {
		y += jump_force * jump_energy;
		jump_energy -= 1;
	}
}


void cameraHandler() {
    if (is_look_left) {
        angle -= x_sens;
        lx = sin(angle);
        lz = -cos(angle);
    }
    else if (is_look_right) {
        angle += x_sens;
        lx = sin(angle);
        lz = -cos(angle);
    }

    if (is_look_up)
        y += y_sens;
    else if (is_look_down)
        y -= y_sens;

    if (is_forward) {
        x += lx * speed;
        z += lz * speed;
    }
    else if (is_backward) {
        x -= lx * speed;
        z -= lz * speed;
    }

    if (is_left) {
        x += lz * speed;
        z += -lx * speed;
    }
    else if (is_right) {
        x -= lz * speed;
        z -= -lx * speed;

    }

    if (is_jumping)
	    jump();

    // Gravity
	if (!on_ground)
		y -= speed;

    if (y <= 1.0f) {
	    y = 1.0f;
	    is_jumping = false;
        on_ground = true;
	    jump_energy = energy_max;
	}
}


void changeSize(int w, int h) {
    // Prevent division by zero when the window is too short
    if (h == 0)
        h = 1;

    float ratio = float(w * 1.0 / h);

    // Use the projection matrix
    glMatrixMode(GL_PROJECTION);

    // Reset matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get back to the modelview
    glMatrixMode(GL_MODELVIEW);
}


// Draws text displayed on screen
void renderText() {
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    drawText("Movement:", 5, 580, blackColor, helvetica);
    drawText("Forward -- ", 21, 558, blackColor, helvetica);
    drawText("W", 120, 558, blackColor, helvetica);
    drawText("Left ----- ", 21, 538, blackColor, helvetica);
    drawText("A", 122, 538, blackColor, helvetica);
    drawText("Back ---- ", 21, 518, blackColor, helvetica);
    drawText("S", 122, 518, blackColor, helvetica);
    drawText("Right ---- ", 21, 498, blackColor, helvetica);
    drawText("D", 122, 498, blackColor, helvetica);
    drawText("Jump ---- ", 20, 478, blackColor, helvetica);
    drawText("Spacebar", 122, 478, blackColor, helvetica);
    drawText("Camera View: Arrow Keys", 5, 453, blackColor, helvetica);
    drawText("Quit Program: Escape", 5, 428, blackColor, helvetica);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}


// Text function
void drawText(std::string text, int x, int y, int rgb[3], void* font) {
    draw_text = text;
    glColor3ub(rgb[0], rgb[1], rgb[2]);
    glRasterPos2i(x, y);
    for (size_t i = 0; i < draw_text.length(); i++)
        glutBitmapCharacter(font, draw_text[i]);
}


void drawPlantLeaf(float size, float rotx, float roty, float rotz, int parent_index, int index) {
    float r = plantMatrix[parent_index][0][0];
    float g = plantMatrix[parent_index][0][1];
    float b = plantMatrix[parent_index][0][2];
    float offset = plantMatrix[parent_index][index][0];
    float r_rand = plantMatrix[parent_index][index][1];
    float g_rand = plantMatrix[parent_index][index][2];
    float b_rand = plantMatrix[parent_index][index][3];

    glPushMatrix();
        glColor3ub(GLubyte(r - r_rand), GLubyte(g - g_rand), GLubyte(b - b_rand));
        glTranslatef(0.0f, 0.0f, 0.0f);
        glRotatef(rotx + offset, 1.0f, 0.0f, 0.0f);
        glRotatef(roty + offset, 0.0f, 1.0f, 0.0f);
        glRotatef(rotz + offset, 0.0f, 0.0f, 1.0f);
        glScalef(.7f, 5.0f, 0.7f);
        glutSolidSphere(size, 5, 5);
    glPopMatrix();
}


void drawPlant(float size, int index) {
    drawPlantLeaf(size, 0, 0, 0, index, 2);
    drawPlantLeaf(size, 30.0f, 0, 0, index, 3);
    drawPlantLeaf(size, -30.0f, 0, 0, index, 4);
    drawPlantLeaf(size, 0, 0, 30.0f, index, 5);
    drawPlantLeaf(size, 0, 0, -30.0f, index, 6);
    drawPlantLeaf(size, 24.0f, 0, 30.0f, index, 7);
    drawPlantLeaf(size, -24.0f, 0, -30.0f, index, 8);
    drawPlantLeaf(size, 24.0f, 0, -30.0f, index, 9);
    drawPlantLeaf(size, -24.0f, 0, 30.0f, index, 10);
}


void createPlant(int seed) {
    vector<vector<float>> plant;
    vector<float> plant_color;
    std::srand(seed);
    float r = float(rand() % 255);
    float g = float(rand() % 255);
    float b = float(rand() % 255);
    plant_color.push_back(r);
    plant_color.push_back(g);
    plant_color.push_back(b);
    plant.push_back(plant_color);
    vector<float> plant_pos;
    float x = float((rand() % 200) - 100);
    float z = float((rand() % 50) + 5);
    plant_pos.push_back(x);
    plant_pos.push_back(z);
    plant.push_back(plant_pos);

    for (size_t i = 0; i < 9; i++) {
        std::srand(i * seed);
        vector<float> plant_entity;
        float offset = float(rand() % 5);
        plant_entity.push_back(offset);
        float r_rand = float(rand() % 25);
        float g_rand = float(rand() % 25);
        float b_rand = float(rand() % 25);
        plant_entity.push_back(r_rand);
        plant_entity.push_back(g_rand);
        plant_entity.push_back(b_rand);
        plant.push_back(plant_entity);
    }
    plantMatrix.push_back(plant);
}


void drawRock(GLfloat angle, double size, GLint smoothness) {
    glPushMatrix();
        glColor3ub(47, 79, 79);        
        glRotatef(angle, 1.0f, 0, 0);
        glutSolidSphere(size, smoothness, smoothness);
    glPopMatrix();
}
