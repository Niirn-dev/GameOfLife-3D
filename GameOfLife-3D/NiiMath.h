#pragma once
#include <cmath>

static constexpr double PI_D = 3.1415926535897932384626433;
static constexpr float PI = (float)PI_D;

template<typename T>
constexpr T sign( T val )
{
	return static_cast<T>( ( val > (T)0.0 ) - ( val < (T)0.0 ) );
}

/// <summary>
/// Wraps angle value to be in [-pi;pi]
/// </summary>
/// <param name="angle_rad">Angle value in radians</param>
/// <returns>Wrapped angle value in radians</returns>
template<typename T>
inline T wrap_angle( T angle_rad ) noexcept
{
	const T modded = std::fmod( angle_rad,(T)2.0 * (T)PI_D );
	return ( std::abs( modded ) > (T)PI_D ) ?
		( modded - (T)2.0 * (T)PI_D * sign( modded ) ) :
		modded;
}