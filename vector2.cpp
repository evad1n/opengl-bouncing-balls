#include <cmath>
#include <iostream>

#include "vector2.h"

Vector2::Vector2(double x, double y)
	: x(x), y(y)
{
}

Vector2::Vector2()
	: x(0), y(0)
{
}

Vector2::~Vector2()
{
}

// Overloading

Vector2 Vector2::operator+(const Vector2& v) {
	return Vector2(this->x + v.x, this->y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) {
	return Vector2(this->x - v.x, this->y - v.y);
}

void Vector2::operator+=(const Vector2& v) {
	this->x += v.x;
	this->y += v.y;
}

void Vector2::operator-=(const Vector2& v) {
	this->x -= v.x;
	this->y -= v.y;
}

// Scaling

Vector2 Vector2::operator*(double f) {
	return Vector2(this->x * f, this->y * f);
}

Vector2 Vector2::operator/(double f) {
	return Vector2(this->x / f, this->y / f);
}

void Vector2::operator*=(double f) {
	this->x *= f;
	this->y *= f;
}

void Vector2::operator/=(double f) {
	this->x /= f;
	this->y /= f;
}

// Methods
// All are idempotent

double Vector2::distTo(const Vector2& v) {
	return sqrt(pow(this->x - v.x, 2) + pow(this->y - v.y, 2));
}

double Vector2::mag() {
	return this->distTo(Vector2());
}

Vector2 Vector2::normalized() {
	double m = this->mag();
	if (m == 0) return Vector2();
	return Vector2(this->x / m, this->y / m);
}

Vector2 Vector2::rotated(double angle) {
	double x = (cos(angle) * this->x) - (sin(angle) * this->y);
	double y = (sin(angle) * this->x) + (cos(angle) * this->y);
	return Vector2(x, y);
}

Vector2 Vector2::rotatedAround(Vector2 center, double angle) {
	// Simply subtract offset
	Vector2 o = *this - center;
	// Rotate around origin
	o = o.rotated(angle);
	// Add back offset
	return o + center;
}


std::ostream& operator << (std::ostream& os, const Vector2& v) {
	return (os << "{x: " << v.x << ", y: " << v.y << "}");
}