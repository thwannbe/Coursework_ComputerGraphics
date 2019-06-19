#pragma once

#include <iostream>
using namespace std;

const float pi = 3.14159265; // For portability across platforms
#define ABS(x) (((x) < 0) ? (-(x)) : (x))

struct ImageSize {
	int w, h;
};

struct V3 {
	float x, y, z;

	V3 operator + (V3& v) {
		V3 ret;
		ret.x = this->x + v.x;
		ret.y = this->y + v.y;
		ret.z = this->z + v.z;
		return ret;
	}

	V3 operator - (V3& v) {
		V3 ret;
		ret.x = this->x - v.x;
		ret.y = this->y - v.y;
		ret.z = this->z - v.z;
		return ret;
	}

	V3 operator * (float f) {
		V3 ret;
		ret.x = this->x * f;
		ret.y = this->y * f;
		ret.z = this->z * f;
		return ret;
	}

	void print(void) {
		cout << "x " << x << " y " << y << " z " << z << endl;
	}
};

struct V4 {
	float x, y, z, w;

	V4 operator + (V4& v) {
		V4 ret;
		ret.x = this->x + v.x;
		ret.y = this->y + v.y;
		ret.z = this->z + v.z;
		ret.w = this->w + v.w;
		return ret;
	}

	V4 operator - (V4& v) {
		V4 ret;
		ret.x = this->x - v.x;
		ret.y = this->y - v.y;
		ret.z = this->z - v.z;
		ret.w = this->w - v.w;
		return ret;
	}

	V4 operator * (float f) {
		V4 ret;
		ret.x = this->x * f;
		ret.y = this->y * f;
		ret.z = this->z * f;
		ret.w = this->w * f;
		return ret;
	}

	void wnormalize(void) {
		if (this->w != 0) {
			this->x /= this->w;
			this->y /= this->w;
			this->z /= this->w;
			this->w /= this->w;
		}
	}
};

struct M33 {
	float mat[3][3];

	void getval(float v00, float v10, float v20, float v01, float v11, float v21, float v02, float v12, float v22) {
		this->mat[0][0] = v00;
		this->mat[0][1] = v01;
		this->mat[0][2] = v02;

		this->mat[1][0] = v10;
		this->mat[1][1] = v11;
		this->mat[1][2] = v12;

		this->mat[2][0] = v20;
		this->mat[2][1] = v21;
		this->mat[2][2] = v22;
	}

	M33 operator + (M33& m) {
		M33 ret;
		ret.mat[0][0] = this->mat[0][0] + m.mat[0][0];
		ret.mat[0][1] = this->mat[0][1] + m.mat[0][1];
		ret.mat[0][2] = this->mat[0][2] + m.mat[0][2];

		ret.mat[1][0] = this->mat[1][0] + m.mat[1][0];
		ret.mat[1][1] = this->mat[1][1] + m.mat[1][1];
		ret.mat[1][2] = this->mat[1][2] + m.mat[1][2];

		ret.mat[2][0] = this->mat[2][0] + m.mat[2][0];
		ret.mat[2][1] = this->mat[2][1] + m.mat[2][1];
		ret.mat[2][2] = this->mat[2][2] + m.mat[2][2];
		return ret;
	}

	M33 operator - (M33& m) {
		M33 ret;
		ret.mat[0][0] = this->mat[0][0] - m.mat[0][0];
		ret.mat[0][1] = this->mat[0][1] - m.mat[0][1];
		ret.mat[0][2] = this->mat[0][2] - m.mat[0][2];

		ret.mat[1][0] = this->mat[1][0] - m.mat[1][0];
		ret.mat[1][1] = this->mat[1][1] - m.mat[1][1];
		ret.mat[1][2] = this->mat[1][2] - m.mat[1][2];

		ret.mat[2][0] = this->mat[2][0] - m.mat[2][0];
		ret.mat[2][1] = this->mat[2][1] - m.mat[2][1];
		ret.mat[2][2] = this->mat[2][2] - m.mat[2][2];
		return ret;
	}

