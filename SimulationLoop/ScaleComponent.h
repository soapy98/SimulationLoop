#pragma once
#include <DirectXMath.h>
#include <glm/glm.hpp>
using namespace DirectX;
class scale_component
{
public:
	scale_component();
	scale_component(const XMFLOAT3& scale);
	scale_component(const scale_component& other) = default;; // Copy Constructor
	scale_component(scale_component&& other) noexcept = default;; // Move Constructor
	~scale_component() = default;; // Destructor
	scale_component& operator = (const scale_component& other) = default;; // Copy Assignment Operator
	scale_component& operator = (scale_component&& other) noexcept = default;; // Move Assignment Operator
	const XMVECTOR& get_scale() const noexcept {
		return m_scale;
	}
	void set_scale(const XMVECTOR& newScale)noexcept {
		m_scale = newScale;
	}
	void add_scale(const XMVECTOR& otherScale)noexcept {
		m_scale += otherScale;
	}
private:
	XMVECTOR m_scale;
};


