#if !defined(DRAWING_H)
#define DRAWING_H

#include <GL/freeglut.h>
#include "vector2.h"

// Simple color struct
struct COLOR
{
	double r, g, b;
};

void DrawCircle(Vector2 pos, double radius, double rot);

void DrawRectangle(double x1, double y1, double x2, double y2);

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);

void DrawArrow(Vector2 start, Vector2 end);

void DrawLine(Vector2 start, Vector2 end);

// Outputs a string of text at the specified location.
void DrawText(double x, double y, const char* string);

#endif // DRAWING_H
