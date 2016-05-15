#include "MathUtility.h"
#include "..\..\Engine\Common\PEMacro.h"

#if defined PE_DIRECTX
#include <DirectXMath.h>
using namespace DirectX;
#endif

Matrix44f MatrixLookAtRH(Vector3f vecEye, Vector3f vecLookAt, Vector3f vecUp)
{
	Matrix44f mat;
#if defined PE_DIRECTX
	XMVECTOR eye = XMVectorSet(vecEye.x, vecEye.y, vecEye.z, 1.0f);
	XMVECTOR at = XMVectorSet(vecLookAt.x, vecLookAt.y, vecLookAt.z, 1.0f);
	XMVECTOR up = XMVectorSet(vecUp.x, vecUp.y, vecUp.z, 1.0f);
	mat = XMMatrixLookAtRH(eye, at, up);
#endif
	return mat;
}

Matrix44f MatrixLookAtRH(Vector4f vecEye, Vector4f vecLookAt, Vector4f vecUp)
{
	Matrix44f mat;
#if defined PE_DIRECTX
	XMVECTOR eye = XMVectorSet(vecEye.x, vecEye.y, vecEye.z, vecEye.w);
	XMVECTOR at = XMVectorSet(vecLookAt.x, vecLookAt.y, vecLookAt.z, vecLookAt.w);
	XMVECTOR up = XMVectorSet(vecUp.x, vecUp.y, vecUp.z, vecUp.w);
	mat = XMMatrixLookAtRH(eye, at, up);
#endif
	return mat;
}

Matrix44f MatrixPerspectiveProjectionRH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	Matrix44f mat;
#if defined PE_DIRECTX
	mat = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, nearZ, farZ);
#endif
	return mat;
}

Matrix44f MatrixTranspose(Matrix44f matrix)
{
	Matrix44f transpose;
	transpose.m11 = matrix.m11; transpose.m12 = matrix.m21; transpose.m13 = matrix.m31; transpose.m14 = matrix.m41;
	transpose.m21 = matrix.m12; transpose.m22 = matrix.m22; transpose.m23 = matrix.m32; transpose.m24 = matrix.m42;
	transpose.m31 = matrix.m13; transpose.m32 = matrix.m23; transpose.m33 = matrix.m33; transpose.m34 = matrix.m43;
	transpose.m41 = matrix.m14; transpose.m42 = matrix.m24; transpose.m43 = matrix.m34; transpose.m44 = matrix.m44;
	return transpose;
}

Matrix44f MatrixRotationX(float angle)
{
	Matrix44f mat;
#if defined PE_DIRECTX
	mat = XMMatrixRotationX(angle);
#endif
	return mat;
}

Matrix44f MatrixRotationY(float angle)
{
	Matrix44f mat;
#if defined PE_DIRECTX
	mat = XMMatrixRotationY(angle);
#endif
	return mat;
}

Matrix44f MatrixRotationZ(float angle)
{
	Matrix44f mat;
#if defined PE_DIRECTX
	mat = XMMatrixRotationZ(angle);
#endif
	return mat;
}