	M33 operator * (float f) {
		M33 ret;
		ret.mat[0][0] = f * this->mat[0][0];
		ret.mat[0][1] = f * this->mat[0][1];
		ret.mat[0][2] = f * this->mat[0][2];

		ret.mat[1][0] = f * this->mat[1][0];
		ret.mat[1][1] = f * this->mat[1][1];
		ret.mat[1][2] = f * this->mat[1][2];

		ret.mat[2][0] = f * this->mat[2][0];
		ret.mat[2][1] = f * this->mat[2][1];
		ret.mat[2][2] = f * this->mat[2][2];
		return ret;
	}

	V3 operator * (V3 v) {
		V3 ret;
		ret.x = this->mat[0][0] * v.x + this->mat[0][1] * v.y + this->mat[0][2] * v.z;
		ret.y = this->mat[1][0] * v.x + this->mat[1][1] * v.y + this->mat[1][2] * v.z;
		ret.z = this->mat[2][0] * v.x + this->mat[2][1] * v.y + this->mat[2][2] * v.z;
		return ret;
	}

	M33 operator * (M33 m) {
		M33 ret;
		ret.mat[0][0] = this->mat[0][0] * m.mat[0][0] + this->mat[0][1] * m.mat[1][0] + this->mat[0][2] * m.mat[2][0];
		ret.mat[0][1] = this->mat[0][0] * m.mat[0][1] + this->mat[0][1] * m.mat[1][1] + this->mat[0][2] * m.mat[2][1];
		ret.mat[0][2] = this->mat[0][0] * m.mat[0][2] + this->mat[0][1] * m.mat[1][2] + this->mat[0][2] * m.mat[2][2];

		ret.mat[1][0] = this->mat[1][0] * m.mat[0][0] + this->mat[1][1] * m.mat[1][0] + this->mat[1][2] * m.mat[2][0];
		ret.mat[1][1] = this->mat[1][0] * m.mat[0][1] + this->mat[1][1] * m.mat[1][1] + this->mat[1][2] * m.mat[2][1];
		ret.mat[1][2] = this->mat[1][0] * m.mat[0][2] + this->mat[1][1] * m.mat[1][2] + this->mat[1][2] * m.mat[2][2];

		ret.mat[2][0] = this->mat[2][0] * m.mat[0][0] + this->mat[2][1] * m.mat[1][0] + this->mat[2][2] * m.mat[2][0];
		ret.mat[2][1] = this->mat[2][0] * m.mat[0][1] + this->mat[2][1] * m.mat[1][1] + this->mat[2][2] * m.mat[2][1];
		ret.mat[2][2] = this->mat[2][0] * m.mat[0][2] + this->mat[2][1] * m.mat[1][2] + this->mat[2][2] * m.mat[2][2];
		return ret;
	}

	M33 inverse (void) {
		float S00 = this->mat[0][0];
		float S01 = this->mat[0][1];
		float S02 = this->mat[0][2];

		float S10 = this->mat[1][0];
		float S11 = this->mat[1][1];
		float S12 = this->mat[1][2];

		float S20 = this->mat[2][0];
		float S21 = this->mat[2][1];
		float S22 = this->mat[2][2];

		M33 ret;
		ret.getval(
			S11 * S22 - S21 * S12,
			S12 * S20 - S22 * S10,
			S10 * S21 - S20 * S11,
			S02 * S21 - S01 * S22,
			S00 * S22 - S02 * S20,
			S01 * S20 - S00 * S21,
			S12 * S01 - S11 * S02,
			S10 * S02 - S12 * S00,
			S11 * S00 - S10 * S01);
		float Determinant =
			S00 * (S11 * S22 - S21 * S12)
			- S10 * (S01 * S22 - S21 * S02)
			+ S20 * (S01 * S12 - S11 * S02);

		ret = ret * (1 / Determinant);
		return ret;
	}
};

