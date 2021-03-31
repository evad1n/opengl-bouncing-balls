#include <cmath>
#include <iostream>
#include "bouncyball.h"


double clamp(double val, double lo, double hi) {
	return val < lo ? lo : (val > hi ? hi : val);
}

double lowSpeed(double val, double threshold) {
	return abs(val) < threshold ? 0 : val;
}