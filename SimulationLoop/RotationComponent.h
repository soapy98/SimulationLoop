#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class rotation_component
{
public:
	rotation_component(); // Default Constructor
	rotation_component(const XMFLOAT4 rotation);
	rotation_component(const float x, const float y, const float z, const float w);
	rotation_component(const rotation_component& other) = default;; // Copy Constructor
	rotation_component(rotation_component&& other) noexcept = default;; // Move Constructor
	~rotation_component() = default;;

	rotation_component& operator = (const rotation_component& other) = default;; // Copy Assignment Operator
	rotation_component& operator = (rotation_component&& other) noexcept = default;; // Move Assignment Operator

	const XMFLOAT4& get_rotation() const noexcept { return m_rotation_; }

	void set_rotation(const XMFLOAT4 newRotation);
	void set_rotation(const float x, const float y, const float z, const float w);


	void add_rotation(const XMFLOAT4 otherRotation);
	void add_rotation(const float x, const float y, const float z, const float w);

private:
	void normalize();

	//Our angular orientation as a quaternion
	XMFLOAT4 m_rotation_;
};

