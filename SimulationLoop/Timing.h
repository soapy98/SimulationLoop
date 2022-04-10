#pragma once
#include <cstdint>
#include <mutex>

class timing
{
public:
	static timing TheTime;
	static std::mutex _time_mutex;
	static std::mutex _dtime_mutex;
	timing();

	~timing();

	void update();

	float get_delta_time() const { std::lock_guard<std::mutex>lock(_dtime_mutex); return dt_; }

	double get_time() const;

	float get_float_time() const
	{
		std::lock_guard<std::mutex>lock(_time_mutex);
		return static_cast<float>(get_time());
	}

	float get_frame_start()const { std::lock_guard<std::mutex>lock(_time_mutex); return current_frame_start_time_; }

	void set_desired_frame_time(float time) { desired_frame_time_ = time; }

	double get_last_frame_time() const
	{
		std::lock_guard<std::mutex>lock(_time_mutex); return last_frame_start_time_;
	}
private:
	float dt_;
	uint64_t d_ttick_;

	double last_frame_start_time_;
	float current_frame_start_time_;
	double count_duration_;

	float desired_frame_time_;
};

