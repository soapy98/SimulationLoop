#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class velocity_component
{
public:
	velocity_component();
	explicit velocity_component(const XMFLOAT3& velocity);
	velocity_component(const float x, const float y, const float z);
	velocity_component(const velocity_component& other)= default; // Copy Constructor
	velocity_component(velocity_component&& other) noexcept = default; // Move Constructor
	~velocity_component() = default; // Destructor

	velocity_component& operator= (const velocity_component& other) = default; // Copy Assignment Operator
	velocity_component& operator= (velocity_component&& other) = default;
	const XMFLOAT3& get_velocity() const noexcept { return m_velocity_; };
	void update_velocity()noexcept { m_velocity_ = m_new_velocity_; };

	void set_new_velocity(const XMFLOAT3& newVelocity)noexcept { m_new_velocity_ = newVelocity; };
	void set_new_velocity(const float x, const float y, const float z){ m_new_velocity_ = XMFLOAT3(x, y, z); };
	void add_velocity(const XMFLOAT3& otherVelocity);
	void add_velocity(const float x, const float y, const float z);

private:
	XMFLOAT3 m_velocity_;
	XMFLOAT3 m_new_velocity_;
};