#pragma once

#include "types.h"

V3 normalize(V3 v);
float size(V3 v);
float dotproduct(V3 a, V3 b); // a * b
V3 crossproduct(V3 a, V3 b); // a x b
V4 V3toV4(V3 v);
V3 V4toV3(V4 v);