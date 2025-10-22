#ifndef PX_TIMER_HPP
#define PX_TIMER_HPP

#include <chrono>

namespace px
{
	class Timer
	{
	public:
		Timer() { startTime_ = std::chrono::steady_clock::now(); }

		// Time elapsed since start...
		unsigned int Elapsed() const
		{
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
			return static_cast<unsigned int>(duration);
		}

		// Duration since last duration call/start... resets every time duration is called.
		unsigned int Duration()
		{
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
			startTime_ = endTime;
			return static_cast<unsigned int>(duration);
		}

		void Reset()
		{
			startTime_ = std::chrono::steady_clock::now();
		}

	private:
		std::chrono::steady_clock::time_point startTime_;
	};
}

#endif // PX_TIMER_HPP