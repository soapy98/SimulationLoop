#include "ColliderComponent.h"
collider_component::~collider_component() = default;
void plane_collider::set_normal(const XMFLOAT3& centre, const XMFLOAT3& pointOne, const XMFLOAT3& pointTwo)
{
	const auto crossOne = pointOne - centre;
	const auto crossTwo = pointTwo - centre;

	m_normal = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&crossOne), XMLoadFloat3(&crossTwo)));

	XMStoreFloat(&m_offsetTest, XMVector3Dot(m_normal, XMLoadFloat3(&centre)));
}


void plane_collider::set_offset(const float offset)
{
	m_offset = offset;
}

