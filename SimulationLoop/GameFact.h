#pragma once
#include "GameObject.h"
#include <vector>
#include <Windows.h>

class game_object_factory
{
public:
	game_object_factory(std::vector<game_object*>& gameObjects);
	game_object_factory() = delete;
	game_object_factory(const game_object_factory& other) = default; // Copy Constructor
	game_object_factory(game_object_factory&& other) noexcept = default; // Move Constructor
	game_object_factory& operator = (const game_object_factory& other) = delete; // Copy Assignment Operator
	game_object_factory& operator = (game_object_factory&& other) noexcept = delete; // Move Assignment Operator

	~game_object_factory() = default;
	void add_game_object(const HWND hwnd, const XMFLOAT3& position, const XMFLOAT3& rotation, XMFLOAT3& scale, const XMFLOAT3& velocity, const XMFLOAT3& angularVelocity, const collider_component::collider_type& colliderType, const object::object_type& objType, const bool& useGravity, const float& mass, const float& drag, const float& angularDrag);
private:
	std::vector<game_object*>& m_game_objects_;
};

