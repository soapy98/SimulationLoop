#pragma once
#include <string>

class i_job
{
public:
	i_job() = default;
	i_job(const i_job&) = delete;
	i_job(i_job&&) = delete;
	i_job& operator=(const i_job&) = delete;
	i_job& operator=(i_job&&) = delete;

	virtual ~i_job() = default;
	virtual void do_work() = 0;
	const std::string& get_name() const noexcept { return _name; }
	void set_name(const std::string& i_name) { _name = i_name; }
	void set_frame_rate(const float fr) noexcept { _frame_rate = fr; }
	float get_frame_rate() const noexcept{ return _frame_rate; }
private:
	std::string _name;
	float _frame_rate = 60.f;
};