struct M44 {
	float mat[4][4];

	void clear (void) {
		this->mat[0][0] = 0;
		this->mat[0][1] = 0;
		this->mat[0][2] = 0;
		this->mat[0][3] = 0;

		this->mat[1][0] = 0;
		this->mat[1][1] = 0;
		this->mat[1][2] = 0;
		this->mat[1][3] = 0;

		this->mat[2][0] = 0;
		this->mat[2][1] = 0;
		this->mat[2][2] = 0;
		this->mat[2][3] = 0;

		this->mat[3][0] = 0;
		this->mat[3][1] = 0;
		this->mat[3][2] = 0;
		this->mat[3][3] = 0;
	}

	void getval(float v00, float v10, float v20, float v30, float v01, float v11, float v21, float v31, float v02, float v12, float v22, float v32, float v03, float v13, float v23, float v33) {
		this->mat[0][0] = v00;
		this->mat[0][1] = v01;
		this->mat[0][2] = v02;
		this->mat[0][3] = v03;

		this->mat[1][0] = v10;
		this->mat[1][1] = v11;
		this->mat[1][2] = v12;
		this->mat[1][3] = v13;

		this->mat[2][0] = v20;
		this->mat[2][1] = v21;
		this->mat[2][2] = v22;
		this->mat[2][3] = v23;

		this->mat[3][0] = v30;
		this->mat[3][1] = v31;
		this->mat[3][2] = v32;
		this->mat[3][3] = v33;
	}

	M44 operator + (M44& m) {
		M44 ret;
		ret.mat[0][0] = this->mat[0][0] + m.mat[0][0];
		ret.mat[0][1] = this->mat[0][1] + m.mat[0][1];
		ret.mat[0][2] = this->mat[0][2] + m.mat[0][2];
		ret.mat[0][3] = this->mat[0][3] + m.mat[0][3];

		ret.mat[1][0] = this->mat[1][0] + m.mat[1][0];
		ret.mat[1][1] = this->mat[1][1] + m.mat[1][1];
		ret.mat[1][2] = this->mat[1][2] + m.mat[1][2];
		ret.mat[1][3] = this->mat[1][3] + m.mat[1][3];

		ret.mat[2][0] = this->mat[2][0] + m.mat[2][0];
		ret.mat[2][1] = this->mat[2][1] + m.mat[2][1];
		ret.mat[2][2] = this->mat[2][2] + m.mat[2][2];
		ret.mat[2][3] = this->mat[2][3] + m.mat[2][3];

		ret.mat[3][0] = this->mat[3][0] + m.mat[3][0];
		ret.mat[3][1] = this->mat[3][1] + m.mat[3][1];
		ret.mat[3][2] = this->mat[3][2] + m.mat[3][2];
		ret.mat[3][3] = this->mat[3][3] + m.mat[3][3];
		return ret;
	}

	M44 operator - (M44& m) {
		M44 ret;
		ret.mat[0][0] = this->mat[0][0] - m.mat[0][0];
		ret.mat[0][1] = this->mat[0][1] - m.mat[0][1];
		ret.mat[0][2] = this->mat[0][2] - m.mat[0][2];
		ret.mat[0][3] = this->mat[0][3] - m.mat[0][3];

		ret.mat[1][0] = this->mat[1][0] - m.mat[1][0];
		ret.mat[1][1] = this->mat[1][1] - m.mat[1][1];
		ret.mat[1][2] = this->mat[1][2] - m.mat[1][2];
		ret.mat[1][3] = this->mat[1][3] - m.mat[1][3];

		ret.mat[2][0] = this->mat[2][0] - m.mat[2][0];
		ret.mat[2][1] = this->mat[2][1] - m.mat[2][1];
		ret.mat[2][2] = this->mat[2][2] - m.mat[2][2];
		ret.mat[2][3] = this->mat[2][3] - m.mat[2][3];

		ret.mat[3][0] = this->mat[3][0] - m.mat[3][0];
		ret.mat[3][1] = this->mat[3][1] - m.mat[3][1];
		ret.mat[3][2] = this->mat[3][2] - m.mat[3][2];
		ret.mat[3][3] = this->mat[3][3] - m.mat[3][3];
		return ret;
	}

