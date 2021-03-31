#if !defined(VECTOR2_H)
#define VECTOR2_H

#include <iostream>

// A vector in 2D space
class Vector2
{
public:
	double x;
	double y;

	Vector2();
	Vector2(double x, double y);
	~Vector2();

	// Operator overloading
	Vector2 operator+(const Vector2& v);
	Vector2 operator-(const Vector2& v);
	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	// Scaling
	Vector2 operator*(double f);
	Vector2 operator/(double f);
	void operator*=(double f);
	void operator/=(double f);

	double distTo(const Vector2& v);
	double mag();
	Vector2 normalized();
	// Rotate a vector around the origin
	Vector2 rotated(double angle);
	Vector2 rotatedAround(Vector2 center, double angle);
};

std::ostream& operator << (std::ostream& os, const Vector2& v);

#endif // VECTOR2_H
