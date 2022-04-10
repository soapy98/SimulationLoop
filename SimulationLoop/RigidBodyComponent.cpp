#include "RigidBodyComponent.h"

rigid_body_component::rigid_body_component(const bool useGravity, const float mass, const float drag, const float angularDrag, const XMFLOAT3 position, const XMFLOAT4 rotation, const XMFLOAT3 velocity, const XMFLOAT3 angularVelocity, const XMFLOAT3X3 inertiaTensor)
	: m_is_awake_(true), m_use_gravity_(useGravity), m_motion_(0.0f), m_inverse_mass_(1.0f / mass), m_drag_(drag), m_angular_drag_(angularDrag), m_last_frame_acceleration_(XMVECTOR()), m_position_(XMLoadFloat3(&position)), m_new_position_(XMVECTOR()), m_rotation_(XMLoadFloat4(&rotation)), m_velocity_(XMLoadFloat3(&velocity)), m_new_velocity_(XMVECTOR()), m_angular_velocity_(XMLoadFloat3(&angularVelocity)), m_accumulated_force_(XMVECTOR()) {

	m_rotation_ = XMQuaternionNormalize(m_rotation_);

	set_inertia_tensor(inertiaTensor);

}
float rigid_body_component::get_mass() const {

	if (m_inverse_mass_ == 0.0f)
	{
		return numeric_limits<float>::max();
	}

	return (1.0f) / m_inverse_mass_;
}
XMFLOAT3X3& rigid_body_component::get_inertia_tensor() const
{
	auto inertiaTensor = XMFLOAT3X3();

	auto determinant = XMMatrixDeterminant(XMLoadFloat3x3(&m_inverse_inertia_tensor_));

	XMStoreFloat3x3(&inertiaTensor, XMMatrixInverse(&determinant, XMLoadFloat3x3(&m_inverse_inertia_tensor_)));

	return inertiaTensor;
}

void rigid_body_component::set_inertia_tensor(const XMFLOAT3X3& inertiaTensor)
{
	auto determinant = XMMatrixDeterminant(XMLoadFloat3x3(&inertiaTensor));

	XMStoreFloat3x3(&m_inverse_inertia_tensor_, XMMatrixInverse(&determinant, XMLoadFloat3x3(&inertiaTensor)));
}

void rigid_body_component::add_accumulated_force(const XMVECTOR& force)
{
	m_accumulated_force_ += force;
	m_is_awake_ = true;
}

void rigid_body_component::add_accumulated_torque(const XMVECTOR& torque)
{
	m_accumulated_torque_ += torque;
	m_is_awake_ = true;
}

void rigid_body_component::clear_accumulators()
{
	m_accumulated_force_ = XMVECTOR();
	m_accumulated_torque_ = XMVECTOR();
}

void rigid_body_component::calculate_derived_data()
{
	m_rotation_ = XMQuaternionNormalize(m_rotation_);

	m_transform_matrix_ = XMMatrixRotationQuaternion(m_rotation_);
	m_transform_matrix_ = XMMatrixMultiply(m_transform_matrix_, XMMatrixTranslationFromVector(m_new_position_));

	//CalculateTransformMatrix(m_transformMatrix, m_newPosition, m_rotation);
	//Calculate the transform matrix for the rigidbody

	auto tempTransformMatrix = XMFLOAT4X4();
	XMStoreFloat4x4(&tempTransformMatrix, m_transform_matrix_);

	auto inverseInertiaTensorInWorld = XMFLOAT3X3();

	inertia_tensor_transform_local_to_world(inverseInertiaTensorInWorld, m_inverse_inertia_tensor_, tempTransformMatrix);

	m_inverse_inertia_tensor_in_world_ = XMLoadFloat3x3(&inverseInertiaTensorInWorld);
}

//Automated optimised code used from Ian Millingtons book on Game Physics Engine Development: Edition 1
void rigid_body_component::calculate_transform_matrix(XMFLOAT4X4& transformMatrix, const XMFLOAT3& position, const XMFLOAT4& rotation)
{

	transformMatrix._11 = 1 - 2 * rotation.y * rotation.y - 2 * rotation.z * rotation.z;
	transformMatrix._12 = 2 * rotation.x * rotation.y - 2 * rotation.w * rotation.z;
	transformMatrix._13 = 2 * rotation.x * rotation.z + 2 * rotation.w * rotation.y;
	transformMatrix._14 = position.x;

	transformMatrix._21 = 2 * rotation.x * rotation.y + 2 * rotation.w * rotation.z;
	transformMatrix._22 = 1 - 2 * rotation.x * rotation.x - 2 * rotation.z * rotation.z;
	transformMatrix._23 = 2 * rotation.y * rotation.z - 2 * rotation.w * rotation.x;
	transformMatrix._24 = position.y;

	transformMatrix._31 = 2 * rotation.x * rotation.z - 2 * rotation.w * rotation.y;
	transformMatrix._32 = 2 * rotation.y * rotation.z + 2 * rotation.w * rotation.x;
	transformMatrix._33 = 1 - 2 * rotation.x * rotation.x - 2 * rotation.y * rotation.y;
	transformMatrix._34 = position.z;

	transformMatrix._41 = 0;
	transformMatrix._42 = 0;
	transformMatrix._43 = 0;
	transformMatrix._44 = 1;
}

