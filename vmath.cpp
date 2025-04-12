#include "vmath.h"

#ifdef VMATH_NAMESPACE
namespace VMATH_NAMESPACE
{
#endif

template class Vector2<float>;
template class Vector2<double>;
template class Vector2<int>;
template class Vector3<float>;
template class Vector3<double>;
template class Vector3<int>;
template class Vector4<float>;
template class Vector4<double>;
template class Vector4<int>;
template class Matrix3<float>;
template class Matrix3<double>;
template class Matrix3<int>;
template class Matrix4<float>;
template class Matrix4<double>;
template class Matrix4<int>;
template class Quaternion<float>;
template class Quaternion<double>;
	
#ifdef VMATH_NAMESPACE
}
#endif
