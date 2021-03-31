#include <GL/freeglut.h>
#include "moves.h"
#include "chess.h"

const double KNIGHT_ROTATION = 20;

double t;

void interpolate(double t, double t1, double t2,
    double& v, double v1, double v2) {
    if (t < t1)
        t = t1;
    if (t > t2)
        t = t2;
    double ratio = (t - t1) / (t2 - t1);
    v = v1 + ratio * (v2 - v1);
}

void drawPieces() {
    t = (double)glutGet(GLUT_ELAPSED_TIME) / 500;

    // Set origin to h1
    glPushMatrix();
    glTranslatef(OFFSET + (SCALE / 2), BOARD_HEIGHT, OFFSET + (SCALE / 2));

    // WHITE
    GLfloat white_ambient[] = { 0.5,	0.5,	0.5, 1.0 };
    GLfloat white_specular[] = { 1, 1, 1, 1.0 };
    float white_shininess = 2;
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, white_shininess * 128.0);

    glPushMatrix();
    glTranslatef(UNIT * 3, 0, 0);
    glCallList(KING);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(UNIT * 4, 0, 0);
    glCallList(QUEEN);
    glPopMatrix();

    for (int x = 0; x < 2; x++) {
        glPushMatrix();
        glTranslatef(UNIT * (2 + (x * 3)), 0, 0);
        glCallList(BISHOP);
        glPopMatrix();
    }

    for (int x = 0; x < 2; x++) {
        glPushMatrix();
        glTranslatef(UNIT * (1 + (x * 5)), 0, 0);
        if (x == 0) {
            if (t < 10) {
                move(3);
            } else if (t < 12) {
                move(5);
            } else {
                move(6, false);
            }
        } else {
            glCallList(KNIGHT);
        }
        glPopMatrix();
    }

    for (int x = 0; x < 2; x++) {
        glPushMatrix();
        glTranslatef(UNIT * (x * 7), 0, 0);
        glCallList(ROOK);
        glPopMatrix();
    }


    for (int x = 0; x < 8; x++) {
        glPushMatrix();
        glTranslatef(UNIT * x, 0, UNIT);
        if (x == 3) {
            move(1);
        } else {
            glCallList(PAWN);
        }
        glPopMatrix();
    }


    // BLACK
    GLfloat black_ambient[] = { 0, 0, 0, 1.0 };
    GLfloat black_specular[] = { 1, 1, 1, 1.0 };
    float black_shininess = 20;
    GLfloat black[] = { 0, 0, 0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, black_shininess * 128.0);

    glTranslatef(UNIT * 7, 0, UNIT * 7);
    glRotated(180, 0, 1, 0);

    glPushMatrix();
    glTranslatef(UNIT * 3, 0, 0);
    glCallList(QUEEN);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(UNIT * 4, 0, 0);
    glCallList(KING);
    glPopMatrix();

    for (int x = 0; x < 2; x++) {
        glPushMatrix();
        glTranslatef(UNIT * (2 + (x * 3)), 0, 0);
        glCallList(BISHOP);
        glPopMatrix();
    }

    for (int x = 0; x < 2; x++) {
        glPushMatrix();
        glTranslatef(UNIT * (1 + (x * 5)), 0, 0);
        if (x == 0) {
            if (t < 12) {
                move(4);
            } else {
                move(6);
            }
        } else {
            glCallList(KNIGHT);
        }
        glPopMatrix();
    }

    for (int x = 0; x < 2; x++) {
        glPushMatrix();
        glTranslatef(UNIT * (x * 7), 0, 0);
        glCallList(ROOK);
        glPopMatrix();
    }


    for (int x = 0; x < 8; x++) {
        glPushMatrix();
        glTranslatef(UNIT * x, 0, UNIT);
        if (x == 4) {
            if (t > 10) {
                move(5, false);
            } else {
                move(2);
            }
        } else {
            glCallList(PAWN);
        }
        glPopMatrix();
    }

    glPopMatrix();
}


