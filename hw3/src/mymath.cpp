#include <math.h>
#include "mymath.h"

V3 normalize(V3 v) {
	float size = pow(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2), 0.5);
	return v * (1 / size);
}

float size(V3 v) {
	return pow(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2), 0.5);
}

float dotproduct(V3 a, V3 b) { // a * b
	return a.x* b.x + a.y * b.y + a.z * b.z;
}

V3 crossproduct(V3 a, V3 b) { // a x b
	a = normalize(a);
	M33 a_cross;
	a_cross.getval(0, a.z, -a.y, -a.z, 0, a.x, a.y, -a.x, 0);
	
	return a_cross * b;
}

V4 V3toV4(V3 v) {
	V4 ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	ret.w = 1.0;
	return ret;
}

V3 V4toV3(V4 v) {
	V3 ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	return ret;
}