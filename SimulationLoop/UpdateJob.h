#pragma once
#include "IJob.h"
#include "PhysicsManager.h"
class update_job final :
    public i_job
{
	physics_manager* _physics;
public:
	update_job(physics_manager* physics);
	update_job(const update_job&) = delete;
	update_job(update_job&&) = delete;
	update_job& operator=(const update_job&) = delete;
	update_job& operator=(update_job&&) = delete;
	~update_job()override;
	void do_work() override;
};

