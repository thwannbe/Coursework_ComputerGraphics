#include <iostream>
#include <FreeImage.h>
#include <math.h>

#include "types.h"
#include "mymath.h"
#include "intersect.h"
#include "raytrace.h"
#include "readfile.h"

#define MAINPROGRAM
#include "variables.h"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "program starts" << endl;

	FreeImage_Initialise();

	// init variables relating to light
	ambient[0] = 0.2; ambient[1] = 0.2; ambient[2] = 0.2;
	diffuse[0] = 0; diffuse[1] = 0; diffuse[2] = 0;
	specular[0] = 0; specular[1] = 0; specular[2] = 0;
	emission[0] = 0; emission[1] = 0; emission[2] = 0;
	shininess = 0;
	attenuation[0] = 1.0; attenuation[1] = 0; attenuation[2] = 0;
	maxdepth = 0;

	readfile(argv[1]);
	cout << "readfile is done" << endl;

	BYTE* buf = new BYTE[3 * imgsize.w * imgsize.h];	

	// raytracing
	for (int i = 0; i < imgsize.h; i++) {
		for (int j = 0; j < imgsize.w; j++) { // each pixels
			Ray ray = rayThruPixel(i, j);

			// check tri
			float min_t_tri = FLT_MAX; int min_t_tri_idx = -1; V3 min_t_tri_norm;
			for (int k = 0; k < tri.size(); k++) {
				float cur_t; V3 cur_norm;
				if (rayTriangleIntersect(tri[k], ray, cur_t, cur_norm)) {
					if (cur_t < min_t_tri) {
						min_t_tri = cur_t;
						min_t_tri_idx = k;
						min_t_tri_norm = cur_norm;
						//cout << "tri intersect t : " << cur_t << endl;
					}
				}
			}

			// check sph
			float min_t_sph = FLT_MAX; int min_t_sph_idx = -1; V3 min_t_sph_norm;
			for (int k = 0; k < sph.size(); k++) {
				float cur_t; V3 cur_norm;
				if (raySphereIntersect(sph[k], ray, cur_t, cur_norm)) {
					if (cur_t < min_t_sph) {
						min_t_sph = cur_t;
						min_t_sph_idx = k;
						min_t_sph_norm = cur_norm;
					}
				}
			}

			Color col; col.r = 0.0; col.g = 0.0; col.b = 0.0; // initialize as black
			if (min_t_tri_idx >= 0 || min_t_sph_idx >= 0) {
				if (min_t_tri_idx >= 0 && ((min_t_sph_idx == -1) || min_t_tri < min_t_sph)) { // render tri
					min_t_tri_norm = min_t_tri_norm * -1;
					col = getColor(ray.p0 + (ray.p1 * min_t_tri), min_t_tri_norm, tri[min_t_tri_idx].ambient, tri[min_t_tri_idx].diffuse, tri[min_t_tri_idx].specular, tri[min_t_tri_idx].shininess, tri[min_t_tri_idx].emission, 0);
				}
				else { // render sph
					col = getColor(ray.p0 + (ray.p1 * min_t_sph), min_t_sph_norm, sph[min_t_sph_idx].ambient, sph[min_t_sph_idx].diffuse, sph[min_t_sph_idx].specular, sph[min_t_sph_idx].shininess, sph[min_t_sph_idx].emission, 0);
				}
			}
			buf[3 * (imgsize.w * i + j)] = (char)(col.b * 0xff);
			buf[3 * (imgsize.w * i + j) + 1] = (char)(col.g * 0xff);
			buf[3 * (imgsize.w * i + j) + 2] = (char)(col.r * 0xff);
		}
	}

	
	FIBITMAP *img = FreeImage_ConvertFromRawBits(buf, imgsize.w, imgsize.h, imgsize.w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
	
	FreeImage_Save(FIF_PNG, img, "test.png", 0);

	FreeImage_DeInitialise();
	
	return 0;
}