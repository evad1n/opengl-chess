#include <GL/freeglut.h>
#include <iostream>
#include "input.h"
#include "camera.h"

// Camera
extern Camera camera;
extern double screenX;
extern double screenY;

const double ZOOM_SPEED = 1.0;
double fov = 45.0;

// Keyboard
bool keyStates[256];
bool specialKeyStates[256];

void mouseClick(int mouse_button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (mouse_button == GLUT_LEFT_BUTTON) {
        }
        if (mouse_button == GLUT_LEFT_BUTTON) {
        }
        if (mouse_button == GLUT_MIDDLE_BUTTON) {
        }
        if (mouse_button == GLUT_MIDDLE_BUTTON) {
        }
        // Scroll up
        if (mouse_button == 3) {
            fov -= ZOOM_SPEED;
            if (fov < 1.0)
                fov = 1.0;
        }
        // Scroll down
        if (mouse_button == 4) {
            fov += ZOOM_SPEED;
            if (fov > 50)
                fov = 50;
        }
    }

    glutPostRedisplay();
}

void mouseMove(int mouseX, int mouseY) {
    static int oldMouseX, oldMouseY;
    static bool userMove = false;
    // When to recenter cursor
    static const int THRESHOLD = 20;
    static const double SENSITIVITY = 0.01;

    int dx = mouseX - oldMouseX;
    int dy = mouseY - oldMouseY;

    if (mouseX < THRESHOLD || mouseX > screenX - THRESHOLD || mouseY < THRESHOLD || mouseY > screenY - THRESHOLD) {
        userMove = false;
        glutWarpPointer(screenX / 2, screenY / 2);
        userMove = true;
        oldMouseX = screenX / 2;
        oldMouseY = screenY / 2;
        return;
    }

    if (!userMove) {
        oldMouseX = mouseX;
        oldMouseY = mouseY;
        return;
    }


    // Save old mouse coordinates
    oldMouseX = mouseX;
    oldMouseY = mouseY;

    camera.Yaw((double)dx * SENSITIVITY);
    // Inverse y
    camera.Pitch((double)-dy * SENSITIVITY);
}

// Keyboard

// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyDown(unsigned char c, int x, int y) {
    keyStates[c] = true;
    switch (c) {
    case 27: // escape character means to quit the program
        exit(0);
        break;
    case 13:
        // Enter
        // Reset player camera
        camera.ResetView();
        break;
    case 32:
        // Space
        break;
    }

    glutPostRedisplay();
}

void keyUp(unsigned char c, int x, int y) {
    keyStates[c] = false;
    glutPostRedisplay();
}

// For the rest of the keys
void specialKeyDown(int key, int x, int y) {
    specialKeyStates[key] = true;
    glutPostRedisplay();
}

void specialKeyUp(int key, int x, int y) {
    specialKeyStates[key] = false;
    glutPostRedisplay();
}