	M44 operator * (float f) {
		M44 ret;
		ret.mat[0][0] = f * this->mat[0][0];
		ret.mat[0][1] = f * this->mat[0][1];
		ret.mat[0][2] = f * this->mat[0][2];
		ret.mat[0][3] = f * this->mat[0][3];

		ret.mat[1][0] = f * this->mat[1][0];
		ret.mat[1][1] = f * this->mat[1][1];
		ret.mat[1][2] = f * this->mat[1][2];
		ret.mat[1][3] = f * this->mat[1][3];

		ret.mat[2][0] = f * this->mat[2][0];
		ret.mat[2][1] = f * this->mat[2][1];
		ret.mat[2][2] = f * this->mat[2][2];
		ret.mat[2][3] = f * this->mat[2][3];

		ret.mat[3][0] = f * this->mat[3][0];
		ret.mat[3][1] = f * this->mat[3][1];
		ret.mat[3][2] = f * this->mat[3][2];
		ret.mat[3][3] = f * this->mat[3][3];
		return ret;
	}

	V4 operator * (V4 v) {
		V4 ret;
		ret.x = this->mat[0][0] * v.x + this->mat[0][1] * v.y + this->mat[0][2] * v.z + this->mat[0][3] * v.w;
		ret.y = this->mat[1][0] * v.x + this->mat[1][1] * v.y + this->mat[1][2] * v.z + this->mat[1][3] * v.w;
		ret.z = this->mat[2][0] * v.x + this->mat[2][1] * v.y + this->mat[2][2] * v.z + this->mat[2][3] * v.w;
		ret.w = this->mat[3][0] * v.x + this->mat[3][1] * v.y + this->mat[3][2] * v.z + this->mat[3][3] * v.w;
		return ret;
	}

