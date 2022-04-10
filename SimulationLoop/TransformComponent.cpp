#include "TransformComponent.h"

transform_component::transform_component()
{
	add_position_component();
	add_rotation_component();
	add_scale_component();
}

transform_component::transform_component(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale)
{
	add_position_component(position);
	add_rotation_component(rotation);
	add_scale_component(scale);
}

transform_component::transform_component(const XMFLOAT3& scale)
{
	add_position_component();
	add_rotation_component();
	add_scale_component(scale);
}
void transform_component::add_position_component() {
	pos_ = new position_component();
}

void transform_component::add_position_component(const XMFLOAT3& position) {
	pos_ = new position_component(position);
}

void transform_component::add_position_component(const float x, const float y, const float z) {
	pos_ = new position_component(x, y, z);
}

void transform_component::add_rotation_component() {
	rot_ = new rotation_component();
}

void transform_component::add_rotation_component(const XMFLOAT4& rotation) {
	rot_ = new rotation_component(rotation);
}

void transform_component::add_rotation_component(const float x, const float y, const float z, const float w) {
	rot_ = new rotation_component(x, y, z, w);
}
void transform_component::add_scale_component() {
	scale_ = new scale_component();
}

void transform_component::add_scale_component(const XMFLOAT3& scale) {
	scale_ = new scale_component(scale);
}

