#pragma once

#include "types.h"

bool raySphereIntersect(Sphere s, Ray ray, float& t, V3& normal);
bool rayTriangleIntersect(Triangle tr, Ray ray, float& t, V3& normal);