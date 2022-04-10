#pragma once

#include <Windows.h>
#include <vector>
#include <DirectXMath.h>
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include "GameFact.h"
#include "ResponseManager.h"
#include "GameObject.h"
#include <mutex>

using namespace DirectX;

auto constexpr FULL_SCREEN = false;
auto constexpr  VSYNC_ENABLED = true;
auto constexpr SCREEN_DEPTH = 1000.0f;
auto constexpr SCREEN_NEAR = 0.1f;

class renderer
{
public:
	static std::mutex _gameMutex;
	renderer() = delete;
	renderer(int screenWidth, int screenHeight, HWND hwnd); // Default Constructor
	renderer(const renderer& other) = delete; // Copy Constructor
	renderer(renderer&& other) noexcept = default;; // Move Constructor
	~renderer(); // Destructor

	renderer& operator = (const renderer& other) = delete; // Copy Assignment Operator
	renderer& operator = (renderer&& other) noexcept = default;; // Move Assignment Operator
	void pause_simulation();

	void clear_spheres_();

	void add_number_of_spheres(const HWND hwnd);

	void add_time_scale(const int number);
	void add_sphere_diameter(const float diameter);
	void add_friction(const float friction);
	void add_restitution(const float restitution);
	bool loop();
	bool get_initialization_state() const;

	void console_update();
	FILE* file;
	bool on = true;
private:
	bool render();

	bool m_initialization_failed_;
	std::unique_ptr < game_object_factory> m_game_object_factory_;

	vector<game_object*> m_game_objects_;

	std::unique_ptr<physics_manager>m_physics_manager_;
	std::unique_ptr<collision_manager>m_collision_manager_;
	std::unique_ptr<response_manager>m_resolution_manager_;
	std::unique_ptr<object>m_object_;

	bool m_pause_simulation_;

	int m_time_scale_;
	int m_total_spheres_in_system_;
	int m_number_of_spheres_to_add_;
	float m_sphere_diameter_;
	float m_friction_;
	float m_restitution_;
	float m_elastic_;
	float m_dt_;
	float m_fps_;
};