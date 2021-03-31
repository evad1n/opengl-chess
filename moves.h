#if !defined(MOVES_H)
#define MOVES_H

#include "chess.h"

extern double t;

void drawPieces();
void interpolate(double t, double t1, double t2, double& v, double v1, double v2);
void interpolate(double t, double t1, double t2, float& v, double v1, double v2);

void move(int moveNumber, bool attacker = true);
void moveKnight(int moveNumber, double startX, double startZ, double endX, double endZ);
void capturePiece(int moveNumber, PIECES piece, double startX, double startZ);

#endif // MOVES_H
