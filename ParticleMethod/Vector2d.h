#pragma once
#include <math.h>

class Vector2d
{
public:
	double x, y;

	double length() {
		return sqrt(x * x + y * y);
	}
	Vector2d(double x1, double y1) {
		x = x1;
		y = y1;
	}

	Vector2d() {}
};

static Vector2d operator -(Vector2d r1, Vector2d r2) {
	Vector2d result;

	result.x = r1.x - r2.x;
	result.y = r1.y - r2.y;


	return result;
}