void move(int moveNumber, bool attacker) {
    switch (moveNumber) {
    case 1:
    {
        // WHITE: Move pawn e2 to e4
        double pawnZ = 0;
        interpolate(t, moveNumber * 2, (moveNumber + 1) * 2, pawnZ, 0, 2);
        glPushMatrix();
        glTranslatef(0, 0, UNIT * pawnZ);
        glCallList(PAWN);
        glPopMatrix();
        break;
    }
    case 2:
    {
        // BLACK: Move pawn e7 to e5
        double pawnZ = 0;
        interpolate(t, moveNumber * 2, (moveNumber + 1) * 2, pawnZ, 0, 2);
        glPushMatrix();
        glTranslatef(0, 0, UNIT * pawnZ);
        glCallList(PAWN);
        glPopMatrix();
        break;
    }
    case 3:
    {
        // WHITE: Move knight g1 to f3
        moveKnight(3, 0, 0, 1, 2);
        break;
    }
    case 4:
    {
        // BLACK: Move knight b8 to c6
        moveKnight(4, 0, 0, 1, 2);
        break;
    }
    case 5:
    {
        if (attacker) {
            // WHITE: Capture pawn with knight f3 to e5
            moveKnight(5, 1, 2, 2, 4);
        } else {
            // BLACK: Get rekt pawn
            capturePiece(5, PAWN, 0, 2);
        }
        break;
    }
    case 6:
    {
        if (attacker) {
            // BLACK: Capture knight with knight c6 to e5
            moveKnight(6, 1, 2, 3, 3);
        } else {
            // BLACK: Get rekt knight
            capturePiece(6, KNIGHT, 2, 4);
        }
        break;
    }
    default:
        break;
    }
}

void capturePiece(int moveNumber, PIECES piece, double startX, double startZ) {
    double start = (moveNumber * 2) + 1.8;
    double end = start + 1;
    // Stop drawing when they dead
    if (t < end) {
        double pieceX = startX;
        double pieceY = 0;
        double pieceZ = startZ;

        double pieceRotX = 0;
        double pieceRotY = 0;
        double pieceRotZ = 0;

        interpolate(t, start, end, pieceX, startX, 0);
        interpolate(t, start, end, pieceY, 0, 0);
        interpolate(t, start, end, pieceZ, startZ, startZ - 1);
        interpolate(t, start, end, pieceRotX, 0, -90);
        interpolate(t, start, end, pieceRotY, 0, 0);
        interpolate(t, start, end, pieceRotZ, 0, 0);

        glPushMatrix();
        glTranslatef(UNIT * pieceX, UNIT * pieceY, UNIT * pieceZ);
        glRotated(pieceRotX, 1, 0, 0);
        glRotated(pieceRotY, 0, 1, 0);
        glRotated(pieceRotZ, 0, 0, 1);
        glCallList(piece);
        glPopMatrix();
    }
}

void moveKnight(int moveNumber, double startX, double startZ, double endX, double endZ) {
    double start = moveNumber * 2;
    double end = (moveNumber + 1) * 2;
    double knightX = startX;
    double knightY = 0;
    double knightZ = startZ;
    double knightRot = 0;
    // Up
    if (t >= start) {
        interpolate(t, start, start + 0.5, knightRot, 0, KNIGHT_ROTATION);
        interpolate(t, start, start + 0.5, knightY, 0, 1);
    }
    if (t >= start + 0.5) {
        interpolate(t, start + 0.5, start + 1.5, knightRot, KNIGHT_ROTATION, -KNIGHT_ROTATION);
        interpolate(t, start + 0.5, start + 1, knightY, 1, 1.4);
    }
    interpolate(t, moveNumber * 2, (moveNumber + 1) * 2, knightX, startX, endX);
    interpolate(t, moveNumber * 2, (moveNumber + 1) * 2, knightZ, startZ, endZ);
    // Down
    if (t >= start + 1) {
        interpolate(t, start + 1, start + 1.5, knightY, 1.4, 1);
    }
    if (t >= start + 1.5) {
        interpolate(t, start + 1.5, end, knightRot, -KNIGHT_ROTATION, 0);
        interpolate(t, start + 1.5, end, knightY, 1, 0);
    }
    glPushMatrix();
    glTranslatef(UNIT * knightX, UNIT * knightY, UNIT * knightZ);
    glRotated(knightRot, 1, 0, 0);
    glCallList(KNIGHT);
    glPopMatrix();
}