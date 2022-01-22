#pragma once
typedef __int16					int16;
typedef unsigned __int16		uint16;
typedef __int32					int32;
typedef unsigned __int32		uint32;
typedef __int64					int64;
typedef unsigned __int64		uint64;


typedef intptr_t				intp;
typedef uintptr_t				uintp;
#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)
#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#define MAX_AREA_STATE_BYTES		32
#define MAX_AREA_PORTAL_STATE_BYTES 24

#define  Assert( _exp )										((void)0)
#define  AssertAligned( ptr )								((void)0)
#define  AssertOnce( _exp )									((void)0)
#define  AssertMsg( _exp, _msg )							((void)0)
#define  AssertMsgOnce( _exp, _msg )						((void)0)
#define  AssertFunc( _exp, _f )								((void)0)
#define  AssertEquals( _exp, _expectedValue )				((void)0)
#define  AssertFloatEquals( _exp, _expectedValue, _tol )	((void)0)
#define  Verify( _exp )										(_exp)
#define  VerifyEquals( _exp, _expectedValue )           	(_exp)

struct Vector2D
{
public:
	float x, y;

	Vector2D() {}
	Vector2D(float x_, float y_) { x = x_; y = y_; }
};

struct colorVec
{
	unsigned r, g, b, a;
};

class Vector
{
public:
	float x, y, z;
	Vector(void);
	Vector(float X, float Y, float Z);
	void Rotate2D(const float& f);
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	//bool IsValid() const;
	float operator[](int i) const;
	float& operator[](int i);
	inline void Zero();
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;
	__forceinline Vector& operator+=(const Vector& v);
	__forceinline Vector& operator-=(const Vector& v);
	__forceinline Vector& operator*=(const Vector& v);
	__forceinline Vector& operator*=(float s);
	__forceinline Vector& operator/=(const Vector& v);
	__forceinline Vector& operator/=(float s);
	__forceinline Vector& operator+=(float fl);
	__forceinline Vector& operator-=(float fl);
	inline float	Length() const;
	__forceinline float LengthSqr(void) const
	{
		CHECK_VALID(*this);
		return (this->x * this->x + this->y * this->y + this->z * this->z);
	}
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance);
	}
	inline Vector Cross(Vector a) { return Vector(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); }
	float	NormalizeInPlace();
	Vector	Normalize();
	__forceinline float	DistToSqr(const Vector& vOther) const;
	__forceinline float	Dist(const Vector& vOther) const;
	float	Dot(const Vector& vOther) const;
	float	Dot(const float* fOther) const;
	float	Length2D(void) const;
	float	Length2DSqr(void) const;
	Vector& operator=(const Vector& vOther);
	Vector	operator-(const Vector& v) const;
	Vector	operator+(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;
	//inline Vector AsVector2D();
	// Base address...
	float* Base();
	float const* Base() const;
};

//===============================================
inline void Vector::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}
//===============================================
inline Vector::Vector(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}

inline void SinCos(const float& r, float& s, float& c)
{
	s = sin(r);
	c = cos(r);
}

inline void Vector::Rotate2D(const float& f)
{
	float _x, _y;

	float s, c;

	SinCos(DEG2RAD(f), s, c);

	_x = x;
	_y = y;

	x = (_x * c) - (_y * s);
	y = (_x * s) + (_y * c);
}
//===============================================
inline Vector::Vector(void) { }
//===============================================
inline void Vector::Zero()
{
	x = y = z = 0.0f;
}
//===============================================
inline void VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.0f;
}
//===============================================
inline Vector& Vector::operator=(const Vector& vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}
//===============================================
//===============================================
inline float& Vector::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
//===============================================
inline float Vector::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
//===============================================
inline bool Vector::operator==(const Vector& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}
//===============================================
inline bool Vector::operator!=(const Vector& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}
//===============================================
__forceinline void VectorCopy(const Vector& src, Vector& dst)
{
	CHECK_VALID(src);
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
//===============================================
__forceinline  Vector& Vector::operator+=(const Vector& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator-=(const Vector& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator*=(const Vector& v)
{
	CHECK_VALID(v);
	x *= v.x;
	y *= v.y;
	z *= v.z;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
__forceinline Vector& Vector::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
__forceinline Vector& Vector::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}

//===============================================
__forceinline  Vector& Vector::operator/=(const Vector& v)
{
	CHECK_VALID(v);
	Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline float Vector::Length(void) const
{
	CHECK_VALID(*this);

	float root = 0.0f;

	float sqsr = x * x + y * y + z * z;

	__asm sqrtss xmm0, sqsr
	__asm movss root, xmm0

	return root;
}
//===============================================
inline float Vector::Length2D(void) const
{
	CHECK_VALID(*this);

	float root = 0.0f;

	float sqst = x * x + y * y;

	__asm
	{
		sqrtss xmm0, sqst
		movss root, xmm0
	}

	return root;
}
//===============================================
inline float Vector::Length2DSqr(void) const
{
	return (x * x + y * y);
}
//===============================================
inline Vector CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
//===============================================
float Vector::DistToSqr(const Vector& vOther) const
{
	Vector delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.LengthSqr();
}

float Vector::Dist(const Vector& vOther) const
{
	Vector delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.Length();
}

inline Vector Vector::Normalize()
{
	Vector vector;
	float length = this->Length();

	if (length != 0) {
		vector.x = x / length;
		vector.y = y / length;
		vector.z = z / length;
	}
	else
		vector.x = vector.y = 0.0f; vector.z = 1.0f;

	return vector;
}

//===============================================
inline float Vector::NormalizeInPlace()
{
	Vector& v = *this;

	float iradius = 1.f / (this->Length() + 1.192092896e-07F); //FLT_EPSILON

	v.x *= iradius;
	v.y *= iradius;
	v.z *= iradius;

	return iradius;
}
//===============================================
inline float VectorNormalize(Vector& v)
{
	Assert(v.IsValid());
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f; v.z = 1.0f;
	}
	return l;
}
//===============================================
FORCEINLINE float VectorNormalize(float* v)
{
	return VectorNormalize(*(reinterpret_cast<Vector*>(v)));
}
//===============================================
inline Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

//===============================================
inline Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}
//===============================================
inline Vector Vector::operator*(float fl) const
{
	Vector res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}
//===============================================
inline Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}
//===============================================
inline Vector Vector::operator/(float fl) const
{
	Vector res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}
//===============================================
inline Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}
inline float Vector::Dot(const Vector& vOther) const
{
	const Vector& a = *this;

	return(a.x * vOther.x + a.y * vOther.y + a.z * vOther.z);
}

