#include "ContactResolveJob.h"

#include "Timing.h"

contact_resolve_job::contact_resolve_job(response_manager* response):_responseManager(response)
{
}

contact_resolve_job::~contact_resolve_job()
{
}

void contact_resolve_job::do_work()
{
	_responseManager->contact_resolve(timing::TheTime.get_delta_time());
}
