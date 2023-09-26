#include "base_stdafx.h"
#include "Quat.h"

#define RAD2DEG (180.0f / PI)

Quat::Quat(CONST FLOAT * pf) : Base(pf)
{
}

Quat::Quat(CONST D3DXFLOAT16 * pf) : Base(pf)
{
}

Quat::Quat(CONST D3DXQUATERNION & q) : Base(q)
{
}

Quat::Quat(CONST Quat & q) : Base(q)
{
}

Quat::Quat(FLOAT x, FLOAT y, FLOAT z, FLOAT w) : Base(x, y, z, w)
{
}

Quat::operator FLOAT*()
{
	return Base::operator FLOAT*();
}

Quat::operator CONST FLOAT*() const
{
	return Base::operator const FLOAT *();
}

Quat & Quat::operator+=(CONST Quat & q)
{
	D3DXQUATERNION Q = Base::operator+=(q);
	x = Q.x;
	y = Q.y;
	z = Q.z;
	w = Q.w;
	return *this;
}

Quat & Quat::operator-=(CONST Quat & q)
{
	D3DXQUATERNION Q = Base::operator-=(q);
	x = Q.x;
	y = Q.y;
	z = Q.z;
	w = Q.w;
	return *this;
}

Quat & Quat::operator*=(CONST Quat & q)
{
	D3DXQUATERNION Q = Base::operator*=(q);
	x = Q.x;
	y = Q.y;
	z = Q.z;
	w = Q.w;
	return *this;
}

Quat & Quat::operator*=(FLOAT s)
{
	D3DXQUATERNION Q = Base::operator*=(s);
	x = Q.x;
	y = Q.y;
	z = Q.z;
	w = Q.w;
	return *this;
}

Quat & Quat::operator/=(FLOAT s)
{
	D3DXQUATERNION Q = Base::operator/=(s);
	x = Q.x;
	y = Q.y;
	z = Q.z;
	w = Q.w;
	return *this;
}

Quat Quat::operator+() const
{
	return (Quat)Base::operator+();
}

Quat Quat::operator-() const
{
	return (Quat)Base::operator-();
}

Quat Quat::operator+(CONST Quat & q) const
{
	return (Quat)Base::operator+(q);
}

Quat Quat::operator-(CONST Quat & q) const
{
	return (Quat)Base::operator-(q);
}

