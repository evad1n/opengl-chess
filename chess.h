#if !defined(CHESS_H)
#define CHESS_H

enum PIECES { PAWN = 10, KNIGHT, BISHOP, ROOK, KING, QUEEN };

extern const double SCALE;
extern const double OFFSET;
extern const double CENTER;
extern const double BOARD_HEIGHT;
extern const double UNIT;

void loadPieces();

void drawTable();
void drawBoard();

void updateLights();

#endif // CHESS_H
