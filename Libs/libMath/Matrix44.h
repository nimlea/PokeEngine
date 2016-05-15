#ifndef _MATRIX44_H_
#define _MATRIX44_H_

#include "..\..\Engine\Common\PEMacro.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

#if defined PE_DIRECTX
#include <DirectXMath.h>
using namespace DirectX;
#endif

/*
 * row-major
 */
template<class T>
class Matrix44
{
public:
	T m11, m12, m13, m14;
	T m21, m22, m23, m24;
	T m31, m32, m33, m34;
	T m41, m42, m43, m44;

public:
	// 构造
	Matrix44()
	{
		m11 = 0; m12 = 0; m13 = 0; m14 = 0;
		m21 = 0; m22 = 0; m23 = 0; m24 = 0;
		m31 = 0; m32 = 0; m33 = 0; m34 = 0;
		m41 = 0; m42 = 0; m43 = 0; m44 = 0;
	}
	Matrix44(
		T _11, T _12, T _13, T _14,
		T _21, T _22, T _23, T _24,
		T _31, T _32, T _33, T _34,
		T _41, T _42, T _43, T _44)
	{
		m11 = _11; m12 = _12; m13 = _13; m14 = _14;
		m21 = _21; m22 = _22; m23 = _23; m24 = _24;
		m31 = _31; m32 = _32; m33 = _33; m34 = _34;
		m41 = _41; m42 = _42; m43 = _43; m44 = _44;
	}
	Matrix44(const Matrix44<T> &m)
	{
		m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
		m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = m.m24;
		m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
		m41 = m.m41; m42 = m.m42; m43 = m.m43; m44 = m.m44;
	}
	Matrix44(const Vector4<T>& _r1, const Vector4<T>& _r2, const Vector4<T>& _r3, const Vector4<T>& _r4)
	{
		m11 = _r1.x; m12 = _r1.y; m13 = _r1.z, m14 = _r1.w;
		m21 = _r2.x; m22 = _r2.y; m23 = _r2.z, m24 = _r2.w;
		m31 = _r3.x; m32 = _r3.y; m33 = _r3.z, m34 = _r3.w;
		m41 = _r4.x; m42 = _r4.y; m43 = _r4.z, m44 = _r4.w;
	}
	template<class T1>
	Matrix44(const Matrix44<T1>& m)
	{
		m11 = static_cast<T>(m.m11); m12 = static_cast<T>(m.m12); m13 = static_cast<T>(m.m13); m14 = static_cast<T>(m.m14);
		m21 = static_cast<T>(m.m21); m22 = static_cast<T>(m.m22); m23 = static_cast<T>(m.m23); m24 = static_cast<T>(m.m24);
		m31 = static_cast<T>(m.m31); m32 = static_cast<T>(m.m32); m33 = static_cast<T>(m.m33); m34 = static_cast<T>(m.m34);
		m41 = static_cast<T>(m.m41); m42 = static_cast<T>(m.m42); m43 = static_cast<T>(m.m43); m44 = static_cast<T>(m.m44);
	}
	template<class T1, class T2, class T3>
	Matrix44(const Vector3<T1>& translate, const Quaternion<T2>& rotation, const Vector3<T3>& scaling)
	{

	}

	// 操作符重载
#if defined PE_DIRECTX
	Matrix44<T>& operator = (const XMMATRIX &matrix)
	{
		Matrix44<T> mat;
		m11 = XMVectorGetX(matrix.r[0]); m12 = XMVectorGetY(matrix.r[0]); m13 = XMVectorGetZ(matrix.r[0]); m14 = XMVectorGetW(matrix.r[0]);
		m21 = XMVectorGetX(matrix.r[1]); m22 = XMVectorGetY(matrix.r[1]); m23 = XMVectorGetZ(matrix.r[1]); m24 = XMVectorGetW(matrix.r[1]);
		m31 = XMVectorGetX(matrix.r[2]); m32 = XMVectorGetY(matrix.r[2]); m33 = XMVectorGetZ(matrix.r[2]); m34 = XMVectorGetW(matrix.r[2]);
		m41 = XMVectorGetX(matrix.r[3]); m42 = XMVectorGetY(matrix.r[3]); m43 = XMVectorGetZ(matrix.r[3]); m44 = XMVectorGetW(matrix.r[3]);
		return *this;
	}
#endif
	friend Matrix44<T> operator * (const Matrix44<T> &m, const float f)
	{
		Matrix44<T> mat;
		mat.m11 = m.m11 * f; mat.m12 = m.m12 * f; mat.m13 = m.m13 * f; mat.m14 = m.m14 * f;
		mat.m21 = m.m21 * f; mat.m22 = m.m22 * f; mat.m23 = m.m23 * f; mat.m24 = m.m24 * f;
		mat.m31 = m.m31 * f; mat.m32 = m.m32 * f; mat.m33 = m.m33 * f; mat.m34 = m.m34 * f;
		mat.m41 = m.m41 * f; mat.m42 = m.m42 * f; mat.m43 = m.m43 * f; mat.m44 = m.m44 * f;
		return mat;
	}

