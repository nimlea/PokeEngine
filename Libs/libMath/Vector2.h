#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#pragma warning(disable:4244)

template <class T>
class Vector2
{
public:
	T x, y;

public:
	Vector2() : x(0), y(0) {}
	Vector2(T _x, T _y) : x(_x), y(_y) {}

	void Set(T _x, T _y)
	{
		x = _x;
		y = _y;
	}
};

// ²Ù×÷·ûÖØÔØ
template<class T1, class T2>
Vector2<T1> operator +(const Vector2<T1>& vec1, const Vector2<T2>& vec2){
	return Vector2<T1>(vec1.x + vec2.x, vec1.y + vec2.y);
}
template<class T1, class T2>
Vector2<T1> operator -(const Vector2<T1>& vec1, const Vector2<T2>& vec2){
	return Vector2<T1>(vec1.x - vec2.x, vec1.y - vec2.y);
}
template<class T1, class T2>
Vector2<T1> operator *(const Vector2<T1>& vec, T2 num){
	return Vector2<T1>(vec.x * num, vec.y * num);
}
template<class T1, class T2>
Vector2<T2> operator *(T1 num, const Vector2<T2>& vec){
	return Vector2<T2>(vec.x * num, vec.y * num);
}

typedef Vector2<float> Vector2f;

#endif