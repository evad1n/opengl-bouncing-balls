#include <cmath>
#include <cstring>
#include <GL/freeglut.h>

#include "vector2.h"
#include "helper.h"
#include "drawing.h"

void DrawCircle(Vector2 pos, double radius, double rot)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 32; i++)
	{
		double theta = ((double)i * M_PI * 2 / 32.0) + rot;
		double px = pos.x + radius * cos(theta);
		double py = pos.y + radius * sin(theta);
		glVertex2d(px, py);
	}
	glEnd();
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 3; i++)
	{
		double theta = ((double)i * M_PI * 2 / 3.0) + rot;

		double px = pos.x + (radius / 2) * cos(theta);
		double py = pos.y + (radius / 2) * sin(theta);
		glVertex2d(px, py);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1, y1);
	glVertex2d(x2, y1);
	glVertex2d(x2, y2);
	glVertex2d(x1, y2);
	glEnd();
}

void DrawTriangle(Vector2 p1, Vector2 p2, Vector2 p3)
{
	glBegin(GL_TRIANGLES);
	glVertex2dv((GLdouble*)&p1);
	glVertex2dv((GLdouble*)&p2);
	glVertex2dv((GLdouble*)&p3);
	glEnd();
}

void DrawArrow(Vector2 start, Vector2 end) {
	double mag = start.distTo(end);
	// Draw line
	glLineWidth(mag / 100);
	glBegin(GL_LINES);
	glVertex2dv((GLdouble*)&start);
	glVertex2dv((GLdouble*)&end);
	glEnd();

	// Get points of arrow
	Vector2 dir = (start - end) / mag;
	Vector2 tip = end - (dir * (mag / 10));
	Vector2 left = tip.rotatedAround(end, 120 * M_PI / 180) + (dir * (mag / 20));
	Vector2 right = tip.rotatedAround(end, 240 * M_PI / 180) + (dir * (mag / 20));

	// Draw arrow
	glBegin(GL_POLYGON);
	glVertex2dv((GLdouble*)&tip);
	glVertex2dv((GLdouble*)&left);
	glVertex2dv((GLdouble*)&end);
	glVertex2dv((GLdouble*)&right);
	glEnd();
}

void DrawLine(Vector2 start, Vector2 end) {
	glBegin(GL_LINES);
	glVertex2dv((GLdouble*)&start);
	glVertex2dv((GLdouble*)&end);
	glEnd();
}

void DrawText(double x, double y, const char* string)
{
	void* font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}