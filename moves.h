#if !defined(MOVES_H)
#define MOVES_H

#include "chess.h"

void drawPieces();

void move(int moveNumber, bool attacker = true);
void moveKnight(int moveNumber, double startX, double startZ, double endX, double endZ);
void capturePiece(int moveNumber, PIECES piece, double startX, double startZ);

#endif // MOVES_H
