#pragma once
#include "contact_manifold.h"
//#include "ContactManifold.h"
#include <algorithm>
#include <cmath>
#include <map>
//Manager to handle detecting collisions and then returning the correct response
class collision_manager
{

public:
	static std::mutex collisionMutex;
	collision_manager() = delete;
	collision_manager(const collision_manager& other) = default; // Copy Constructor
	collision_manager(collision_manager&& other) noexcept = default;// Move Constructor
	collision_manager& operator = (const collision_manager& other) = delete;// Copy Assignment Operator
	collision_manager& operator = (collision_manager&& other) noexcept = delete; // Move Assignment Operator

	collision_manager(vector<game_object*>& gameObjects, float friction, float restitution);
	~collision_manager();
	void set_friction(const float friction) noexcept { m_friction_ = friction; };
	void set_restitution(const float restitution) noexcept { m_restitution_ = restitution; };
	void dynamic_collision_detection();
	contact_manifold* get_contact_manifold_ref() const;
	void plane_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo);
	typedef void (collision_manager::* collision_function)(game_object*, game_object*);

private:
	//Sphere on Sphere Collision Detection
	void sphere_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo) ;

	//Cylinder Collision Detection
	void cylinder_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo);
	void sphere_on_cylinder_detection(game_object* gameObjectOne, game_object* gameObjectTwo);

	//Half-Space Sphere Collision Detection
	void sphere_on_plane_detection(game_object* gameObjectOne, game_object* gameObjectTwo);

	//ABB Sphere Collision Detection
	void aabb_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo);
	void sphere_on_aabb_detection(game_object* gameObjectOne, game_object* gameObjectTwo);

	//OBB Sphere Collision Detection
	void obb_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo);
	void sphere_on_obb_detection(game_object* gameObjectOne, game_object* gameObjectTwo);

	float m_friction_;
	float m_restitution_;
	vector<game_object*>& m_game_objects_;
	contact_manifold* m_contact_manifold_;
	//Function map holds each collision which can occur 
	map<pair<collider_component::collider_type, collider_component::collider_type>, collision_function> m_col_func_map_{};
};

