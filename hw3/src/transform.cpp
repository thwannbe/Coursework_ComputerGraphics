#include "transform.h"
#include <math.h>
#include "mymath.h"

// Helper rotation function.  Please implement this.  
M33 myRotate(const float degrees, const V3& axis)
{
	float radians = -(pi / 180) * degrees;
	M33 ret; ret.getval(1, 0, 0, 0, 1, 0, 0, 0, 1);
	ret = ret * cos(radians);
	M33 tmp; tmp.getval(axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, axis.x * axis.y, axis.y * axis.y, axis.y * axis.z, axis.x * axis.z, axis.y * axis.z, axis.z * axis.z);
	tmp = tmp * (1 - cos(radians));
	ret = ret + tmp;
	tmp.getval(0, axis.z, -axis.y, -axis.z, 0, axis.x, axis.y, -axis.x, 0);
	tmp = tmp * sin(radians);
	ret = ret + tmp;
	return ret.inverse();
}

M44 myScale(const float& sx, const float& sy, const float& sz)
{
	M44 ret;
	ret.getval(sx, 0, 0, 0, \
		0, sy, 0, 0, \
		0, 0, sz, 0, \
		0, 0, 0, 1);

	return ret;
}

M44 myTranslate(const float& tx, const float& ty, const float& tz)
{
	M44 ret;
	ret.getval(1, 0, 0, 0, \
		0, 1, 0, 0, \
		0, 0, 1, 0, \
		tx, ty, tz, 1);
	return ret;
}