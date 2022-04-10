#pragma once
#include <DirectXMath.h>
#include "PositionComponent.h"
#include "RotationComponent.h"
#include "ScaleComponent.h"
//Transform component holds all information to do with transformations and caches them to be used
class transform_component
{
public:
	transform_component();
	transform_component(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale );
	transform_component(const XMFLOAT3 &scale);
	transform_component(const transform_component& other) = default;; // Copy Constructor
	transform_component(transform_component&& other) noexcept = default;; // Move Constructor
	~transform_component() = default;// Destructor

	transform_component& operator = (const transform_component& other) = default;; // Copy Assignment Operator
	transform_component& operator = (transform_component&& other) noexcept = default;; // Move Assignment Operator
	//Position
	position_component* get_position_component() const noexcept { return pos_; }
	void add_position_component();
	void add_position_component(const XMFLOAT3& position);
	void add_position_component(const float x, const float y, const float z);
	const XMFLOAT3& get_position() const noexcept { return pos_->get_position(); }
	void set_new_position(const XMFLOAT3& newPosition) const noexcept { pos_->set_new_position(newPosition); }
	void set_new_position(const float x, const float y, const float z) const { pos_->set_new_position(x, y, z); }
	void change_position(const XMFLOAT3& translate) const { pos_->change_position(translate); }
	void change_position(const float x, const float y, const float z) const { pos_->change_position(x, y, z); }

	//RotationComponent
	rotation_component* get_rotation_component() const noexcept { return rot_; };
	void add_rotation_component();
	void add_rotation_component(const XMFLOAT4& rotation);
	void add_rotation_component(const float x, const float y, const float z, const float w);
	void set_rotation(const XMFLOAT4& newRotation) const  {rot_->set_rotation(newRotation);}
	void set_rotation(const float x, const float y, const float z, const float w) const  {rot_->set_rotation(x, y, z, w);}
	void add_rotation(const XMFLOAT4& otherRotation) const  {rot_->add_rotation(otherRotation);}
	void add_rotation(const float x, const float y, const float z, const float w) const  {rot_->add_rotation(x, y, z, w);}
	const XMFLOAT4& get_rotation() const noexcept { return rot_->get_rotation(); }

	//Scale
	scale_component* get_scale_component() const noexcept { return scale_; };
	void add_scale_component();
	void add_scale_component(const XMFLOAT3& scale);
	void add_scale(const XMVECTOR& otherScale) const  noexcept { scale_->add_scale(otherScale); }
	const XMVECTOR& get_scale() const  noexcept { return scale_->get_scale(); }
	void set_scale(const XMVECTOR& newScale) const  noexcept { scale_->set_scale(newScale); }
private:
	scale_component* scale_ = nullptr;
	position_component* pos_ = nullptr;
	rotation_component* rot_ = nullptr;
};

