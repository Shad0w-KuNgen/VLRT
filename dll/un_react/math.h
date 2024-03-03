#pragma once
#include <xmmintrin.h>

float XM_PI = 3.141592654f;
float XM_2PI = 6.283185307f;
float XM_1DIVPI = 0.318309886f;
float XM_1DIV2PI = 0.159154943f;
float XM_PIDIV2 = 1.570796327f;
float XM_PIDIV4 = 0.785398163f;

#define M_PI       3.14159265358979323846   // pi

#define PI 3.1415926535897932f
#define FLOAT_NON_FRACTIONAL 8388608.f /* All single-precision floating point numbers greater than or equal to this have no fractional value. */
#define INV_PI 0.31830988618f
#define HALF_PI 1.57079632679f
#define DEG_TO_RAD PI / 180.f
#define RADS_DIVIDED_BY_2 DEG_TO_RAD / 2.f

#define min(a,b) a < b ? a : b
#define max(a,b) a > b ? a : b


float __sqrtf(float _X) { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(_X))); }
float __fabs(float x)
{
	if (x < 0)
		return -x;

	return x;
}
float __atan2f(float y, float x)
{
	float t0, t1, t3, t4;

	t3 = __fabs(x);
	t1 = __fabs(y);
	t0 = max(t3, t1);
	t1 = min(t3, t1);
	t3 = float(1) / t0;
	t3 = t1 * t3;

	t4 = t3 * t3;
	t0 = -float(0.013480470);
	t0 = t0 * t4 + float(0.057477314);
	t0 = t0 * t4 - float(0.121239071);
	t0 = t0 * t4 + float(0.195635925);
	t0 = t0 * t4 - float(0.332994597);
	t0 = t0 * t4 + float(0.999995630);
	t3 = t0 * t3;

	t3 = (__fabs(y) > __fabs(x)) ? float(1.570796327) - t3 : t3;
	t3 = (x < 0) ? float(3.141592654) - t3 : t3;
	t3 = (y < 0) ? -t3 : t3;

	return t3;
}

inline float __acosf
(
	float Value
)
{
	// Clamp input to [-1,1].
	bool nonnegative = (Value >= 0.0f);
	float x = __fabs(Value);
	float omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}
	float root = __sqrtf(omx);

	// 7-degree minimax approximation
	float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
	result *= root;

	// acos(x) = pi - acos(-x) when x < 0
	return (nonnegative ? result : XM_PI - result);
}

float __cosf(float Value)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = XM_1DIV2PI * Value;
	if (Value >= 0.0f)
	{
		quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
	}
	else
	{
		quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
	}
	float y = Value - XM_2PI * quotient;

	// Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
	float sign;
	if (y > XM_PIDIV2)
	{
		y = XM_PI - y;
		sign = -1.0f;
	}
	else if (y < -XM_PIDIV2)
	{
		y = -XM_PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	// 10-degree minimax approximation
	float y2 = y * y;
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	return sign * p;
}

float __sinf(float Value)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = XM_1DIV2PI * Value;
	if (Value >= 0.0f)
	{
		quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
	}
	else
	{
		quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
	}
	float y = Value - XM_2PI * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	if (y > XM_PIDIV2)
	{
		y = XM_PI - y;
	}
	else if (y < -XM_PIDIV2)
	{
		y = -XM_PI - y;
	}

	// 11-degree minimax approximation
	float y2 = y * y;
	return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
}

#define EXP_A 184
#define EXP_C 16249 

float EXP(float y)
{
	union
	{
		float d;
		struct
		{
#ifdef LITTLE_ENDIAN
			short j, i;
#else
			short i, j;
#endif
		} n;
	} eco;
	eco.n.i = EXP_A * (y)+(EXP_C);
	eco.n.j = 0;
	return eco.d;
}

float LOG(float y)
{
	int* nTemp = (int*)&y;
	y = (*nTemp) >> 16;
	return (y - EXP_C) / EXP_A;
}

float __powf(float b, float p)
{
	return EXP(LOG(b) * p);
}