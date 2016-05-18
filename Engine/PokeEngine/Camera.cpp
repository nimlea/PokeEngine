#include "Camera.h"

PE_BEGIN

Camera::Camera()
{
	mUp.x = 0.0f;
	mUp.y = 1.0f;
	mUp.z = 0.0f;
}

void Camera::SetLookat(float x, float y, float z)
{
	mLookPoint.x = x;
	mLookPoint.y = y;
	mLookPoint.z = z;
}

void Camera::SetLookat(const Vector3f &p)
{
	SetLookat(p.x, p.y, p.z);
}

PE_END