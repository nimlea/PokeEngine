#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#pragma warning(disable:4244)

template <class T>
class Vector3
{
public:
	T x, y, z;

public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	void Set(T _x, T _y, T _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3<T>& Zero()
	{
		x = 0; y = 0; z = 0;
		return *this;
	}
};

// ²Ù×÷·ûÖØÔØ
template<class T1, class T2>
Vector3<T1> operator +(const Vector3<T1>& vec1, const Vector3<T2>& vec2){
	return Vector3<T1>(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}
template<class T1, class T2>
Vector3<T1> operator -(const Vector3<T1>& vec1, const Vector3<T2>& vec2){
	return Vector3<T1>(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}
template<class T1, class T2>
Vector3<T1> operator *(const Vector3<T1>& vec, T2 num){
	return Vector3<T1>(vec.x * num, vec.y * num, vec.z * num);
}
template<class T1, class T2>
Vector3<T2> operator *(T1 num, const Vector3<T2>& vec){
	return Vector3<T2>(vec.x * num, vec.y * num, vec.z * num);
}

typedef Vector3<float> Vector3f;

#endif