#pragma once

#include <vector>
using namespace std;
#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif 

EXTERN ImageSize imgsize;
EXTERN Camera cam;

EXTERN float ambient[3];
EXTERN float diffuse[3];
EXTERN float specular[3];
EXTERN float emission[3];
EXTERN float shininess;
EXTERN float attenuation[3]; // const linear quadratic
EXTERN int maxdepth;
EXTERN vector <PositionLight> pos_l;
EXTERN vector <DirectionalLight> dir_l;
EXTERN vector <Triangle> tri;
EXTERN vector <Sphere> sph;
EXTERN vector <V3> vertex;