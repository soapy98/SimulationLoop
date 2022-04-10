#include "CalculatePhysicsJob.h"

#include "Timing.h"

calculate_physics_job::calculate_physics_job(physics_manager* physics, vector<game_object*> gos):_phys(physics),m_game_objects_(gos)
{
}

calculate_physics_job::~calculate_physics_job()
{
	//delete _phys;
}

void calculate_physics_job::do_work()
{
	_phys->calculate_game_object_physics(timing::TheTime.get_delta_time());
}
