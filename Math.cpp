#include "Cheat.h"


void math::vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
    out[0] = in1.Dot(in2[0]) + in2[0][3];
    out[1] = in1.Dot(in2[1]) + in2[1][3];
    out[2] = in1.Dot(in2[2]) + in2[2][3];
}

Vector math::vector_transform(Vector in, matrix3x4_t matrix) {
	return Vector(in.Dot(matrix[0]) + matrix[0][3], in.Dot(matrix[1]) + matrix[1][3],
		in.Dot(matrix[2]) + matrix[2][3]);
}

void math::AngleVectors(const QAngle& angles, Vector& forward)
{
    float	sp, sy, cp, cy;

    SinCos(DEG2RAD(angles[1]), sy, cy);
    SinCos(DEG2RAD(angles[0]), sp, cp);

    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;
}
float math::VectorDistance(Vector v1, Vector v2) {
	return sqrtf((float)(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2)));
}
void math::AngleVectors(const QAngle& angles, Vector& forward, Vector& right, Vector& up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), sy, cy);
	SinCos(DEG2RAD(angles[0]), sp, cp);
	SinCos(DEG2RAD(angles[2]), sr, cr);

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}
void math::AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), sy, cy);
	SinCos(DEG2RAD(angles[0]), sp, cp);
	SinCos(DEG2RAD(angles[2]), sr, cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

QAngle math::CalcAngle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;
	VectorAngles(delta, angles);
	delta.Normalize();
	return angles;
}

void math::VectorAngles(const Vector& forward, QAngle& angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}
}

void math::VectorAngles(const Vector& forward, Vector& up, QAngle& angles)
{
	Vector left = CrossProduct(up, forward);
	left.NormalizeInPlace();

	float forwardDist = forward.Length2D();

	if (forwardDist > 0.001f)
	{
		angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
		angles.y = atan2f(forward.y, forward.x) * 180 / M_PI;

		float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles.z = atan2f(left.z, upZ) * 180 / M_PI;
	}
	else
	{
		angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
		angles.y = atan2f(-left.x, left.y) * 180 / M_PI;
		angles.z = 0;
	}
}

float math::NormalizeFloat(float result)
{
	while (result > 180.0f)
		result -= 360.0f;

	while (result < -180.0f)
		result += 360.0f;

	return result;
}

float math::NormalizeAngle(float angle) {
	if (angle > 180.f || angle < -180.f) {
		auto revolutions = std::round(std::abs(angle / 360.f));

		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}
	return angle;
}