	friend Matrix44<T> operator * (const float f, const Matrix44<T> &m)
	{
		Matrix44<T> mat;
		mat.m11 = m.m11 * f; mat.m12 = m.m12 * f; mat.m13 = m.m13 * f; mat.m14 = m.m14 * f;
		mat.m21 = m.m21 * f; mat.m22 = m.m22 * f; mat.m23 = m.m23 * f; mat.m24 = m.m24 * f;
		mat.m31 = m.m31 * f; mat.m32 = m.m32 * f; mat.m33 = m.m33 * f; mat.m34 = m.m34 * f;
		mat.m41 = m.m41 * f; mat.m42 = m.m42 * f; mat.m43 = m.m43 * f; mat.m44 = m.m44 * f;
		return mat;
	}

	Matrix44<T>& operator += (const Matrix44<T> &m)
	{
		m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
		m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
		m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
		m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
		return *this;
	}

	friend Vector3<T> operator *(const Vector3<T> vec, const Matrix44<T> m)
	{
		return Vector3<T>(
			vec.x * m.m11 + vec.y * m.m21 + vec.z * m.m31 + m.m41,
			vec.x * m.m12 + vec.y * m.m22 + vec.z * m.m32 + m.m42,
			vec.x * m.m13 + vec.y * m.m23 + vec.z * m.m33 + m.m43
			);
	}

	Matrix44<T> operator * (const Matrix44<T> &m) const
	{
		Matrix44<T> mat(*this);
		mat *= m;
		return mat;
	}

	Matrix44<T>& operator *= (const Matrix44<T> &m)
	{
		T _11 = m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41;
		T _12 = m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42;
		T _13 = m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43;
		T _14 = m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44;

		T _21 = m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41;
		T _22 = m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42;
		T _23 = m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43;
		T _24 = m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44;

		T _31 = m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41;
		T _32 = m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42;
		T _33 = m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43;
		T _34 = m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44;

		T _41 = m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41;
		T _42 = m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42;
		T _43 = m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43;
		T _44 = m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44;

		*this = Matrix44<T>(
			_11, _12, _13, _14,
			_21, _22, _23, _24,
			_31, _32, _33, _34,
			_41, _42, _43, _44
			);
		return *this;
	}

	// 其他方法
	Matrix44<T> &Zero()
	{
		m11 = 0; m12 = 0; m13 = 0; m14 = 0;
		m21 = 0; m22 = 0; m23 = 0; m24 = 0;
		m31 = 0; m32 = 0; m33 = 0; m34 = 0;
		m41 = 0; m42 = 0; m43 = 0; m44 = 0;
		return *this;
	}

	Matrix44<T> &Indentity()
	{
		m11 = 1; m12 = 0; m13 = 0; m14 = 0;
		m21 = 0; m22 = 1; m23 = 0; m24 = 0;
		m31 = 0; m32 = 0; m33 = 1; m34 = 0;
		m41 = 0; m42 = 0; m43 = 0; m44 = 1;
		return *this;
	}

	Matrix44<T> &Transpose()
	{
		std::swap((T&)m12, (T&)m21);
		std::swap((T&)m13, (T&)m31);
		std::swap((T&)m14, (T&)m41);
		std::swap((T&)m23, (T&)m32);
		std::swap((T&)m24, (T&)m42);
		std::swap((T&)m34, (T&)m43);
		return *this;
	}
};

typedef Matrix44<float> Matrix44f;

#endif