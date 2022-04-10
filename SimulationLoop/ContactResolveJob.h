#pragma once
#include "IJob.h"
#include "ResponseManager.h"

class contact_resolve_job final :
    public i_job
{
protected:
	response_manager* _responseManager;
public:
	contact_resolve_job(response_manager* response);
	~contact_resolve_job();
	contact_resolve_job() = default;
	contact_resolve_job(const contact_resolve_job&) = delete;
	contact_resolve_job(contact_resolve_job&&) = delete;
	contact_resolve_job& operator=(const contact_resolve_job&) = delete;
	contact_resolve_job& operator=(contact_resolve_job&&) = delete;
	void do_work() override;
};

