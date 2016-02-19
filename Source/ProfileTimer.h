#pragma once
#include <time.h>
#include <chrono>
#include <string>

namespace Nova
{
	namespace Profiler
	{
		class ProfileTimer
		{
		public:
			
			ProfileTimer() = delete;
			~ProfileTimer();

		private:
			std::chrono::high_resolution_clock    m_clock;
			std::chrono::system_clock::time_point m_start;
			std::chrono::system_clock::time_point m_end;
			std::chrono::duration<double>         m_timeSpan;
			std::string							  m_name;
			bool								  m_ended = false;

			ProfileTimer(std::string name);

			void End();

			friend class ProfileManager;
			
		};
	}
}
