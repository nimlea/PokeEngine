#ifndef _MATHUTILITY_H_
#define _MATHUTILITY_H_

#include "Matrix44.h"
#include "Vector4.h"

#define MATH_PI 3.14f

Matrix44f MatrixLookAtRH(Vector3f vecEye, Vector3f vecLookAt, Vector3f vecUp);
Matrix44f MatrixLookAtRH(Vector4f vecEye, Vector4f vecLookAt, Vector4f vecUp);
Matrix44f MatrixPerspectiveProjectionRH(float fovAngleY, float aspectRatio, float nearZ, float farZ);


Matrix44f MatrixTranspose(Matrix44f matrix);
Matrix44f MatrixRotationX(float angle);
Matrix44f MatrixRotationY(float angle);
Matrix44f MatrixRotationZ(float angle);

#endif