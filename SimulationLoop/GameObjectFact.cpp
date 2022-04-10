#include "GameFact.h"
#include <vector>

game_object_factory::game_object_factory(vector<game_object*>& gameObjects) : m_game_objects_(gameObjects)
{
}
void game_object_factory::add_game_object(const HWND hwnd, const XMFLOAT3& position, const XMFLOAT3& rotation,XMFLOAT3 &scale ,const XMFLOAT3& velocity, const XMFLOAT3& angularVelocity, const collider_component::collider_type& colliderType,const object::object_type& objType, const bool& useGravity, const float& mass, const float& drag, const float& angularDrag)
{
	auto quaternionRotation = XMFLOAT4();
		XMStoreFloat4(&quaternionRotation, XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
		m_game_objects_.push_back(new game_object(hwnd));
	m_game_objects_.back()->add_transform_component(position, quaternionRotation,scale);
	m_game_objects_.back()->add_collider_component(colliderType);
	m_game_objects_.back()->add_object_type(objType);
	m_game_objects_.back()->add_rigid_body_component(useGravity, mass, drag, angularDrag, position, quaternionRotation, velocity, angularVelocity);
}
