#include"../Vector2D.h"

//À‘Ì‚¾‚¯‚Ç .h ‚È‚Ì•sv‹c‚ÈŠ´‚¶ 

template <class T>
Vector2DTemp<T>::Vector2DTemp()
{
	x = 0;
	y = 0;
}
template <class T>
Vector2DTemp<T>::Vector2DTemp(T x, T y)
{
	Vector2DTemp::x = x;
	Vector2DTemp::y = y;
}

template <class T>
Vector2DTemp<T>::~Vector2DTemp()
{
}

//‘ã“ü‰‰Zq

template <class T>Vector2DTemp<T>& Vector2DTemp<T>::operator=(const Vector2DTemp<T>& vec)
{
	x = vec.x;
	y = vec.y;

	return *this;

}

//“Y‚¦š‰‰Zq
template <class T>
T& Vector2DTemp<T>::operator[](int j)
{
	switch (j)
	{
	case 0:
		return x;
		//break;
	case 1:
		return y;
		//break;
	default:
		return x;
		//break;
	}
}

//’P€‰‰Zq

template <class T>
Vector2DTemp<T>& Vector2DTemp<T>::operator+=(const Vector2DTemp<T>& vec)
{
	x += vec.x;
	y += vec.y;

	return *this;
}

template <class T>
Vector2DTemp<T>& Vector2DTemp<T>::operator-=(const Vector2DTemp<T>& vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}
template <class T>
Vector2DTemp<T>& Vector2DTemp<T>::operator*=(const Vector2DTemp<T>& vec)
{
	x *= vec.x;
	y *= vec.y;

	return *this;
}
template <class T>
Vector2DTemp<T>& Vector2DTemp<T>::operator/=(const Vector2DTemp<T>& vec)
{
	//ƒK[ƒhˆ—
	if (vec.x)
	{
		x /= vec.x;
	}
	else
	{
		x = 0;
	}
	if (vec.y)
	{
		y /= vec.y;
	}
	else
	{
		y = 0;
	}


	return *this;
}
template <class T>
Vector2DTemp<T>& Vector2DTemp<T>::operator%=(const Vector2DTemp<T>& vec)
{
	//ƒK[ƒhˆ—
	if (vec.x)
	{
		x = static_cast<int>(x) % static_cast<int>(vec.x);
	}
	else
	{
		x = 0;
	}
	if (vec.y)
	{
		y = static_cast<int>(y) % static_cast<int>(vec.y);
	}
	else
	{
		y = 0;
	}
	

	return *this;
}
template <class T>
Vector2DTemp<T> Vector2DTemp<T>::operator+() const
{
	return *this;
}
template <class T>
Vector2DTemp<T> Vector2DTemp<T>::operator-() const
{

	return{ -x,-y };
}

//“ñ€‰‰Zq
template <class T>
Vector2DTemp<T> Vector2DTemp<T>::operator+(const Vector2DTemp<T>& vec)
{
	//Vector2D val;
	//val.x = x + vec.x;
	//val.y = y + vec.y;
	//return val;
	return { x + vec.x,y + vec.y };
}
template <class T>
Vector2DTemp<T> Vector2DTemp<T>::operator-(const Vector2DTemp<T>& vec)
{
	return { x - vec.x,y - vec.y };
}
template <class T>
Vector2DTemp<T> Vector2DTemp<T>::operator*(const Vector2DTemp<T>& vec)
{
	return { x * vec.x,y * vec.y };
}

//Vector2D Vector2D::operator/(const Vector2D& vec)
//{
//	Vector2D val;
//	//ƒK[ƒhˆ—
//	if (vec.x)
//	{
//		val.x = x / vec.x;
//	}
//	else
//	{
//		val.x = 0;
//	}
//	if (vec.y)
//	{
//		val.y = y / vec.y;
//	}
//	else
//	{
//		val.y = 0;
//	}
//
//	
//	return val;
//}

//Vector2D Vector2D::operator%(const Vector2D& vec)
//{
//	Vector2D val;
//	//ƒK[ƒhˆ—
//	if (vec.x)
//	{
//		val.x = x % vec.x;
//	}
//	else
//	{
//		val.x = 0;
//	}
//	if (vec.y)
//	{
//		val.y = y % vec.y;
//	}
//	else
//	{
//		val.y = 0;
//	}
//	return val;
//}

//”äŠr‰‰Zq
template <class T>
bool Vector2DTemp<T>::operator==(const Vector2DTemp<T>& vec) const
{
	//if (x == vec.x && y == vec.y)
	//{
	//	return true;
	//}
	//return false;
	return(x == vec.x && y == vec.y);
}
template <class T>
bool Vector2DTemp<T>::operator!=(const Vector2DTemp<T>& vec) const
{
	return(x != vec.x && y != vec.y);
}
template <class T>
bool Vector2DTemp<T>::operator<(const Vector2DTemp<T>& vec) const
{
	return(x < vec.x && y < vec.y);
}
template <class T>
bool Vector2DTemp<T>::operator<=(const Vector2DTemp<T>& vec) const
{
	return(x <= vec.x && y <= vec.y);
}
template <class T>
bool Vector2DTemp<T>::operator>(const Vector2DTemp<T>& vec) const
{
	return(x > vec.x && y > vec.y);
}
template <class T>
bool Vector2DTemp<T>::operator>=(const Vector2DTemp<T>& vec) const
{
	return(x >= vec.x && y >= vec.y);
}
template <class T>
Vector2DTemp<T> operator*(const Vector2DTemp<T>& vec1, const Vector2DTemp<T>& vec2)
{
	return {vec1.x*vec2.x,vec1.y*vec2.y};
}

//®”Œ^
template <class T>
Vector2DTemp<T> operator*(const Vector2DTemp<T> vec, const int num)
{
	return{ vec.x * num,vec.y * num };
}
template <class T>
Vector2DTemp<T> operator*(const int num, const Vector2DTemp<T> vec)
{
	return{ vec.x * num,vec.y * num };
}


//¬”“_‚à‰Â
template <class T>
Vector2DTemp<T> operator*(const Vector2DTemp<T> vec, const float num)
{
	return{ static_cast<int>(vec.x * num),static_cast<int>(vec.y * num) };
}
template <class T>
Vector2DTemp<T> operator*(const float num, const Vector2DTemp<T> vec)
{
	return{ static_cast<int>(vec.x * num),static_cast<int>(vec.y * num) };
}
template <class T>
Vector2DTemp<T> operator/(const Vector2DTemp<T> vec, const int num)
{
	return{ vec.x / num,vec.y / num };
}
template <class T>
Vector2DTemp<T> operator/(const Vector2DTemp<T> vec1, Vector2DTemp<T> vec2)
{
	Vector2DTemp<T> val;
	//ƒK[ƒhˆ—
	if (vec2.x)
	{
		val.x = vec1.x / vec2.x;
	}
	else
	{
		val.x = 0;
	}
	if (vec2.y)
	{
		val.y =vec1. y / vec2.y;
	}
	else
	{
		val.y = 0;
	}
	return val;
}
template <class T>
Vector2DTemp<T> operator%(const Vector2DTemp<T> vec1, Vector2DTemp<T> vec2)
{
	Vector2DTemp<T> val;
	//ƒK[ƒhˆ—
	if (vec2.x)
	{
		val.x = vec1.x % vec2.x;
	}
	else
	{
		val.x = 0;
	}
	if (vec2.y)
	{
		val.y = vec1.y % vec2.y;
	}
	else
	{
		val.y = 0;
	}
	return val;
}