inline float Vector::Dot(const float* fOther) const
{
	const Vector& a = *this;

	return(a.x * fOther[0] + a.y * fOther[1] + a.z * fOther[2]);
}

inline float* Vector::Base()
{
	return (float*)this;
}

inline float const* Vector::Base() const
{
	return (float const*)this;
}

class QAngleByValue;
class QAngle
{
public:
	float x, y, z;

	QAngle(void);
	QAngle(float X, float Y, float Z);

	operator QAngleByValue& () { return *((QAngleByValue*)(this)); }
	operator const QAngleByValue& () const { return *((const QAngleByValue*)(this)); }

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	void Random(float minVal, float maxVal);

	bool IsValid() const;
	void Invalidate();

	bool IsZero()
	{
		CHECK_VALID(*this);
		if (this->x == 0.f && this->y == 0.f && this->z == 0.f)
			return true;

		return false;
	}

	float operator[](int i) const;
	float& operator[](int i);

	float* Base();
	float const* Base() const;

	bool operator==(const QAngle& v) const;
	bool operator!=(const QAngle& v) const;

	QAngle& operator+=(const QAngle& v);
	QAngle& operator-=(const QAngle& v);
	QAngle& operator*=(float s);
	QAngle& operator/=(float s);

	float   Length() const;
	float   LengthSqr() const;

	QAngle& operator=(const QAngle& src);

	QAngle  operator-(void) const;

	QAngle  operator+(const QAngle& v) const;
	QAngle  operator-(const QAngle& v) const;
	QAngle  operator*(float fl) const;
	QAngle  operator/(float fl) const;

	QAngle Clamp();
	QAngle Mod(float N);

	inline QAngle Normalize()
	{
		QAngle vector;
		float length = this->Length();

		if (length != 0) {
			vector.x = x / length;
			vector.y = y / length;
			vector.z = z / length;
		}
		else
			vector.x = vector.y = 0.0f; vector.z = 1.0f;

		return vector;
	}

	Vector Direction()
	{
		return Vector(cos(y * M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin(y * M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin(-x * M_PI / 180.0f)).Normalize();
	}

	QAngle Normalized()
	{
		if (this->x != this->x)
			this->x = 0;
		if (this->y != this->y)
			this->y = 0;
		if (this->z != this->z)
			this->z = 0;

		if (this->x > 89.f)
			this->x = 89.f;
		if (this->x < -89.f)
			this->x = -89.f;

		while (this->y > 180)
			this->y -= 360;
		while (this->y <= -180)
			this->y += 360;

		if (this->y > 180.f)
			this->y = 180.f;
		if (this->y < -180.f)
			this->y = -180.f;

		this->z = 0;

		return *this;
	}
};

//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
inline QAngle::QAngle(void)
{
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
	// Initialize to NAN to catch errors
	x = y = z = float_NAN;
#endif
#endif
}

inline QAngle::QAngle(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------
inline void QAngle::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}

inline void QAngle::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator=(const QAngle& vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------
inline bool QAngle::operator==(const QAngle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool QAngle::operator!=(const QAngle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator+=(const QAngle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline QAngle& QAngle::operator-=(const QAngle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline QAngle& QAngle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}

inline QAngle& QAngle::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* QAngle::Base()
{
	return (float*)this;
}

inline float const* QAngle::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& QAngle::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

inline float QAngle::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float QAngle::Length() const
{
	CHECK_VALID(*this);
	return (float)sqrt(LengthSqr()); //todo replace with fastsqrt
}


inline float QAngle::LengthSqr() const
{
	CHECK_VALID(*this);
	return x * x + y * y;
}

inline QAngle QAngle::operator-(void) const
{
	return QAngle(-x, -y, -z);
}

inline QAngle QAngle::operator+(const QAngle& v) const
{
	QAngle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline QAngle QAngle::operator-(const QAngle& v) const
{
	QAngle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline QAngle QAngle::operator*(float fl) const
{
	QAngle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline QAngle QAngle::operator/(float fl) const
{
	QAngle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline QAngle QAngle::Clamp()
{
	CHECK_VALID(*this);

	if (this->x < -89.0f)
		this->x = -89.0f;

	if (this->x > 89.0f)
		this->x = 89.0f;

	while (this->y < -180.0f)
		this->y += 360.0f;

	while (this->y > 180.0f)
		this->y -= 360.0f;

	if (this->z < -49.0f)
		this->z = -49.0f;

	if (this->z > 49.0f)
		this->z = 49.0f;


	return *this;
}

inline QAngle QAngle::Mod(float N)
{
	CHECK_VALID(*this);
	this->x = fmod(x, N);
	this->y = fmod(y, N);
	this->z = fmod(z, N);

	return *this;
}