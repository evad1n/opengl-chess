#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <cstring>
#include "camera.h"
#include "input.h"
#include "drawing.h"

extern double deltaTime;
extern double screenX, screenY;

const double SPEED = 10;

Camera::Camera() {}

Camera::Camera(double startX, double startY, double startZ, double lookAtX, double lookAtY, double lookAtZ)
    : x(startX), y(startY), z(startZ), pitch(0), yaw(0), roll(0), speed(SPEED) {
    // Calc starting angles
    double look[] = { lookAtX - startX, lookAtY - startY, lookAtZ - startZ };

    this->yaw = -atan2(look[2], look[0]) * 180 / M_PI;
    this->pitch = atan2(look[1], look[0]) * 180 / M_PI;
}

Camera::~Camera() {}


void Camera::Update() {
    // Move camera
    if (keyStates[32] || keyStates['e'])
        this->MoveUp();
    if (keyStates['q'])
        this->MoveDown();
    if (keyStates['w'] || specialKeyStates[GLUT_KEY_UP])
        this->MoveForward();
    if (keyStates['s'] || specialKeyStates[GLUT_KEY_DOWN])
        this->MoveBackward();
    if (keyStates['a'] || specialKeyStates[GLUT_KEY_LEFT])
        this->MoveLeft();
    if (keyStates['d'] || specialKeyStates[GLUT_KEY_RIGHT])
        this->MoveRight();
}

// Move

void Camera::MoveDir(double rot) {
    double yawRads = (180 + rot + this->yaw) / 180 * M_PI;
    double fx = cos(yawRads);
    double fz = sin(yawRads);

    this->x += (this->speed * fx * deltaTime);
    this->z += (this->speed * fz * deltaTime);
}

void Camera::MoveForward() {
    this->MoveDir(-90);
}

void Camera::MoveBackward() {
    this->MoveDir(90);
}

void Camera::MoveLeft() {
    this->MoveDir(180);
}

void Camera::MoveRight() {
    this->MoveDir(0);
}

void Camera::MoveUp() {
    this->y += this->speed * deltaTime;
}

void Camera::MoveDown() {
    this->y -= this->speed * deltaTime;
}

// Look

void Camera::Yaw(double dRot) {
    this->yaw += dRot * deltaTime;
}

void Camera::Pitch(double dRot) {
    this->pitch += dRot * deltaTime;
    if (this->pitch > 89)
        this->pitch = 89;
    if (this->pitch < -89)
        this->pitch = -89;
}

void Camera::ResetView() {
    this->yaw = 0;
    this->pitch = 0;
}

void Camera::DisplayInfo(double fps) {
    std::ostringstream coords;
    coords << "X: " << this->x << "  ";
    coords << "Y: " << this->y << "  ";
    coords << "Z: " << this->z;
    this->DrawText(5, screenY - 20, coords.str());

    std::ostringstream oss;
    oss << "FPS: " << fps;
    this->DrawText(5, screenY - 40, oss.str());
}

void Camera::Display() {
    // Pos
    double eyeX = this->x;
    double eyeY = this->y;
    double eyeZ = this->z;

    // World up
    static double wx = 0;
    static double wy = 1;
    static double wz = 0;

    // Look
    double yawRads = (90 + this->yaw) / 180 * M_PI;
    double pitchRads = (this->pitch) / 180 * M_PI;
    // std::cout << "{ yaw: " << this->yaw << ", yawRadians: " << yawRads << ", pitch: " << this->pitch << ", pitchRads: " << pitchRads << " }" << std::endl;
    // Front
    double fx = cos(yawRads) * cos(pitchRads);
    double fy = sin(pitchRads);
    double fz = sin(yawRads) * cos(pitchRads);
    // Normalize
    double mag = sqrt(fx * fx + fy * fy + fz * fz);
    double atx = eyeX + (fx / mag);
    double aty = eyeY + (fy / mag);
    double atz = eyeZ + (fz / mag);

    gluLookAt(eyeX, eyeY, eyeZ, atx, aty, atz, wx, wy, wz);
}

// Outputs a string of text at the specified location.
void Camera::DrawText(double x, double y, std::string text) {
    glDisable(GL_BLEND);
    glColor4ub(0, 0, 0, 1);
    void* font = GLUT_BITMAP_HELVETICA_18;

    int len, i;
    glRasterPos2d(x, y);
    for (i = 0; i < text.length(); i++) {
        glutBitmapCharacter(font, text[i]);
    }
    glEnable(GL_BLEND);
}