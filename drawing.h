#if !defined(DRAWING_H)
#define DRAWING_H

#include <string>

void drawAxes(float length);
void drawCircle(double x1, double y1, double radius);
void drawRectangle(double x, double y, double width, double height, std::string textureName = "");
void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
void drawLine(double x1, double y1, double x2, double y2);
void drawText(double x, double y, const char* string);
void draw3DQuad(double x, double y, double z, double xSize, double ySize, double height, std::string textureName = "", int repeats = 1);


#endif // DRAWING_H