	M44 operator * (M44 m) {
		M44 ret;
		ret.mat[0][0] = this->mat[0][0] * m.mat[0][0] + this->mat[0][1] * m.mat[1][0] + this->mat[0][2] * m.mat[2][0] + this->mat[0][3] * m.mat[3][0];
		ret.mat[0][1] = this->mat[0][0] * m.mat[0][1] + this->mat[0][1] * m.mat[1][1] + this->mat[0][2] * m.mat[2][1] + this->mat[0][3] * m.mat[3][1];
		ret.mat[0][2] = this->mat[0][0] * m.mat[0][2] + this->mat[0][1] * m.mat[1][2] + this->mat[0][2] * m.mat[2][2] + this->mat[0][3] * m.mat[3][2];
		ret.mat[0][3] = this->mat[0][0] * m.mat[0][3] + this->mat[0][1] * m.mat[1][3] + this->mat[0][2] * m.mat[2][3] + this->mat[0][3] * m.mat[3][3];

		ret.mat[1][0] = this->mat[1][0] * m.mat[0][0] + this->mat[1][1] * m.mat[1][0] + this->mat[1][2] * m.mat[2][0] + this->mat[1][3] * m.mat[3][0];
		ret.mat[1][1] = this->mat[1][0] * m.mat[0][1] + this->mat[1][1] * m.mat[1][1] + this->mat[1][2] * m.mat[2][1] + this->mat[1][3] * m.mat[3][1];
		ret.mat[1][2] = this->mat[1][0] * m.mat[0][2] + this->mat[1][1] * m.mat[1][2] + this->mat[1][2] * m.mat[2][2] + this->mat[1][3] * m.mat[3][2];
		ret.mat[1][3] = this->mat[1][0] * m.mat[0][3] + this->mat[1][1] * m.mat[1][3] + this->mat[1][2] * m.mat[2][3] + this->mat[1][3] * m.mat[3][3];

		ret.mat[2][0] = this->mat[2][0] * m.mat[0][0] + this->mat[2][1] * m.mat[1][0] + this->mat[2][2] * m.mat[2][0] + this->mat[2][3] * m.mat[3][0];
		ret.mat[2][1] = this->mat[2][0] * m.mat[0][1] + this->mat[2][1] * m.mat[1][1] + this->mat[2][2] * m.mat[2][1] + this->mat[2][3] * m.mat[3][1];
		ret.mat[2][2] = this->mat[2][0] * m.mat[0][2] + this->mat[2][1] * m.mat[1][2] + this->mat[2][2] * m.mat[2][2] + this->mat[2][3] * m.mat[3][2];
		ret.mat[2][3] = this->mat[2][0] * m.mat[0][3] + this->mat[2][1] * m.mat[1][3] + this->mat[2][2] * m.mat[2][3] + this->mat[2][3] * m.mat[3][3];

		ret.mat[3][0] = this->mat[3][0] * m.mat[0][0] + this->mat[3][1] * m.mat[1][0] + this->mat[3][2] * m.mat[2][0] + this->mat[3][3] * m.mat[3][0];
		ret.mat[3][1] = this->mat[3][0] * m.mat[0][1] + this->mat[3][1] * m.mat[1][1] + this->mat[3][2] * m.mat[2][1] + this->mat[3][3] * m.mat[3][1];
		ret.mat[3][2] = this->mat[3][0] * m.mat[0][2] + this->mat[3][1] * m.mat[1][2] + this->mat[3][2] * m.mat[2][2] + this->mat[3][3] * m.mat[3][2];
		ret.mat[3][3] = this->mat[3][0] * m.mat[0][3] + this->mat[3][1] * m.mat[1][3] + this->mat[3][2] * m.mat[2][3] + this->mat[3][3] * m.mat[3][3];
		return ret;
	}

