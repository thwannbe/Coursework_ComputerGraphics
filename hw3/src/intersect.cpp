#include "intersect.h"
#include "mymath.h"
#include <math.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

bool raySphereIntersect(Sphere s, Ray ray, float &t, V3& normal) {
	V3 center = s.center;
	float radius = s.radius;
	Ray nray;
	M44 inverse_trans = s.transform.inverse();
	V4 np0 = inverse_trans * V3toV4(ray.p0); np0.wnormalize();
	nray.p0 = V4toV3(np0);
	V4 np1; np1 = V3toV4(ray.p1); np1.w = 0;
	np1 = inverse_trans * np1; np1.wnormalize();
	nray.p1 = V4toV3(np1);

	// a * t^2 + b * t + c = 0
	// a = (p1 * p1)
	float a = dotproduct(nray.p1, nray.p1);
	// b = 2 * p1 * (p0 - c)
	float b = 2 * dotproduct(nray.p1, nray.p0 - center);
	// c = (p0 - c) * (p0 - c) - r^2
	float c = dotproduct(nray.p0 - center, nray.p0 - center) - pow(radius, 2);

	// b^2 - 4ac > 0 : two root, == 0 : one root, < 0 : imaginary root (no hit)
	float d = pow(b, 2) - 4 * a * c; // test
	bool ret;
	if (d >= 0) { // hit the sphere
		if (d == 0) { // rarely happened
			t = (-b) / (2.0 * a);
			if (t < 0)
				ret = false; // behind the camera
			else
				ret = true;
		}
		else { // d > 0
			float r0 = (-b + sqrt(d)) / (2.0 * a);
			float r1 = (-b - sqrt(d)) / (2.0 * a);
			if (r0 < 0 && r1 < 0) { // behind the camera
				ret = false;
			}
			else {
				if (r0 * r1 < 0) { // +/- root
					if (r0 > 0) {
						t = r0;
					}
					else {
						t = r1;
					}
					ret = true;
				}
				else { // r0 * r1 > 0, ++ root
					t = MIN(r0, r1);
					ret = true;
				}
			}
		}				
	}
	else { // d < 0
		ret = false;
	}
	
	// calculate normal
	if (ret) {
		V3 meet = nray.p0 + (nray.p1 * t);
		V3 norm_obj = meet - center;
		V4 norm_obj_v4 = V3toV4(norm_obj); norm_obj_v4.w = 0;
		V4 norm_w_v4 = s.transform * norm_obj_v4;
		norm_w_v4.wnormalize();
		normal = normalize(V4toV3(norm_w_v4));
	}

	return ret;
}

bool rayTriangleIntersect(Triangle tr, Ray ray, float &t, V3& normal) {
	V4 nnv0 = tr.transform * V3toV4(tr.v0); nnv0.wnormalize();
	V4 nnv1 = tr.transform * V3toV4(tr.v1); nnv1.wnormalize();
	V4 nnv2 = tr.transform * V3toV4(tr.v2); nnv2.wnormalize();
	V3 nv0 = V4toV3(nnv0);
	V3 nv1 = V4toV3(nnv1);
	V3 nv2 = V4toV3(nnv2);

	V3 v0v1 = nv1 - nv0;
	V3 v0v2 = nv2 - nv0;
	normal = normalize(crossproduct(v0v2, v0v1));

	if (ABS(dotproduct(ray.p1, normal)) < 0.001) // parallel
		return false;

	float d = dotproduct(normal, nv0);
	t = (d - dotproduct(normal, ray.p0)) / dotproduct(ray.p1, normal);

	//cout << "t : " << t << endl;

	if (t < 0) // in front of camera
		return false;

	V3 P = ray.p0 + ray.p1 * t;
	V3 C; // will be used for checking P is left side for each edges on the triangle

	// first edge check
	V3 e0 = nv1 - nv0;
	V3 vp0 = P - nv0;
	C = crossproduct(vp0, e0);
	if (dotproduct(normal, C) < 0) // P is on the right side
		return false;

	// second edge check
	V3 e1 = nv2 - nv1;
	V3 vp1 = P - nv1;
	C = crossproduct(vp1, e1);
	if (dotproduct(normal, C) < 0) // P is on the right side
		return false;

	// third edge check
	V3 e2 = nv0 - nv2;
	V3 vp2 = P - nv2;
	C = crossproduct(vp2, e2);
	if (dotproduct(normal, C) < 0) // P is on the right side
		return false;

	return true; // this ray hits the triangle
}