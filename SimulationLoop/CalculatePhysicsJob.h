#pragma once
#include "IJob.h"
#include "PhysicsManager.h"

class calculate_physics_job final :
    public i_job
{
	physics_manager* _phys{};
	vector<game_object*>m_game_objects_{};
public:
	calculate_physics_job(physics_manager* physics , vector<game_object*> gos);
	~calculate_physics_job()override;
	calculate_physics_job() = default;
	calculate_physics_job(const calculate_physics_job&) = delete;
	calculate_physics_job(calculate_physics_job&&) = delete;
	calculate_physics_job& operator=(const calculate_physics_job&) = delete;
	calculate_physics_job& operator=(calculate_physics_job&&) = delete;
	void do_work() override;
};