	M44 inverse(void) {
		// Calculate all mat2 determinants
		float SubFactor00 = this->mat[2][2] * this->mat[3][3] - this->mat[3][2] * this->mat[2][3];
		float SubFactor01 = this->mat[2][1] * this->mat[3][3] - this->mat[3][1] * this->mat[2][3];
		float SubFactor02 = this->mat[2][1] * this->mat[3][2] - this->mat[3][1] * this->mat[2][2];
		float SubFactor03 = this->mat[2][0] * this->mat[3][3] - this->mat[3][0] * this->mat[2][3];
		float SubFactor04 = this->mat[2][0] * this->mat[3][2] - this->mat[3][0] * this->mat[2][2];
		float SubFactor05 = this->mat[2][0] * this->mat[3][1] - this->mat[3][0] * this->mat[2][1];
		float SubFactor06 = this->mat[1][2] * this->mat[3][3] - this->mat[3][2] * this->mat[1][3];
		float SubFactor07 = this->mat[1][1] * this->mat[3][3] - this->mat[3][1] * this->mat[1][3];
		float SubFactor08 = this->mat[1][1] * this->mat[3][2] - this->mat[3][1] * this->mat[1][2];
		float SubFactor09 = this->mat[1][0] * this->mat[3][3] - this->mat[3][0] * this->mat[1][3];
		float SubFactor10 = this->mat[1][0] * this->mat[3][2] - this->mat[3][0] * this->mat[1][2];
		float SubFactor11 = this->mat[1][1] * this->mat[3][3] - this->mat[3][1] * this->mat[1][3];
		float SubFactor12 = this->mat[1][0] * this->mat[3][1] - this->mat[3][0] * this->mat[1][1];
		float SubFactor13 = this->mat[1][2] * this->mat[2][3] - this->mat[2][2] * this->mat[1][3];
		float SubFactor14 = this->mat[1][1] * this->mat[2][3] - this->mat[2][1] * this->mat[1][3];
		float SubFactor15 = this->mat[1][1] * this->mat[2][2] - this->mat[2][1] * this->mat[1][2];
		float SubFactor16 = this->mat[1][0] * this->mat[2][3] - this->mat[2][0] * this->mat[1][3];
		float SubFactor17 = this->mat[1][0] * this->mat[2][2] - this->mat[2][0] * this->mat[1][2];
		float SubFactor18 = this->mat[1][0] * this->mat[2][1] - this->mat[2][0] * this->mat[1][1];

		M44 ret;
		ret.getval(
			+this->mat[1][1] * SubFactor00 - this->mat[1][2] * SubFactor01 + this->mat[1][3] * SubFactor02,
			-this->mat[1][0] * SubFactor00 + this->mat[1][2] * SubFactor03 - this->mat[1][3] * SubFactor04,
			+this->mat[1][0] * SubFactor01 - this->mat[1][1] * SubFactor03 + this->mat[1][3] * SubFactor05,
			-this->mat[1][0] * SubFactor02 + this->mat[1][1] * SubFactor04 - this->mat[1][2] * SubFactor05,

			-this->mat[0][1] * SubFactor00 + this->mat[0][2] * SubFactor01 - this->mat[0][3] * SubFactor02,
			+this->mat[0][0] * SubFactor00 - this->mat[0][2] * SubFactor03 + this->mat[0][3] * SubFactor04,
			-this->mat[0][0] * SubFactor01 + this->mat[0][1] * SubFactor03 - this->mat[0][3] * SubFactor05,
			+this->mat[0][0] * SubFactor02 - this->mat[0][1] * SubFactor04 + this->mat[0][2] * SubFactor05,

			+this->mat[0][1] * SubFactor06 - this->mat[0][2] * SubFactor07 + this->mat[0][3] * SubFactor08,
			-this->mat[0][0] * SubFactor06 + this->mat[0][2] * SubFactor09 - this->mat[0][3] * SubFactor10,
			+this->mat[0][0] * SubFactor11 - this->mat[0][1] * SubFactor09 + this->mat[0][3] * SubFactor12,
			-this->mat[0][0] * SubFactor08 + this->mat[0][1] * SubFactor10 - this->mat[0][2] * SubFactor12,

			-this->mat[0][1] * SubFactor13 + this->mat[0][2] * SubFactor14 - this->mat[0][3] * SubFactor15,
			+this->mat[0][0] * SubFactor13 - this->mat[0][2] * SubFactor16 + this->mat[0][3] * SubFactor17,
			-this->mat[0][0] * SubFactor14 + this->mat[0][1] * SubFactor16 - this->mat[0][3] * SubFactor18,
			+this->mat[0][0] * SubFactor15 - this->mat[0][1] * SubFactor17 + this->mat[0][2] * SubFactor18);

		float Determinant =
			+this->mat[0][0] * ret.mat[0][0]
			+ this->mat[0][1] * ret.mat[1][0]
			+ this->mat[0][2] * ret.mat[2][0]
			+ this->mat[0][3] * ret.mat[3][0];

		ret = ret * (1 / Determinant);
		return ret;
	}
};

struct Ray {
	V3 p0, p1;
};

struct Camera {
	V3 lf, la, u;
	float fovy;
	float fovx;
};

struct PositionLight{
	V3 pos;
	float col[3]; // rgb
};

struct DirectionalLight {
	V3 dir;
	float col[3]; // rgb
};

struct Triangle {
	V3 v0, v1, v2;

	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emission[3];
	float shininess;

	M44 transform;
};

struct Sphere {
	V3 center;
	float radius;

	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emission[3];
	float shininess;

	M44 transform;
};

struct Color {
	float r, g, b; // scale from 0 ~ 1
};