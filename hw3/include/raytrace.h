#pragma once
#include "types.h"

Ray rayThruPixel(int i, int j);
Color getColor(V3 point, V3 normal, float amb[3], float dif[3], float spe[3], float shi, float emi[3], int depth);