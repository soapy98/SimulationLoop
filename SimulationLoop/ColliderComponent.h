#pragma once
#include <DirectXMath.h>
#include "XMFloat3.h"

using namespace DirectX;
//Collider component with different collider types for each object type
class collider_component
{
public:
	enum class collider_type
	{
		sphere,
		aabb_cube,
		obb_cube,
		plane,
		peg
	};

	collider_component() = default;
	virtual ~collider_component() = 0;
	collider_component(const collider_component& other) = default; // Copy Constructor
	collider_component(collider_component&& other) = default; // Move Constructor

	collider_component& operator = (const collider_component& other) = default;// Copy Assignment Operator
	collider_component& operator = (collider_component&& other) noexcept = default; // Move Assignment Operator

	virtual collider_type get_collider() const = 0;
};

class sphere_collider final : public collider_component {

public:
	sphere_collider() = default;;
	~sphere_collider()override = default;;
	sphere_collider(const sphere_collider& other) = default; // Copy Constructor
	sphere_collider(sphere_collider&& other) = default; // Move Constructor

	sphere_collider& operator = (const sphere_collider& other) = default;// Copy Assignment Operator
	sphere_collider& operator = (sphere_collider&& other) noexcept = default; // Move Assignment Operator

	collider_type get_collider() const noexcept override { return collider_type::sphere; }
};

class aabb_cube_collider final : public collider_component {

public:
	aabb_cube_collider() = default;;
	~aabb_cube_collider()override = default;;
	aabb_cube_collider(const aabb_cube_collider& other) = default; // Copy Constructor
	aabb_cube_collider(aabb_cube_collider&& other) = default; // Move Constructor

	aabb_cube_collider& operator = (const aabb_cube_collider& other) = default;// Copy Assignment Operator
	aabb_cube_collider& operator = (aabb_cube_collider&& other) noexcept = default; // Move Assignment Operator

	collider_type get_collider() const noexcept override { return collider_type::aabb_cube; }
};

class obb_cube_collider final : public collider_component {

public:
	obb_cube_collider() = default;;
	~obb_cube_collider()override = default;;
	obb_cube_collider(const obb_cube_collider& other) = default; // Copy Constructor
	obb_cube_collider(obb_cube_collider&& other) = default; // Move Constructor

	obb_cube_collider& operator = (const obb_cube_collider& other) = default;// Copy Assignment Operator
	obb_cube_collider& operator = (obb_cube_collider&& other) noexcept = default; // Move Assignment Operator

	collider_type get_collider() const noexcept override { return collider_type::obb_cube; };
};

class plane_collider final : public collider_component {

public:
	plane_collider() = default;;
	~plane_collider()override = default;;
	plane_collider(const plane_collider& other) = default; // Copy Constructor
	plane_collider(plane_collider&& other) = default; // Move Constructor

	plane_collider& operator = (const plane_collider& other) = default;// Copy Assignment Operator
	plane_collider& operator = (plane_collider&& other) noexcept = default; // Move Assignment Operator

	collider_type get_collider() const noexcept override { return collider_type::plane; };
	auto get_normal() const noexcept-> XMVECTOR { return m_normal; }
	float get_offset() const noexcept { return m_offset; }

	void set_normal(const XMFLOAT3& centre, const XMFLOAT3& pointOne, const XMFLOAT3& pointTwo);
	void set_offset(const float offset);

private:
	XMVECTOR m_normal{};
	float m_offset{};
	float m_offsetTest{};
};

class cylinder_collider final : public collider_component {

public:
	cylinder_collider() = default;;
	~cylinder_collider()override = default;;
	cylinder_collider(const cylinder_collider& other) = default; // Copy Constructor
	cylinder_collider(cylinder_collider&& other) = default; // Move Constructor

	cylinder_collider& operator = (const cylinder_collider& other) = default;// Copy Assignment Operator
	cylinder_collider& operator = (cylinder_collider&& other) noexcept = default; // Move Assignment Operator

	collider_type get_collider() const noexcept override { return collider_type::peg; }
};