#include "VelocityComponent.h"

//Easy access to set and get velocity of an object
velocity_component::velocity_component() : m_velocity_(0.0f, 0.0f, 0.0f), m_new_velocity_(0.0f, 0.0f, 0.0f)
{

}

velocity_component::velocity_component(const XMFLOAT3& velocity) : m_velocity_(velocity), m_new_velocity_(0.0f, 0.0f, 0.0f)
{

}

velocity_component::velocity_component(const float x, const float y, const float z) : m_velocity_(x, y, z), m_new_velocity_(0.0f, 0.0f, 0.0f)
{

}

void velocity_component::add_velocity(const XMFLOAT3& otherVelocity)
{
	m_velocity_ = XMFLOAT3(m_velocity_.x + otherVelocity.x, m_velocity_.y + otherVelocity.y, m_velocity_.z + otherVelocity.z);
}

void velocity_component::add_velocity(const float x, const float y, const float z)
{
	m_velocity_ = XMFLOAT3(m_velocity_.x + x, m_velocity_.y + y, m_velocity_.z + z);
}

