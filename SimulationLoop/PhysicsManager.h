#pragma once
#include <vector>
#include "GameObject.h"
class physics_manager
{
public:
	physics_manager(vector<game_object*>& gameObjects);
	~physics_manager() = default;
	physics_manager() = delete;
	physics_manager(const physics_manager& other) = default; // Copy Constructor
	physics_manager(physics_manager&& other) noexcept = default;// Move Constructor
	physics_manager& operator = (const physics_manager& other) = delete;// Copy Assignment Operator
	physics_manager& operator = (physics_manager&& other) noexcept = delete; // Move Assignment Operator
	void calculate_game_object_physics(const float dt) const;
	void update_game_object_physics() const;
private:
	XMVECTOR m_gravity_;
	vector<game_object*>& m_game_objects_;
};
