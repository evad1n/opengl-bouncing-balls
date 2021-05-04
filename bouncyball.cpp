#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

#include "bouncyball.h"
#include "vector2.h"
#include "helper.h"
#include "drawing.h"

const double GROUNDED_THRESHOLD = 0;
const double ELASTICITY = 0.9;
const double COLLISION_THRESHOLD = 0.01;
const int COLLISION_TIMEOUT = 1;

BouncyBall::BouncyBall(Vector2 startPos, Vector2 startVel, double radius, COLOR color, double gravity)
	: pos(startPos), vel(startVel), radius(radius), color(color), rot(0), drot(0), justCollided(0), gravity(gravity) {}

BouncyBall::~BouncyBall() {}

Vector2 BouncyBall::nextPos(double dt) {
	Vector2 n;
	n.x = this->pos.x + (this->vel.x * dt);
	n.y = this->pos.y + (this->vel.y * dt);
	return n;
}

void BouncyBall::bounce(double dt, int width, int height) {
	Vector2 np = this->nextPos(dt);
	if (np.x > width - this->radius + GROUNDED_THRESHOLD || np.x < this->radius) {
		this->vel.x *= -0.67;
		this->drot = this->vel.x / this->radius;
	}
	if (np.y > height - this->radius + GROUNDED_THRESHOLD || np.y < this->radius) {
		this->vel.y *= -0.67;
		this->drot = this->vel.x / this->radius;
	}
}

void BouncyBall::update(double dt, int width, int height, double resistance) {
	static Vector2 prevVel;
	this->bounce(dt, width, height);

	// std::cout << this->justCollided << std::endl;


	// Gravity
	if (this->pos.y > this->radius + GROUNDED_THRESHOLD && this->justCollided == 0) {
		this->vel.y -= this->gravity * dt;
	}

	// So things can stop
	// Make sure this is the last modification to velocity so it works
	// If yVel is slow and we are either on the ground or not at apex
	if ((abs(this->vel.y) < 1) && ((this->pos.y <= this->radius + GROUNDED_THRESHOLD) || (prevVel.y > 0)))
		this->vel.y = 0;
	if (abs(this->vel.x) < 0.01)
		this->vel.x = 0;


	// Actually move
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;
	this->rot -= this->drot * dt / 2;

	// So balls don't retreat to the shadow dimension
	this->pos.y = std::max(this->radius, this->pos.y);

	// Add friction if rolling on ground
	if (this->pos.y <= this->radius + 5) {
		this->vel.x *= 0.99;
		// Rolling speed
		this->drot = this->vel.x / this->radius;
	}

	this->vel *= (1 - resistance);

	prevVel = this->vel;

	this->draw();

	if (--this->justCollided < 0)
		this->justCollided = 0;
}

void BouncyBall::draw() {
	glColor3dv((GLdouble*)&this->color);
	DrawCircle(this->pos, this->radius, this->rot);
}

// If the distance between the next positions is less than the sum of the radii then they are colliding
bool isColliding(BouncyBall b1, BouncyBall b2, double dt) {
	if (b1.nextPos(dt).distTo(b2.nextPos(dt)) <= b1.radius + b2.radius) {
		return true;
	}
	return false;
}

