#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#pragma warning(disable:4244)

template <class T>
class Vector4
{
public:
	T x, y, z, w;

public:
	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

	void Set(T _x, T _y, T _z, T _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
};

// ²Ù×÷·ûÖØÔØ
template<class T1, class T2>
Vector4<T1> operator +(const Vector4<T1>& vec1, const Vector4<T2>& vec2){
	return Vector4<T1>(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w);
}
template<class T1, class T2>
Vector4<T1> operator -(const Vector4<T1>& vec1, const Vector4<T2>& vec2){
	return Vector4<T1>(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w);
}
template<class T1, class T2>
Vector4<T1> operator *(const Vector4<T1>& vec, T2 num){
	return Vector4<T1>(vec.x * num, vec.y * num, vec.z * num, vec.w *num);
}
template<class T1, class T2>
Vector4<T2> operator *(T1 num, const Vector4<T2>& vec){
	return Vector4<T2>(vec.x * num, vec.y * num, vec.z * num, vec.w *num);
}

typedef Vector4<float> Vector4f;

#endif