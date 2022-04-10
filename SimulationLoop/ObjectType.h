#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include <gl\gl.h>  
#include <gl\GLU.h>
#include "PositionComponent.h"
#include "RotationComponent.h"


using namespace DirectX;
using namespace std;

class object
{
public:

	enum class object_type
	{
		sphere,
		cube,
		plane,
		peg,
	};
	object() = default;
	object(object_type modelType, XMFLOAT3& pos, XMFLOAT4& rot,XMFLOAT3& scale);
	object(object_type modelType);

	object(const object& other) = default;; // Copy Constructor
	object(object&& other) noexcept = default;; // Move Constructor
	~object() = default; // Destructor

	object& operator = (const object& other) = default;; // Copy Assignment Operator
	object& operator = (object&& other) noexcept = default;; // Move Assignment Operator

	void render(XMVECTOR& pos, XMVECTOR& r, XMVECTOR& s);
	bool get_initialization_state() const noexcept { return m_fail_; }
	object_type get_object_type() const noexcept { return m_model_type_; }

	void render_cube() const;
	void render_sphere() const;
	void render_peg() const;
private:
	GLuint m_texture_;
	GLuint m_wall_text_;
	GLuint m_peg_text_;
	XMVECTOR translate_;
	XMVECTOR scale_;
	XMVECTOR rot_;
	XMVECTOR col_;
	float m_radius_;
	bool m_fail_;
	object_type m_model_type_;
};