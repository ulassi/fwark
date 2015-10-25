#include <core/vec3.h>
#include <stdexcept>

namespace core
{
vec3::vec3(Real r)
{
	m_vec[0] = m_vec[1] = m_vec[2] = r;
}

vec3::vec3(Real x, Real y, Real z) : m_vec{ x,y,z }
{
}

vec3::vec3(const vec3 & rhs) : m_vec(rhs.m_vec)
{
}

vec3::vec3(vec3 && rhs) : m_vec(std::move(rhs.m_vec) )
{
}

vec3 & vec3::operator=(vec3 && rhs)
{
	m_vec = std::move(rhs.m_vec);
	return *this;
}

vec3 & vec3::operator=(const vec3 & rhs)
{
	m_vec = rhs.m_vec;
	return *this;
}

vec3& vec3::operator+=(const vec3& rhs)
{ 
	m_vec[0] += rhs.m_vec[0];
	m_vec[1] += rhs.m_vec[1];
	m_vec[2] += rhs.m_vec[2];
	return *this;
}

vec3 vec3::operator+(const vec3& rhs) const
{
	return vec3(m_vec[0] + rhs.m_vec[0], m_vec[1] + rhs.m_vec[1], m_vec[2] + rhs.m_vec[2]);
}

vec3& vec3::operator-=(const vec3& rhs)
{
	m_vec[0] -= rhs.m_vec[0];
	m_vec[1] -= rhs.m_vec[1];
	m_vec[2] -= rhs.m_vec[2];
	return *this;
}

vec3 vec3::operator-(const vec3& rhs) const
{
	return vec3(x() - rhs.x(), y() - rhs.y(), z() - rhs.z());
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