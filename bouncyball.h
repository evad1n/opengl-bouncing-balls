#if !defined(BOUNCYBALL_H)
#define BOUNCYBALL_H

#include "vector2.h"
#include "helper.h"
#include "drawing.h"


// A ball that bounces
class BouncyBall {
public:
	// Position vector
	Vector2 pos;
	// Velocity vector
	Vector2 vel;
	// Radius, from which mass is derived
	double radius;
	// The color to draw the ball with
	COLOR color;
	// Rotation 
	double rot;
	double drot;
	// Collision timeout
	int justCollided;

	double gravity;

	BouncyBall(Vector2 startPos, Vector2 startVel, double radius, COLOR color, double gravity = 9.8);
	~BouncyBall();
	void update(double dt, int width, int height, double resistance);
	void draw();
	void bounce(double dt, int width, int height);

	// Next position given deltaTime
	Vector2 nextPos(double dt);
	double mass();
};

bool isColliding(BouncyBall b1, BouncyBall b2, double dt);
void handleCollision(BouncyBall& b1, BouncyBall& b2, double dt);
void handleCollision2(BouncyBall& b1, BouncyBall& b2, double dt);

#endif // BOUNCYBALL_H
