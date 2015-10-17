#pragma once
#include "types.h"

namespace core
{
	class vec3
	{
		Real m_x, m_y, m_z;
	public:

		// construction
		explicit vec3(Real);
		explicit vec3(Real x, Real y, Real z);

		// copy and move
		vec3(const vec3&);
		vec3(vec3&&);
		vec3& operator = (vec3&&);
		vec3& operator = (const vec3&);
	
		vec3& operator += (const vec3& rhs);
		vec3 operator + (const vec3& rhs) const;
		vec3& operator -= (const vec3& rhs);
		vec3 operator - (const vec3& rhs) const;

		Real dot(const vec3& rhs) const;
		vec3 cross(const vec3& rhs) const;

		Real x() { return m_x; };
		Real y() { return m_y; };
		Real z() { return m_z; };
	};
}	