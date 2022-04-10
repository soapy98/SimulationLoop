#include "DetectCollisionJob.h"
detect_collision_job::detect_collision_job(collision_manager* physics):_phys(physics)
{
}
detect_collision_job::~detect_collision_job()
{
}

void detect_collision_job::do_work()
{
	_phys->dynamic_collision_detection();
}
