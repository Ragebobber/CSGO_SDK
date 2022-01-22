#pragma once
namespace math {
	extern void vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	extern Vector vector_transform(Vector in, matrix3x4_t matrix);
	extern void AngleVectors(const QAngle& angles,  Vector& forward);
	extern float VectorDistance(Vector v1, Vector v2);
	extern void AngleVectors(const QAngle& angles, Vector& forward, Vector& right, Vector& up);
	extern void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
	extern QAngle CalcAngle(Vector src, Vector dst);
	extern void VectorAngles(const Vector& forward, QAngle& angles);
	extern void VectorAngles(const Vector& forward, Vector& up, QAngle& angles);
	extern float NormalizeFloat(float res);
	extern float NormalizeAngle(float angle);

}