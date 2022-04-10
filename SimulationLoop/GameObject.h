#pragma once
#include "PositionComponent.h"
#include "RotationComponent.h"
#include "VelocityComponent.h"
#include "RigidBodyComponent.h"
#include <Windows.h>
#include "ColliderComponent.h"
#include "ObjectType.h"
#include "ScaleComponent.h"
#include "TransformComponent.h"

class game_object
{
private:
	bool m_initializationFailed;
	HWND m_hwnd;
	velocity_component* m_velocity;
	rigid_body_component* m_rigidBody;
	collider_component* m_collider;
	object* m_object;
	transform_component* m_transform;
	float m_radius;
public:
	game_object()=default;
	game_object(HWND hwnd); // Default Constructor (Empty GameObject)
	game_object(const game_object& other)= default; // Copy Constructor
	game_object(game_object&& other) noexcept = default; // Move Constructor
	~game_object(); // Destructor

	game_object& operator = (const game_object& other) = default;; // Copy Assignment Operator
	game_object& operator = (game_object&& other) noexcept = default;; // Move Assignment Operator

	void add_velocity_component();
	void add_velocity_component(const XMFLOAT3& velocity);
	void add_velocity_component(const float x, const float y, const float z);

	void add_rigid_body_component(const bool useGravity, const float mass, const float drag, const float angularDrag, const XMFLOAT3 position, const XMFLOAT4 rotation, const XMFLOAT3 velocity, const XMFLOAT3 angularVelocity);
	void add_collider_component(const collider_component::collider_type colliderType);
	void set_plane_collider_data(const XMFLOAT3& centre, const XMFLOAT3& pointOne, const XMFLOAT3& pointTwo, const float offset) const;

	void add_transform_component(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& velocity);
	void add_transform_component(const XMFLOAT3& scale);

	position_component* get_position_component() const noexcept { return m_transform->get_position_component(); }
	const XMFLOAT3& get_position() const noexcept { return m_transform->get_position(); }

	rotation_component* get_rotation_component() const noexcept { return m_transform->get_rotation_component(); };
	const XMFLOAT4& get_rotation() const noexcept { return m_transform->get_rotation(); }

	velocity_component* get_velocity_component() const noexcept { return m_velocity; }
	const XMFLOAT3& get_velocity() const noexcept { return m_velocity->get_velocity(); }

	rigid_body_component* get_rigid_body_component() const noexcept { return m_rigidBody; }
	collider_component* get_collider_component() const noexcept { return m_collider; }

	scale_component* get_scale_component() const noexcept { return m_transform->get_scale_component(); }
	const XMVECTOR& get_scale() const noexcept { return m_transform->get_scale(); }
	
	bool render();
	void add_object_type(const object::object_type obj);
	object* get_object_type() const noexcept { return m_object; }


};