void handleCollision(BouncyBall& b1, BouncyBall& b2, double dt) {
	b1.justCollided = COLLISION_TIMEOUT;
	b2.justCollided = COLLISION_TIMEOUT;

	// if (b1.vel.mag() < 0.5 && b2.vel.mag() < 0.5) {
	// 	b1.vel = Vector2();
	// 	b2.vel = Vector2();
	// 	return;
	// }

	// If balls are moving away from each other then don't collide
	double nowDist = b1.pos.distTo(b2.pos);
	double nextDist = b1.nextPos(dt).distTo(b2.nextPos(dt));
	if (nowDist < nextDist) {
		return;
	}


	Vector2 massNormal;	  // Center of Mass coordinate system, normal component
	Vector2 massTangent;	  // Center of Mass coordinate system, tangential component
	Vector2 vi[2];  // initial velocities of two particles
	Vector2 viMass[2]; // initial velocities in Center of Mass coordinates
	double viMassTangent[2];	  // initial velocities in Center of Mass coordinates, tangent component
	double viMassNormal[2];	  // initial velocities in Center of Mass coordinates, normal component
	Vector2 vf[2];  // final velocities of two particles
	double mass[2];	  // mass of two particles
	double M;		  // mass of two particles together
	Vector2 V;	  // velocity of two particles together

	Vector2 dif = b1.nextPos(dt) - b2.nextPos(dt);

	// set Center of Mass coordinate system
	dif = dif.normalized();
	massNormal = dif;
	massTangent = Vector2(dif.y, -dif.x);
	// set u values
	vi[0].x = b1.vel.x;
	vi[0].y = b1.vel.y;
	mass[0] = b1.radius * b1.radius;
	vi[1].x = b2.vel.x;
	vi[1].y = b2.vel.y;
	mass[1] = b2.radius * b2.radius;

	// set M and V
	M = mass[0] + mass[1];
	V.x = (vi[0].x * mass[0] + vi[1].x * mass[1]) / M;
	V.y = (vi[0].y * mass[0] + vi[1].y * mass[1]) / M;

	// set um values
	viMass[0].x = mass[1] / M * (vi[0].x - vi[1].x);
	viMass[0].y = mass[1] / M * (vi[0].y - vi[1].y);
	viMass[1].x = mass[0] / M * (vi[1].x - vi[0].x);
	viMass[1].y = mass[0] / M * (vi[1].y - vi[0].y);

	// set viMassTangent and viMassNormal values
	for (int i = 0; i < 2; i++) {
		viMassTangent[i] = viMass[i].x * massTangent.x + viMass[i].y * massTangent.y;
		viMassNormal[i] = viMass[i].x * massNormal.x + viMass[i].y * massNormal.y;
	}

	// set v values
	for (int i = 0; i < 2; i++) {
		vf[i].x = viMassTangent[i] * massTangent.x - viMassNormal[i] * massNormal.x + V.x;
		vf[i].y = viMassTangent[i] * massTangent.y - viMassNormal[i] * massNormal.y + V.y;
	}

	// Set final speeds
	b1.vel = Vector2(vf[0].x * ELASTICITY, vf[0].y * ELASTICITY);
	b2.vel = Vector2(vf[1].x * ELASTICITY, vf[1].y * ELASTICITY);

	// Rotation
	b1.drot = b1.vel.x / b1.radius;
	b2.drot = b2.vel.x / b2.radius;
}

// Old
void handleCollision2(BouncyBall& b1, BouncyBall& b2, double dt) {
	Vector2 en; // Center of Mass coordinate system, normal component
	Vector2 et; // Center of Mass coordinate system, tangential component
	Vector2 u[2]; // initial velocities of two particles
	Vector2 um[2]; // initial velocities in Center of Mass coordinates
	double umt[2]; // initial velocities in Center of Mass coordinates, tangent component
	double umn[2]; // initial velocities in Center of Mass coordinates, normal component
	Vector2 v[2]; // final velocities of two particles
	double m[2];	// mass of two particles
	double M; // mass of two particles together
	Vector2 V; // velocity of two particles together
	double size;
	int i;

	double xdif = b1.nextPos(dt).x - b2.nextPos(dt).x;
	double ydif = b1.nextPos(dt).y - b2.nextPos(dt).y;

	// set Center of Mass coordinate system
	size = sqrt(xdif * xdif + ydif * ydif);
	xdif /= size; ydif /= size; // normalize
	en.x = xdif;
	en.y = ydif;
	et.x = ydif;
	et.y = -xdif;

	// set u values
	u[0].x = b1.vel.x;
	u[0].y = b1.vel.y;
	m[0] = b1.radius * b1.radius;
	u[1].x = b2.vel.x;
	u[1].y = b2.vel.y;
	m[1] = b2.radius * b2.radius;

	// set M and V
	M = m[0] + m[1];
	V.x = (u[0].x * m[0] + u[1].x * m[1]) / M;
	V.y = (u[0].y * m[0] + u[1].y * m[1]) / M;

	// set um values
	um[0].x = m[1] / M * (u[0].x - u[1].x);
	um[0].y = m[1] / M * (u[0].y - u[1].y);
	um[1].x = m[0] / M * (u[1].x - u[0].x);
	um[1].y = m[0] / M * (u[1].y - u[0].y);

	// set umt and umn values
	for (i = 0;i < 2;i++) {
		umt[i] = um[i].x * et.x + um[i].y * et.y;
		umn[i] = um[i].x * en.x + um[i].y * en.y;
	}

	// set v values
	for (i = 0;i < 2;i++) {
		v[i].x = umt[i] * et.x - umn[i] * en.x + V.x;
		v[i].y = umt[i] * et.y - umn[i] * en.y + V.y;
	}

	// reset particle values
	b1.vel.x = (v[0].x * ELASTICITY);
	b1.vel.y = (v[0].y * ELASTICITY);
	b2.vel.x = (v[1].x * ELASTICITY);
	b2.vel.y = (v[1].y * ELASTICITY);

} /* Collide */