Quat Quat::operator*(CONST Quat & q) const
{
	/*return (Quat)Base::operator*(q);*/
	const Quat& lhs = *this;
	const Quat& rhs = q;
	return Quat(
		lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

Quat Quat::operator*(FLOAT s) const
{
	return (Quat)Base::operator*(s);
}

Quat Quat::operator/(FLOAT s) const
{
	return (Quat)Base::operator/(s);
}

BOOL Quat::operator==(CONST Quat & q) const
{
	return Base::operator==(q);
}

BOOL Quat::operator!=(CONST Quat & q) const
{
	return Base::operator!=(q);
}

Quat::operator D3DXQUATERNION() const
{
	return *this;
}

D3DXVECTOR3 Quat::GetAxis() const
{
	D3DXVECTOR3 axis(x,y,z);
	D3DXVec3Normalize(&axis, &axis);
	return axis;
}

D3DXVECTOR3 Quat::GetEulerAngle() const
{
	auto NormalizeAngle = [&](float angle) -> float
	{
		while (angle > 360)
			angle -= 360;
		while (angle < 0)
			angle += 360;
		return angle;
	};
	auto NormalizeAngles = [&](D3DXVECTOR3 angles) -> D3DXVECTOR3
	{
		angles.x = NormalizeAngle(angles.x);
		angles.y = NormalizeAngle(angles.y);
		angles.z = NormalizeAngle(angles.z);
		return angles;
	};

	float sqw = w * w;
	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = x * w - y * z;
	D3DXVECTOR3 v;

	if (test > 0.4995f * unit)
	{ // singularity at north pole
		v.y = 2.0f * atan2f(y, x);
		v.x = PI / 2;
		v.z = 0;
		return NormalizeAngles(v * RAD2DEG);
	}
	if (test < -0.4995f * unit)
	{ // singularity at south pole
		v.y = -2.0f * atan2f(y, x);
		v.x = -PI / 2;
		v.z = 0;
		return NormalizeAngles(v * RAD2DEG);
	}
	Quat q(w, z, x, y);
	v.y = atan2f(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1 - 2.0f * (q.z * q.z + q.w * q.w));		// Yaw
	v.x = asinf(2.0f * (q.x * q.z - q.w * q.y));												// Pitch
	v.z = atan2f(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1 - 2.0f * (q.y * q.y + q.z * q.z));      // Roll
	return NormalizeAngles(v * RAD2DEG);
}

float Quat::sqrMagnitude() const
{
	return D3DXQuaternionLengthSq(this);
}

float Quat::magnitude() const
{
	return D3DXQuaternionLength(this);
}

Quat Quat::normalized() const
{
	Quat q = *this;
	q.Normalize();
	return q;
}

void Quat::Normalize()
{
	D3DXQuaternionNormalize(this, this);
}

Quat Quat::inversed() const
{
	Quat q = *this;
	q.Inverse();
	return q;
}

void Quat::Inverse()
{
	D3DXQuaternionInverse(this, this);
}

Mat4 Quat::ToMatrix() const
{
	// 전치행렬 처리를 해줘야 할 수도 있습니다.
	return Mat4(
		1.0f - 2.0f * y * y - 2.0f * z * z,		2.0f * x * y - 2.0f * z * w,			2.0f * x * z + 2.0f * y * w,		0.0f,
		2.0f * x * y + 2.0f * z * w,			1.0f - 2.0f * x * x - 2.0f * z * z,		2.0f * y * z - 2.0f * x * w,		0.0f,
		2.0f * x * z - 2.0f * y * w,			2.0f * y * z + 2.0f * x * w,			1.0f - 2.0f * x * x - 2.0f * y * y, 0.0f,
		0.0f,									0.0f,									0.0f,								1.0f
	);
}

float Quat::Dot(const Quat & a, const Quat & b)
{
	return D3DXQuaternionDot(&a, &b);
}

float Quat::Radian(const Quat& a, const Quat& b)
{
	float f = Dot(a, b);
	return acosf(min(fabsf(f), 1.0f)) * 2.0f;
}

float Quat::Angle(const Quat& a, const Quat& b)
{
	return 0.0f;
}

Quat Quat::one()
{
	return Quat(1, 1, 1, 1);
}

Quat Quat::zero()
{
	return Quat(0, 0, 0, 0);
}

Quat Quat::Identity()
{
	return Quat(0, 0, 0, 1);
}

Quat Quat::AxisAngle(D3DXVECTOR3 axis, float rAngle)
{
	//D3DXVec3Normalize(&axis, &axis);
	//D3DXVECTOR3 v = axis * sinf(0.5f * rAngle);
	//float w = cosf(0.5f * rAngle);
	//return Quat(v.x, v.y, v.z, w);

	D3DXVec3Normalize(&axis, &axis);
	float sina = sinf(0.5f * rAngle);
	float cosa = cosf(0.5f * rAngle);
	Vec3 v = axis * sina;
	return Quat(v.x, v.y, v.z, cosa);
}

Quat Quat::FromToRotation(const D3DXVECTOR3& from, const D3DXVECTOR3& to)
{
	// https://answers.unity.com/questions/1668856/whats-the-source-code-of-quaternionfromtorotation.html

	//D3DXVECTOR3 axis;
	//D3DXVec3Cross(&axis, &from, &to);
	//float dot = D3DXVec3Dot(&from, &to);
	//float denominator = D3DXVec3Length(&from) * D3DXVec3Length(&to);
	//float cos;
	//if (denominator == 0)
	//	cos = 0;
	//else
	//	cos = dot / denominator;
	//float rAngle = acosf(cos);
	//return AxisAngle(axis, rAngle);

	/*
	Vector3 axis = Vector3.Cross(aFrom, aTo);
	float angle = Vector3.Angle(aFrom, aTo);
	return Quaternion.AngleAxis(angle, axis.normalized);
	*/

	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &from, &to);

	float dot = D3DXVec3Dot(&from, &to);
	float denominator = D3DXVec3Length(&from) * D3DXVec3Length(&to);
	float cos;
	if (denominator == 0)
		cos = 0;
	else
		cos = dot / denominator;
	if (cos < -1.0f) cos = -1.0f;
	else if (cos > +1.0f) cos = +1.0f;
	float rAngle = acosf(cos);
	float angle = rAngle * 180.0f / D3DX_PI;

	if (angle >= 179.9196f)
	{
		D3DXVECTOR3 r;
		D3DXVECTOR3 right(1, 0, 0);
		D3DXVec3Cross(&r, &from, &right);
		D3DXVec3Cross(&axis, &r, &from);

		float axisSqrtMagnitude = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;

		if (axisSqrtMagnitude < 0.000001f)
			axis = D3DXVECTOR3(0, 0, 1);
	}

	D3DXVec3Normalize(&axis, &axis);
	return AxisAngle(axis, rAngle);
}

Quat Quat::SLerp(const Quat & a, const Quat & b, const float & t)
{
	Quat q;
	D3DXQuaternionSlerp(&q, &a, &b, t);
	return q;
}

Quat Quat::EulerToRotation(const D3DXVECTOR3& euler)
{
	Quat x = AxisAngle({ 1,0,0 }, euler.x);
	Quat y = AxisAngle({ 0,1,0 }, euler.y);
	Quat z = AxisAngle({ 0,0,1 }, euler.z);
	return z * x * y;
}

Quat Quat::RotateTowards(const Quat& from, const Quat& to, float maxDegreeDelta)
{
	float angle = Angle(from, to);
	if (angle == 0) return to;
	float t = min(1.0f, maxDegreeDelta / angle);
	return SLerp(from, to, t);
}

Quat operator*(FLOAT s, CONST Quat & q)
{
	return q*s;
}

D3DXVECTOR3 operator*(const Quat& q, const D3DXVECTOR3& v)
{
	Quat qv = Quat(v.x, v.y, v.z, 0);

	// qInv = 켤레쿼터니언 / 쿼터니언 크기의 제곱
	Quat qInv;
	D3DXQuaternionInverse(&qInv, &q);

	Quat res = q * qv * qInv;
	return D3DXVECTOR3(res.x, res.y, res.z);
}
