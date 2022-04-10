#include "GameObject.h"
#include <Windows.h>
game_object::game_object(HWND hwnd) : m_initializationFailed(false),
                                      m_hwnd(hwnd) , m_velocity(nullptr),
                                      m_rigidBody(nullptr), m_collider(nullptr), m_radius(0)
{
}

game_object::~game_object()
{
	if (m_hwnd)
	{
		//Don't delete as it only stores a reference
		m_hwnd = nullptr;
	}

	if (m_collider)
	{
		delete m_collider;
		m_collider = nullptr;
	}

	if (m_rigidBody)
	{
		delete m_rigidBody;
		m_rigidBody = nullptr;
	}

	if (m_object)
	{
		delete m_object;
		m_object = nullptr;
	}

	if (m_transform)
	{
		delete m_transform;
		m_transform = nullptr;
	}
}


void game_object::add_velocity_component()
{
	m_velocity = new velocity_component();
}

void game_object::add_velocity_component(const XMFLOAT3& velocity)
{
	m_velocity = new velocity_component(velocity);
}

void game_object::add_velocity_component(const float x, const float y, const float z)
{
	m_velocity = new velocity_component(x, y, z);
}

//Inertia tensor is based off the model type, if the model isn't initialised before the rigidbody then it will try the colliders type, else it throws an error stating this
void game_object::add_rigid_body_component(const bool useGravity, const float mass, const float drag, const float angularDrag, const XMFLOAT3 position, const XMFLOAT4 rotation, const XMFLOAT3 velocity, const XMFLOAT3 angularVelocity) {

	auto inertiaTensor = XMFLOAT3X3();
	auto scale = XMVECTOR();

	if (m_object)
	{
		{
			scale = m_transform->get_scale();
		}
		const auto scaleX = XMVectorGetX(scale);
		const auto scaleY = XMVectorGetY(scale);
		const auto scaleZ = XMVectorGetZ(scale);

		switch (m_object->get_object_type())
		{
		case object::object_type::sphere:
			inertiaTensor = XMFLOAT3X3(0.4f * (mass * (scaleX * scaleX)), 0.0f, 0.0f,
				0.0f, 0.4f * (mass * (scaleX * scaleX)), 0.0f,
				0.0f, 0.0f, 0.4f * (mass * (scaleX * scaleX)));
			break;
		case object::object_type::cube:
			inertiaTensor = XMFLOAT3X3(1.0f / 12.0f * (mass * (scaleY * scaleY + scaleZ * scaleZ)), 0.0f, 0.0f,
				0.0f, 1.0f / 12.0f * (mass * (scaleX * scaleX + scaleZ * scaleZ)), 0.0f,
				0.0f, 0.0f, 1.0f / 12.0f * (mass * (scaleX * scaleX + scaleY * scaleY)));
			break;
		case object::object_type::plane:
			//No inertia tensor needed
			break;
		case object::object_type::peg:
			inertiaTensor = XMFLOAT3X3(((1.0f / 12.0f) * (mass * (scaleY * scaleY))) + 0.25f * (mass * (scaleX * scaleX)), 0.0f, 0.0f,
				0.0f, 0.5f * (mass * (scaleX * scaleX)), 0.0f,
				0.0f, 0.0f, ((1.0f / 12.0f) * (mass * (scaleY * scaleY))) + 0.25f * (mass * (scaleX * scaleX)));
			break;
		default:
			m_initializationFailed = true;
			return;
		}
	}
	else
	{
		MessageBox(m_hwnd, "You need to define a model component before the rigidbody!", "Error: Missing Component", MB_OK);
		m_initializationFailed = true;
		return;
	}

	m_rigidBody = new rigid_body_component(useGravity, mass, drag, angularDrag, position, rotation, velocity, angularVelocity, inertiaTensor);
}

void game_object::add_collider_component(const collider_component::collider_type colliderType) {

	switch (colliderType)
	{
	case collider_component::collider_type::sphere:
		m_collider = new sphere_collider();
		break;
	case collider_component::collider_type::aabb_cube:
		m_collider = new aabb_cube_collider();
		break;
	case collider_component::collider_type::obb_cube:
		m_collider = new obb_cube_collider();
		break;
	case collider_component::collider_type::plane:
		m_collider = new plane_collider();
		break;
	case collider_component::collider_type::peg:
		m_collider = new cylinder_collider();
		break;
	default:
		m_initializationFailed = true;
		break;
	}
}

void game_object::set_plane_collider_data(const XMFLOAT3& centre, const XMFLOAT3& pointOne, const XMFLOAT3& pointTwo, const float offset) const
{
	auto* planeCollider = dynamic_cast<plane_collider*>(m_collider);

	if (!planeCollider)
	{
		MessageBox(m_hwnd, "You tried setting data to a collider type that's isn't a plane collider!", "Error: Collider Conversion", MB_OK);
		return;
	}

	planeCollider->set_normal(centre, pointOne, pointTwo);
	planeCollider->set_offset(offset);

	planeCollider = nullptr;
}

void game_object::add_transform_component(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale)
{
	m_transform = new transform_component(position, rotation, scale);
}
void game_object::add_transform_component(const XMFLOAT3& scale)
{
	m_transform = new transform_component(scale);
}

bool game_object::render() {
	auto position = m_rigidBody->get_position();
	auto rotation = m_rigidBody->get_rotation();
	auto scale = get_scale();
	if (XMVectorGetZ(rotation) > 0.f)
	{
		rotation = XMVectorSet(0, 0, 1, 0);
	}
	else if (XMVectorGetZ(rotation) < 0.f)
	{
		rotation = XMVectorSet(0, 0, -1, 0);
	}
	m_object->render(position, rotation, scale/*,color*/);
	auto result = true;
	return result;
}

void game_object::add_object_type(const object::object_type obj)
{
	m_object = new object(obj);
}

