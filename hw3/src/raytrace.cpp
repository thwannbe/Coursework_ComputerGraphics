#include "raytrace.h"
#include <math.h>
#include "mymath.h"
#include "variables.h"
#include "intersect.h"

#define ENABLE_SHADOW
#define ENABLE_REFLECTION

Ray rayThruPixel(int i, int j) {
	float fi, fj;
	fi = (float)i + 0.5;
	fj = (float)j + 0.5;
	float w_h = ((float)imgsize.w) / 2.0;
	float h_h = ((float)imgsize.h) / 2.0;
	float alpha = tan(cam.fovx * 0.5) * (fj - w_h) / w_h;
	float beta = tan(cam.fovy * 0.5) * (h_h - fi) / h_h;
	Ray ret;
	ret.p0.x = cam.lf.x;
	ret.p0.y = cam.lf.y;
	ret.p0.z = cam.lf.z;

	V3 a; // eye - center
	a.x = cam.lf.x - cam.la.x;
	a.y = cam.lf.y - cam.la.y;
	a.z = cam.lf.z - cam.la.z;

	V3 w = normalize(a);
	V3 u = normalize(crossproduct(cam.u, w));
	V3 v = crossproduct(w, u);

	V3 p1t;
	p1t.x = alpha * u.x + beta * v.x - w.x;
	p1t.y = alpha * u.y + beta * v.y - w.y;
	p1t.z = alpha * u.z + beta * v.z - w.z;
	ret.p1 = normalize(p1t);

	return ret;
}

#define MAX(a,b) ((a) > (b) ? (a) : (b))

