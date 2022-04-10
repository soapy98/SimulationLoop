#pragma once
#include <DirectXMath.h>
#include <glm/glm.hpp>

using namespace DirectX;

class position_component
{
public:
	position_component();
	position_component(XMFLOAT3 position);
	position_component(float x, float y, float z);
	position_component(const position_component& other) = default;
	position_component(position_component&& other) noexcept = default;
	position_component& position_component::operator=(const position_component& other) = default;
	~position_component() = default;; //Destructor

	position_component& operator = (position_component&& other) noexcept = default; // Move Assignment Operator

	const XMFLOAT3& get_position() const noexcept { return m_position; }
	auto set_new_position(const XMFLOAT3& newPosition)noexcept -> void { m_new_position_ = newPosition; };
	void set_new_position(const float x, const float y, const float z) { m_new_position_ = XMFLOAT3(x, y, z); };
	void change_position(const XMFLOAT3& translate)
	{
		m_position = XMFLOAT3(m_position.x + translate.x, m_position.y + translate.y, m_position.z + translate.z);
	};
	void change_position(const float x, const float y, const float z)
	{
		m_position = XMFLOAT3(m_position.x + x, m_position.y + y, m_position.z + z);
	}
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_new_position_;
};

