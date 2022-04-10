#pragma once
#include <limits>
#include <DirectXMath.h>
#include <glm/glm.hpp>

using namespace std;
using namespace DirectX;



class rigid_body_component
{
public:
	rigid_body_component(const bool useGravity, const float mass, const float drag, const float angularDrag, const XMFLOAT3 position, const XMFLOAT4 rotation, const XMFLOAT3 velocity, const XMFLOAT3 angularVelocity, const XMFLOAT3X3 inertiaTensor);
	rigid_body_component(const rigid_body_component& other)=default; // Copy Constructor
	rigid_body_component(rigid_body_component&& other) noexcept = default;  // Move Constructor
	~rigid_body_component() = default;  // Destructor

	rigid_body_component& operator = (const rigid_body_component& other) = delete; // Copy Assignment Operator
	rigid_body_component& operator = (rigid_body_component&& other) noexcept = delete;  // Move Assignment Operator

	//Gets
	bool get_is_awake() const noexcept { return m_is_awake_; }
	bool get_use_gravity() const noexcept { return m_use_gravity_; };
	float get_sleep_epsilon() const noexcept { return m_epsilon_; };
	float get_motion() const noexcept { return m_motion_; };
	float get_mass() const;
	float get_inverse_mass() const noexcept { return m_inverse_mass_; };
	float get_drag() const noexcept { return m_drag_; };
	float get_angular_drag() const noexcept { return m_angular_drag_; };

	const XMVECTOR& get_last_frame_acceleration() const noexcept { return m_last_frame_acceleration_; }

	const XMVECTOR& get_position() const noexcept { return m_position_; }
	const XMVECTOR& get_new_position() const noexcept { return m_new_position_; }
	const XMVECTOR& get_rotation() const noexcept { return m_rotation_; }
	const XMVECTOR& get_velocity() const noexcept { return m_velocity_; }
	const XMVECTOR& get_new_velocity() const noexcept { return m_new_velocity_; }

	const XMVECTOR& get_accumulated_force() const noexcept { return m_accumulated_force_; }
	const XMVECTOR& get_accumulated_torque() const noexcept { return m_accumulated_torque_; }
	const XMVECTOR& get_angular_velocity() const noexcept { return m_angular_velocity_; }

	XMFLOAT3X3& get_inertia_tensor() const;
	const XMMATRIX& get_inverse_inertia_tensor_world() const noexcept { return m_inverse_inertia_tensor_in_world_; }

	//Sets
	void set_is_awake(const bool isAwake)noexcept { m_is_awake_ = isAwake; }
	void set_use_gravity(const bool useGravity)noexcept { m_use_gravity_ = useGravity; }
	void set_motion(const float motion) noexcept { m_motion_ = motion; };
	void set_mass(const float mass)noexcept { m_inverse_mass_ = 1.0f / mass; };
	void set_inverse_mass(const float inverseMass)noexcept { m_inverse_mass_ = inverseMass; }
	void set_drag(const float drag)noexcept { m_drag_ = drag; };
	void set_angular_drag(const float angularDrag)noexcept { m_angular_drag_ = angularDrag; }

	void set_last_frame_acceleration(const XMVECTOR& newLastFrameAcceleration)noexcept { m_last_frame_acceleration_ = newLastFrameAcceleration; }

	void set_new_position(const XMVECTOR& newPosition)noexcept { m_new_position_ = newPosition; };
	void set_rotation(const XMVECTOR& newRotation){ m_rotation_ = XMQuaternionNormalize(newRotation); }
	void set_rotation(const float x, const float y, const float z, const float w){ m_rotation_ = XMLoadFloat4(&XMFLOAT4(x, y, z, w)); }
	
	void set_new_velocity(const XMVECTOR& newVelocity)noexcept { m_new_velocity_ = newVelocity; }

	void set_angular_velocity(const XMVECTOR& angularVelocity)noexcept { m_angular_velocity_ = angularVelocity; }
	void set_inertia_tensor(const XMFLOAT3X3& inertiaTensor);

	//Updates
	void update_position()noexcept { m_position_ = m_new_position_; }
	void update_velocity()noexcept{ m_velocity_ = m_new_velocity_; }

	bool has_finite_mass() const noexcept {return  m_inverse_mass_ >= 0.0f;}


	void add_accumulated_force(const XMVECTOR& force);
	void add_accumulated_torque(const XMVECTOR& torque);
	void clear_accumulators();
	
	//Updates our derived data (transform matrix) whenever a change is made to the transform or orientation
	void calculate_derived_data();

private:
	//Calculate the transform matrix from the position and rotation
	static void calculate_transform_matrix(XMFLOAT4X4& transformMatrix, const XMFLOAT3& position, const XMFLOAT4& rotation);

	//Convert the inverseInertiaTensor to world space
	void inertia_tensor_transform_local_to_world(XMFLOAT3X3& inverseInertiaTensorInWorld, const XMFLOAT3X3& inertiaTensorInLocal, const XMFLOAT4X4& transformMatrix);

	bool m_is_awake_;
	bool m_use_gravity_;
	float m_motion_;
	float m_inverse_mass_;
	float m_drag_;
	float m_angular_drag_;
	const float m_epsilon_ = 0.2f;
	
	XMVECTOR m_last_frame_acceleration_;

	XMVECTOR m_position_;
	XMVECTOR m_new_position_;

	//"Orientation"
	XMVECTOR m_rotation_;

	XMVECTOR m_velocity_;
	XMVECTOR m_new_velocity_;

	//"Rotation"
	XMVECTOR m_angular_velocity_;

	XMVECTOR m_accumulated_force_;
	XMVECTOR m_accumulated_torque_;

	//In local space
	XMFLOAT3X3 m_inverse_inertia_tensor_;

	//In world space
	XMMATRIX m_inverse_inertia_tensor_in_world_;

	//Holds transform matrix for converting body space into world space and vice versa
	//Note: Just derived data that already exists and can be calculated and used as is with our transform position and orientation
	//Last row is just 0,0,0,1
	XMMATRIX m_transform_matrix_;
};