Color getColor(V3 point, V3 normal, float amb[3], float dif[3], float spe[3], float shi, float emi[3], int depth) {
	Color ret;
	// for convenience, treat color with vector
	V3 final_col; final_col.x = 0; final_col.y = 0; final_col.z = 0; // init as black
	V3 ambient; ambient.x = amb[0]; ambient.y = amb[1]; ambient.z = amb[2]; // rgb
	final_col = final_col + ambient;
	V3 emission; emission.x = emi[0]; emission.y = emi[1]; emission.z = emi[2]; // rgb
	final_col = final_col + emission;
	
#ifdef ENABLE_REFLECTION
	if (depth < 5) {
		V3 pos2eye = normalize(cam.lf - point);
		V3 dir_ref_ray = normal - pos2eye; dir_ref_ray = normalize(dir_ref_ray);
		Ray ref_ray; ref_ray.p0 = point; ref_ray.p1 = dir_ref_ray;
		ref_ray.p0 = ref_ray.p0 + (ref_ray.p1 * 0.001);

		// find the next object which the reflective ray meets
		// check tri
		float min_t_tri = FLT_MAX; int min_t_tri_idx = -1; V3 min_t_tri_norm;
		for (int j = 0; j < tri.size(); j++) {
			float cur_t; V3 cur_norm;
			if (rayTriangleIntersect(tri[j], ref_ray, cur_t, cur_norm)) {
				if (cur_t < min_t_tri) {
					min_t_tri = cur_t;
					min_t_tri_idx = j;
					min_t_tri_norm = cur_norm;
				}
			}
		}

		// check sph
		float min_t_sph = FLT_MAX; int min_t_sph_idx = -1; V3 min_t_sph_norm;
		for (int j = 0; j < sph.size(); j++) {
			float cur_t; V3 cur_norm;
			if (raySphereIntersect(sph[j], ref_ray, cur_t, cur_norm)) {
				if (cur_t < min_t_sph) {
					min_t_sph = cur_t;
					min_t_sph_idx = j;
					min_t_sph_norm = cur_norm;
				}
			}
		}

		Color col; col.r = 0.0; col.g = 0.0; col.b = 0.0; // initialize as black
		if (min_t_tri_idx >= 0 || min_t_sph_idx >= 0) {
			if (min_t_tri_idx >= 0 && ((min_t_sph_idx == -1) || min_t_tri < min_t_sph)) { // render tri
				min_t_tri_norm = min_t_tri_norm * -1;
				col = getColor(ref_ray.p0 + (ref_ray.p1 * min_t_tri), min_t_tri_norm, tri[min_t_tri_idx].ambient, tri[min_t_tri_idx].diffuse, tri[min_t_tri_idx].specular, tri[min_t_tri_idx].shininess, tri[min_t_tri_idx].emission, depth + 1);
			}
			else { // render sph
				col = getColor(ref_ray.p0 + (ref_ray.p1 * min_t_sph), min_t_sph_norm, sph[min_t_sph_idx].ambient, sph[min_t_sph_idx].diffuse, sph[min_t_sph_idx].specular, sph[min_t_sph_idx].shininess, sph[min_t_sph_idx].emission, depth + 1);
			}
		}
		V3 reflection; reflection.x = spe[0] * col.r; reflection.y = spe[1] * col.g; reflection.z = spe[2] * col.b;
		final_col = final_col + reflection;
	}
#endif

	// calculate per point lights
	for (int i = 0; i < pos_l.size(); i++) {
		Ray ray; ray.p0 = point; ray.p1 = normalize(pos_l[i].pos - point);
		ray.p0 = ray.p0 + (ray.p1 * 0.001);

		bool block = false;
		
#ifdef ENABLE_SHADOW
		// check tri
		for (int j = 0; j < tri.size(); j++) {
			float cur_t; V3 cur_norm;
			if (rayTriangleIntersect(tri[j], ray, cur_t, cur_norm)) {
				if (cur_t < size(pos_l[i].pos - point)) {
					block = true;
					break;
				}
			}
		}

		// check sph
		for (int j = 0; !block && j < sph.size(); j++) {
			float cur_t; V3 cur_norm;
			if (raySphereIntersect(sph[j], ray, cur_t, cur_norm)) {
				if (cur_t < size(pos_l[i].pos - point)) {
					block = true;
					break;
				}
			}
		}
#endif

		if (!block) {
			V3 dir = normalize(pos_l[i].pos - point);
			V3 half = normalize(dir + normalize(cam.lf - point));
			V3 light_col; light_col.x = pos_l[i].col[0]; light_col.y = pos_l[i].col[1]; light_col.z = pos_l[i].col[2];
			float r = size(pos_l[i].pos - point);
			float att = 1 / (attenuation[0] + attenuation[1] * r + attenuation[2] * pow(r, 2));

			V3 diffuse; float nDotL = dotproduct(normal, dir);
			diffuse.x = dif[0] * att * light_col.x * MAX(0, nDotL);
			diffuse.y = dif[1] * att * light_col.y * MAX(0, nDotL);
			diffuse.z = dif[2] * att * light_col.z * MAX(0, nDotL);
			final_col = final_col + diffuse;

			V3 specular; float nDotH = dotproduct(normal, half);
			specular.x = spe[0] * att * light_col.x * pow(MAX(0, nDotH), shi);
			specular.y = spe[1] * att * light_col.y * pow(MAX(0, nDotH), shi);
			specular.z = spe[2] * att * light_col.z * pow(MAX(0, nDotH), shi);
			final_col = final_col + specular;
		}
	}

	// calculate per directional lights
	for (int i = 0; i < dir_l.size(); i++) {
		Ray ray; ray.p0 = point; ray.p1 = normalize(dir_l[i].dir * -1.0);
		ray.p0 = ray.p0 + (ray.p1 * 0.001);

		bool block = false;

#ifdef ENABLE_SHADOW
		// check tri
		for (int j = 0; j < tri.size(); j++) {
			float cur_t; V3 cur_norm;
			if (rayTriangleIntersect(tri[j], ray, cur_t, cur_norm)) {
				block = true;
				break;
			}
		}

		// check sph
		for (int j = 0; !block && j < sph.size(); j++) {
			float cur_t; V3 cur_norm;
			if (raySphereIntersect(sph[j], ray, cur_t, cur_norm)) {
				block = true;
				break;
			}
		}
#endif

		if (!block) {
			V3 dir = normalize(dir_l[i].dir);
			V3 half = normalize(dir + normalize(cam.lf - point));
			V3 light_col; light_col.x = dir_l[i].col[0]; light_col.y = dir_l[i].col[1]; light_col.z = dir_l[i].col[2];

			V3 diffuse; float nDotL = dotproduct(normal, dir);
			diffuse.x = dif[0] * light_col.x * MAX(0, nDotL);
			diffuse.y = dif[1] * light_col.y * MAX(0, nDotL);
			diffuse.z = dif[2] * light_col.z * MAX(0, nDotL);
			final_col = final_col + diffuse;

			V3 specular; float nDotH = dotproduct(normal, half);
			specular.x = spe[0] * light_col.x * pow(MAX(0, nDotH), shi);
			specular.y = spe[1] * light_col.y * pow(MAX(0, nDotH), shi);
			specular.z = spe[2] * light_col.z * pow(MAX(0, nDotH), shi);
			final_col = final_col + specular;
		}
	}

	// calibrating each color attributes (0~1)
	final_col.x = (final_col.x > 1.0) ? 1.0 : final_col.x;
	final_col.y = (final_col.y > 1.0) ? 1.0 : final_col.y;
	final_col.z = (final_col.z > 1.0) ? 1.0 : final_col.z;	

	ret.r = final_col.x;
	ret.g = final_col.y;
	ret.b = final_col.z;

	return ret;
}