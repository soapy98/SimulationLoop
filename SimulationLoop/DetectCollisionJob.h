#pragma once
#include "IJob.h"
#include "CollisionManager.h"
class detect_collision_job final :
    public i_job
{
protected:
	collision_manager* _phys;
public:
	detect_collision_job() = default;
	detect_collision_job(collision_manager* physics);
	detect_collision_job(const detect_collision_job&) = delete;
	detect_collision_job(detect_collision_job&&) = delete;
	detect_collision_job& operator=(const detect_collision_job&) = delete;
	detect_collision_job& operator=(detect_collision_job&&) = delete;
	~detect_collision_job() override;
	void do_work() override;
};

