#include <GL/freeglut.h>
#include <map>
#include <cmath>
#include <fstream>
#include <iostream>
#include "chess.h"
#include "moves.h"

extern std::map<std::string, unsigned int> texMap;


const double SCALE = 1000;
const double OFFSET = SCALE * 2;
const double CENTER = OFFSET + (SCALE * 4);
const double BOARD_HEIGHT = SCALE / 2;
const double UNIT = SCALE; // One square on the board


void updateLights() {
    float lightx = CENTER;
    float lighty = UNIT * 10;
    interpolate(t, 14, 15, lighty, UNIT * 10, BOARD_HEIGHT);
    float lightz = CENTER;
    GLfloat light_position[] = { lightx, lighty, lightz, 1.0 };
    float dirx = CENTER - lightx;
    float diry = BOARD_HEIGHT - lighty;
    float dirz = CENTER - lightz;;
    GLfloat spot_direction[] = { dirx, diry, dirz };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

    // So light actually looks lit
    GLfloat light_position2[] = { lightx, lighty + (float)SCALE * 4, lightz, 1.0 };
    GLfloat spot_reverse_direction[] = { -dirx, -diry, -dirz };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_reverse_direction);

    // Draw light cone facing right direction
    glPushMatrix();
    glTranslatef(lightx, lighty, lightz);
    double rotY = atan2(dirx, dirz) * 180 / M_PI;
    glRotatef(rotY, 0, 1, 0);
    double rotX = -atan2(diry, dirz) * 180 / M_PI;
    glRotatef(-rotX, 1, 0, 0);
    double rotZ = -atan2(diry, dirx) * 180 / M_PI;
    glRotatef(-rotZ, 0, 0, 1);
    // Cone
    glutSolidCone(5 * SCALE / 2, 5 * SCALE / 2, 5, 5);
    // Bright part
    GLfloat white_specular[] = { 1, 1, 1, 1.0 };
    float white_shininess = 100;
    GLfloat lightColor[] = { .9, .8, .6, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lightColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, white_shininess * 128.0);
    glutSolidSphere(SCALE / 1.1, 10, 10);

    glPopMatrix();


}

