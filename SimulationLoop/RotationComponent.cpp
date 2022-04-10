#include "RotationComponent.h"

//Easy set and get to access Rotation of each object
rotation_component::rotation_component() : m_rotation_(0.0f, 0.0f, 0.0f, 0.0f)
{
}

rotation_component::rotation_component(const XMFLOAT4 rotation) : m_rotation_(rotation) {
}



rotation_component::rotation_component(const float x, const float y, const float z, const float w) : m_rotation_(x, y, z, w) {
}

void rotation_component::set_rotation(const XMFLOAT4 newRotation) {
	m_rotation_ = newRotation;
	normalize();
}

void rotation_component::set_rotation(const float x, const float y, const float z, const float w) {
	m_rotation_ = XMFLOAT4(x, y, z, w);
	normalize();
}



void rotation_component::add_rotation(const XMFLOAT4 otherRotation) {

	//m_rotation += otherRotation;
}

void rotation_component::add_rotation(const float x, const float y, const float z, const float w) {
	//m_rotation += XMFLOAT4(x, y, z, w);
}

void rotation_component::normalize()
{
	auto d = m_rotation_.x * m_rotation_.x +
		m_rotation_.y * m_rotation_.y +
		m_rotation_.z * m_rotation_.z +
		m_rotation_.w * m_rotation_.w;

	if (d == 0.f)

	{
		m_rotation_.x = 1;
		return;
	}

	d = (1.0f) / sqrt(d);

	m_rotation_.x *= d;
	m_rotation_.y *= d;
	m_rotation_.z *= d;
	m_rotation_.w *= d;
	

}