//Automated optimised code used from Ian Millingtons book on Game Physics Engine Development: Edition 1
void rigid_body_component::inertia_tensor_transform_local_to_world(XMFLOAT3X3& inverseInertiaTensorInWorld, const XMFLOAT3X3& inertiaTensorInLocal, const XMFLOAT4X4& transformMatrix)
{
	//Only using the rotation data from our transform matrix
	auto t4 = transformMatrix._11 * inertiaTensorInLocal._11 +
		transformMatrix._12 * inertiaTensorInLocal._21 +
		transformMatrix._13 * inertiaTensorInLocal._31;
	auto t9 = transformMatrix._11 * inertiaTensorInLocal._12 +
		transformMatrix._12 * inertiaTensorInLocal._22 +
		transformMatrix._13 * inertiaTensorInLocal._32;
	auto t14 = transformMatrix._11 * inertiaTensorInLocal._13 +
		transformMatrix._12 * inertiaTensorInLocal._23 +
		transformMatrix._13 * inertiaTensorInLocal._33;

	auto t28 = transformMatrix._21 * inertiaTensorInLocal._11 +
		transformMatrix._22 * inertiaTensorInLocal._21 +
		transformMatrix._23 * inertiaTensorInLocal._31;
	auto t33 = transformMatrix._21 * inertiaTensorInLocal._12 +
		transformMatrix._22 * inertiaTensorInLocal._22 +
		transformMatrix._23 * inertiaTensorInLocal._32;
	auto t38 = transformMatrix._21 * inertiaTensorInLocal._13 +
		transformMatrix._22 * inertiaTensorInLocal._23 +
		transformMatrix._23 * inertiaTensorInLocal._33;

	auto t52 = transformMatrix._31 * inertiaTensorInLocal._11 +
		transformMatrix._32 * inertiaTensorInLocal._21 +
		transformMatrix._33 * inertiaTensorInLocal._31;
	auto t57 = transformMatrix._31 * inertiaTensorInLocal._12 +
		transformMatrix._32 * inertiaTensorInLocal._22 +
		transformMatrix._33 * inertiaTensorInLocal._32;
	auto t62 = transformMatrix._31 * inertiaTensorInLocal._13 +
		transformMatrix._32 * inertiaTensorInLocal._23 +
		transformMatrix._33 * inertiaTensorInLocal._33;

	inverseInertiaTensorInWorld._11 = t4 * transformMatrix._11 +
		t9 * transformMatrix._12 +
		t14 * transformMatrix._13;
	inverseInertiaTensorInWorld._12 = t4 * transformMatrix._21 +
		t9 * transformMatrix._22 +
		t14 * transformMatrix._23;
	inverseInertiaTensorInWorld._13 = t4 * transformMatrix._31 +
		t9 * transformMatrix._32 +
		t14 * transformMatrix._33;

	inverseInertiaTensorInWorld._21 = t28 * transformMatrix._11 +
		t33 * transformMatrix._12 +
		t38 * transformMatrix._13;
	inverseInertiaTensorInWorld._22 = t28 * transformMatrix._21 +
		t33 * transformMatrix._22 +
		t38 * transformMatrix._23;
	inverseInertiaTensorInWorld._23 = t28 * transformMatrix._31 +
		t33 * transformMatrix._32 +
		t38 * transformMatrix._33;

	inverseInertiaTensorInWorld._31 = t52 * transformMatrix._11 +
		t57 * transformMatrix._12 +
		t62 * transformMatrix._13;
	inverseInertiaTensorInWorld._32 = t52 * transformMatrix._21 +
		t57 * transformMatrix._22 +
		t62 * transformMatrix._23;
	inverseInertiaTensorInWorld._33 = t52 * transformMatrix._31 +
		t57 * transformMatrix._32 +
		t62 * transformMatrix._33;
}
