#include "UpdateJob.h"

update_job::update_job(physics_manager* p):_physics(p)
{
}

update_job::~update_job()
{
}

void update_job::do_work()
{
	_physics->update_game_object_physics();
}
