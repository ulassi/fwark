#include "vec3.h"
#include <stdexcept>

namespace core
{
vec3::vec3(Real r) : m_x(r), m_y(r), m_z(r)
{
}

vec3::vec3(Real x, Real y, Real z) : m_x(x), m_y(y), m_z(z)
{
}

vec3& vec3::operator+=(const vec3& rhs)
{ 
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	m_z += rhs.m_z;
	return *this;
}

vec3 vec3::operator+(const vec3& rhs) const
{
	return vec3(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
}

vec3& vec3::operator-=(const vec3& rhs)
{
	m_x -= rhs.m_x;
	m_y -= rhs.m_y;
	m_z -= rhs.m_z;
	return *this;
}

vec3 vec3::operator-(const vec3& rhs) const
{
	return vec3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
}

Real vec3::dot(const vec3& rhs) const
{
	throw std::logic_error("not implement");
}

vec3 vec3::cross(const vec3& rhs) const
{
	throw std::logic_error("not implement");
}

}