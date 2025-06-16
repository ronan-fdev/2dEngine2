#pragma once

using namespace std::chrono;

class Timer
{
private:
	time_point<steady_clock> m_StartPoint, m_PausedPoint;
	bool m_bIsRunning{ false }, m_bIsPaused{ false };

public:
	Timer();
	~Timer() = default;
	void Start();
	void Stop();
	void Pause();
	void Resume();
	const int64_t ElapsedMS();
	const int64_t ElapsedSec();
	inline const bool IsRunning() const { return m_bIsRunning; }
	inline const bool IsPaused() const { return m_bIsPaused; }
};
