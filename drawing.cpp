#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include <map>
#include <iostream>
#include "drawing.h"

extern std::map<std::string, unsigned int> texMap;

// Draw coordinate axes at the origin
void drawAxes(float length) {
    glDisable(GL_LIGHTING);
    glLineWidth(3.0);
    glBegin(GL_LINES);

    // x axis in red
    glColor3f(1, 0, 0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(length, 0.0, 0.0);
    // y axis in green
    glColor3f(0, 1, 0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, length, 0.0);
    // z axis in blue
    glColor3f(0, 0, 1);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, length);

    glEnd();
    glLineWidth(1.0);
    glEnable(GL_LIGHTING);
}

void drawCircle(double x1, double y1, double radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 32; i++) {
        double theta = (double)i / 32.0 * 2.0 * 3.1415926;
        double x = x1 + radius * cos(theta);
        double y = y1 + radius * sin(theta);
        glVertex2d(x, y);
    }
    glEnd();
}

void drawRectangle(double x, double y, double width, double height, std::string textureName) {
    if (textureName != "") {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texMap[textureName]);

        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2d(x, y);
        glTexCoord2f(1, 1); glVertex2d(x + width, y);
        glTexCoord2f(1, 0); glVertex2d(x + width, y + height);
        glTexCoord2f(0, 0); glVertex2d(x, y + height);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    } else {
        glBegin(GL_QUADS);
        glVertex2d(x, y);
        glVertex2d(x + width, y);
        glVertex2d(x + width, y + height);
        glVertex2d(x, y + height);
        glEnd();
    }
}

void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    glBegin(GL_TRIANGLES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glVertex2d(x3, y3);
    glEnd();
}

void drawLine(double x1, double y1, double x2, double y2) {
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}

// Outputs a string of text at the specified location.
void drawText(double x, double y, const char* string) {
    glColor4ub(0, 0, 0, 1);
    void* font = GLUT_BITMAP_HELVETICA_18;

    int len, i;
    glRasterPos2d(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, string[i]);
    }
}

// 3D Quad
// x,y,z are the lower coordinate values (bottom left lowest corner)
void draw3DQuad(double x, double y, double z, double xSize, double ySize, double height, std::string textureName, int repeats) {
    if (textureName != "") {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texMap[textureName]);

        // int xRepeats = int(xSize / 50) + 1, yRepeats = int(ySize / 50) + 1;
        // int xRepeats = 5, yRepeats = 5;

        if (xSize || ySize)

            // Find correct direction
            if (ySize > xSize) {
                // -z
                glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex3d(x, y, z);
                glTexCoord2f(repeats, 1); glVertex3d(x, y + ySize, z);
                glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y, z);
                glTexCoord2f(0, 0); glVertex3d(x + xSize, y + ySize, z);
                glEnd();
                // +z
                glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex3d(x, y, z + height);
                glTexCoord2f(repeats, 1); glVertex3d(x, y + ySize, z + height);
                glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y + ySize, z + height);
                glTexCoord2f(0, 0); glVertex3d(x + xSize, y, z + height);
                glEnd();
            } else {
                // -z
                glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex3d(x, y, z);
                glTexCoord2f(0, 1); glVertex3d(x, y + ySize, z);
                glTexCoord2f(repeats, 1); glVertex3d(x + xSize, y + ySize, z);
                glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y, z);
                glEnd();
                // +z
                glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex3d(x, y, z + height);
                glTexCoord2f(0, 1); glVertex3d(x, y + ySize, z + height);
                glTexCoord2f(repeats, 1); glVertex3d(x + xSize, y + ySize, z + height);
                glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y, z + height);
                glEnd();
            }

        // -x
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3d(x, y, z);
        glTexCoord2f(repeats, 1); glVertex3d(x, y + ySize, z);
        glTexCoord2f(repeats, 0); glVertex3d(x, y + ySize, z + height);
        glTexCoord2f(0, 0); glVertex3d(x, y, z + height);
        glEnd();
        // +x
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3d(x + xSize, y, z);
        glTexCoord2f(repeats, 1); glVertex3d(x + xSize, y + ySize, z);
        glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y + ySize, z + height);
        glTexCoord2f(0, 0); glVertex3d(x + xSize, y, z + height);
        glEnd();
        // -y
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);glVertex3d(x, y, z);
        glTexCoord2f(repeats, 1);glVertex3d(x + xSize, y, z);
        glTexCoord2f(repeats, 0);glVertex3d(x + xSize, y, z + height);
        glTexCoord2f(0, 0);glVertex3d(x, y, z + height);
        glEnd();
        // +y
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);glVertex3d(x, y + ySize, z);
        glTexCoord2f(repeats, 1);glVertex3d(x + xSize, y + ySize, z);
        glTexCoord2f(repeats, 0);glVertex3d(x + xSize, y + ySize, z + height);
        glTexCoord2f(0, 0);glVertex3d(x, y + ySize, z + height);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    } else {
        // -z
        glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x, y + ySize, z);
        glVertex3d(x + xSize, y, z);
        glVertex3d(x + xSize, y + ySize, z);
        glEnd();
        // +z
        glBegin(GL_QUADS);
        glVertex3d(x, y, z + height);
        glVertex3d(x, y + ySize, z + height);
        glVertex3d(x + xSize, y + ySize, z + height);
        glVertex3d(x + xSize, y, z + height);
        glEnd();
        // -x
        glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x, y + ySize, z);
        glVertex3d(x, y + ySize, z + height);
        glVertex3d(x, y, z + height);
        glEnd();
        // +x
        glBegin(GL_QUADS);
        glVertex3d(x + xSize, y, z);
        glVertex3d(x + xSize, y + ySize, z);
        glVertex3d(x + xSize, y + ySize, z + height);
        glVertex3d(x + xSize, y, z + height);
        glEnd();
        // -y
        glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x + xSize, y, z);
        glVertex3d(x + xSize, y, z + height);
        glVertex3d(x, y, z + height);
        glEnd();
        // +y
        glBegin(GL_QUADS);
        glVertex3d(x, y + ySize, z);
        glVertex3d(x + xSize, y + ySize, z);
        glVertex3d(x + xSize, y + ySize, z + height);
        glVertex3d(x, y + ySize, z + height);
        glEnd();
    }
}