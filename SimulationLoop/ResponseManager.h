#pragma once
#include "contact_manifold.h"

class response_manager
{
public:
	static std::mutex _responseMutex;
	response_manager(contact_manifold* contactManifold, const int positionIterations, const int velocityIterations, const float positionEpsilon, const float velocityEpsilon);
	~response_manager() = default;
	response_manager() = delete;
	response_manager(const response_manager& other) = default; // Copy Constructor
	response_manager(response_manager&& other) noexcept = default;// Move Constructor
	response_manager& operator = (const response_manager& other) = delete;// Copy Assignment Operator
	response_manager& operator = (response_manager&& other) noexcept = delete; // Move Assignment Operator
	void contact_resolve(const float dt);

private:
	void ready_contacts(const float dt) const;
	void change_pos(const float dt);
	void change_vel(const float dt);

	int m_position_iterations_done_;
	int m_position_iterations_;
	int m_velocity_iterations_count_;
	int m_velocity_count_;

	float m_pos_epsilon_;
	float m_vel_epsilon_;

	contact_manifold* m_contact_manifold_;
};