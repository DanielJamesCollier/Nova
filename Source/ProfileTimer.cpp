#include "ProfileTimer.h"
#include <iostream>
#include <chrono>

namespace Nova
{
	namespace Profiler
	{
		ProfileTimer::ProfileTimer(std::string name)
			:
			m_name(name)
		{
			m_start = std::chrono::system_clock::now();
		}


		ProfileTimer::~ProfileTimer()
		{
		}

		void ProfileTimer::End()
		{
			if (m_ended) return;
			m_ended = true;

			m_end = std::chrono::system_clock::now();
			m_timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(m_end - m_start);
		}

	}
}