// Draw table
void drawTable() {
    const double RADIUS = SCALE * 8;
    const double THICK = SCALE;
    GLfloat table[] = { 0.8, 0.8, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, table);
    // Top face
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMap["table.jpg"]);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 32; i++) {
        double theta = (double)i / 32.0 * 2.0 * 3.1415926;
        double x = CENTER + RADIUS * cos(theta);
        double z = CENTER + RADIUS * sin(theta);
        double tx = cos(theta) * 0.5 + 0.5;
        double tz = sin(theta) * 0.5 + 0.5;
        glNormal3d(0, 1, 0);
        glTexCoord2d(tx, tz);
        glVertex3d(x, 0, z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Sides
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMap["table.jpg"]);
    for (int i = 0; i < 32; i++) {
        double theta1 = (double)i / 32.0 * 2.0 * 3.1415926;
        double theta2 = (double)(i + 1) / 32.0 * 2.0 * 3.1415926;
        double x1 = CENTER + RADIUS * cos(theta1);
        double z1 = CENTER + RADIUS * sin(theta1);

        double x2 = CENTER + RADIUS * cos(theta2);
        double z2 = CENTER + RADIUS * sin(theta2);

        double x3 = CENTER + (RADIUS + SCALE / 3) * cos(theta2);
        double z3 = CENTER + (RADIUS + SCALE / 3) * sin(theta2);

        double x4 = CENTER + (RADIUS + SCALE / 3) * cos(theta1);
        double z4 = CENTER + (RADIUS + SCALE / 3) * sin(theta1);

        const double repeats = 1;
        // Top edge
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex3d(x1, 0, z1);

        glTexCoord2d(0, 0);
        glVertex3d(x2, 0, z2);

        glTexCoord2d(0, repeats);
        glVertex3d(x3, -THICK / 2, z3);

        glTexCoord2d(repeats, repeats);
        glVertex3d(x4, -THICK / 2, z4);
        glEnd();

        // Bot edge
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex3d(x1, -THICK, z1);

        glTexCoord2d(0, 0);
        glVertex3d(x2, -THICK, z2);

        glTexCoord2d(0, repeats);
        glVertex3d(x3, -THICK / 2, z3);

        glTexCoord2d(repeats, repeats);
        glVertex3d(x4, -THICK / 2, z4);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);

    // Bottom face
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMap["table.jpg"]);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 32; i++) {
        double theta = (double)i / 32.0 * 2.0 * 3.1415926;
        double x = CENTER + RADIUS * cos(theta);
        double z = CENTER + RADIUS * sin(theta);
        double tx = cos(theta) * 0.5 + 0.5;
        double tz = sin(theta) * 0.5 + 0.5;
        glTexCoord2d(tx, tz);
        glVertex3d(x, -THICK, z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawBoard() {
    // Draw top
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMap["table.jpg"]);
    GLfloat light[] = { 252.0 / 255.0, 234.0 / 255.0,207.0 / 255.0, 1.0 };
    GLfloat dark[] = { 46.0 / 255.0, 27.0 / 255.0, 19.0 / 255.0, 1.0 };
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0)
                glMaterialfv(GL_FRONT, GL_DIFFUSE, light);
            else
                glMaterialfv(GL_FRONT, GL_DIFFUSE, dark);
            glNormal3d(0, 1, 0);
            glTexCoord2d(0, 0);
            glVertex3d(i * SCALE + OFFSET, BOARD_HEIGHT, j * SCALE + OFFSET);
            glTexCoord2d(0, 1);
            glVertex3d(i * SCALE + OFFSET, BOARD_HEIGHT, (j + 1) * SCALE + OFFSET);
            glTexCoord2d(1, 1);
            glVertex3d((i + 1) * SCALE + OFFSET, BOARD_HEIGHT, (j + 1) * SCALE + OFFSET);
            glTexCoord2d(1, 0);
            glVertex3d((i + 1) * SCALE + OFFSET, BOARD_HEIGHT, j * SCALE + OFFSET);
        }
    glDisable(GL_TEXTURE_2D);
    glEnd();

    // Draw sides
    double x = OFFSET;
    double y = 0;
    double z = OFFSET;
    double xSize = SCALE * 8;
    double height = BOARD_HEIGHT;
    double zSize = SCALE * 8;
    // Side texture
    double repeats = 8;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMap["wood.jpg"]);
    // -x
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3d(x, y, z);
    glTexCoord2f(0, 1); glVertex3d(x, y + height, z);
    glTexCoord2f(repeats, 1); glVertex3d(x, y + height, z + zSize);
    glTexCoord2f(repeats, 0); glVertex3d(x, y, z + zSize);
    glEnd();
    // +x
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3d(x + xSize, y, z);
    glTexCoord2f(0, 1); glVertex3d(x + xSize, y + height, z);
    glTexCoord2f(repeats, 1); glVertex3d(x + xSize, y + height, z + zSize);
    glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y, z + zSize);
    glEnd();
    // -z
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3d(x, y, z);
    glTexCoord2f(0, 1); glVertex3d(x, y + height, z);
    glTexCoord2f(repeats, 1); glVertex3d(x + xSize, y + height, z);
    glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y, z);
    glEnd();
    // +z
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3d(x, y, z + zSize);
    glTexCoord2f(0, 1); glVertex3d(x, y + height, z + zSize);
    glTexCoord2f(repeats, 1); glVertex3d(x + xSize, y + height, z + zSize);
    glTexCoord2f(repeats, 0); glVertex3d(x + xSize, y, z + zSize);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void findTriangleNormal(double x[], double y[], double z[], double n[]) {
    // Convert the 3 input points to 2 vectors, v1 and v2.
    double v1[3], v2[3];
    v1[0] = x[1] - x[0];
    v1[1] = y[1] - y[0];
    v1[2] = z[1] - z[0];
    v2[0] = x[2] - x[0];
    v2[1] = y[2] - y[0];
    v2[2] = z[2] - z[0];

    // Take the cross product of v1 and v2, to find the vector perpendicular to both.
    n[0] = v1[1] * v2[2] - v1[2] * v2[1];
    n[1] = -(v1[0] * v2[2] - v1[2] * v2[0]);
    n[2] = v1[0] * v2[1] - v1[1] * v2[0];

    double size = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
    n[0] /= -size;
    n[1] /= -size;
    n[2] /= -size;
}

// Loads the given data file and draws it at its default position.
// Call glTranslate before calling this to get it in the right place.
void loadPiece(std::string filename) {
    filename = "pieces/" + filename;
    // Try to open the given file.
    char buffer[200];
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error. Could not open " << filename << std::endl;
        exit(1);
    }

    double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices.
    int done = false;
    int verts = 0; // vertices in the current polygon
    int polygons = 0; // total polygons in this file.
    do {
        in.getline(buffer, 200); // get one line (point) from the file.
        int count = sscanf(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
        done = in.eof();
        if (!done) {
            if (count == 3) // if this line had an x,y,z point.
            {
                verts++;
            } else // the line was empty. Finish current polygon and start a new one.
            {
                if (verts >= 3) {
                    glBegin(GL_POLYGON);
                    double n[3];
                    findTriangleNormal(x, y, z, n);
                    glNormal3dv(n);
                    for (int i = 0; i < verts; i++) {
                        glVertex3d(x[i], y[i], z[i]);
                    }
                    glEnd(); // end previous polygon
                    polygons++;
                    verts = 0;
                }
            }
        }
    } while (!done);

    if (verts > 0) {
        std::cerr << "Error. Extra vertices in file " << filename << std::endl;
        exit(1);
    }

}

void loadPieces() {
    glNewList(KING, GL_COMPILE);
    loadPiece("KING.POL");
    glEndList();

    glNewList(QUEEN, GL_COMPILE);
    loadPiece("QUEEN.POL");
    glEndList();

    glNewList(BISHOP, GL_COMPILE);
    loadPiece("BISHOP.POL");
    glEndList();

    glNewList(KNIGHT, GL_COMPILE);
    loadPiece("KNIGHT.POL");
    glEndList();

    glNewList(ROOK, GL_COMPILE);
    loadPiece("ROOK.POL");
    glEndList();

    glNewList(PAWN, GL_COMPILE);
    loadPiece("PAWN.POL");
    glEndList();
}