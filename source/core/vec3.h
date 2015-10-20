#pragma once
#include "types.h"
#include <array>

namespace core
{
	class vec3
	{
		std::array<Real,3> m_vec;
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

		Real x() const { return m_vec[0]; };
		Real y() const { return m_vec[1]; };
		Real z() const { return m_vec[2]; };
	};
}	