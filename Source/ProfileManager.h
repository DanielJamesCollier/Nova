#pragma once
#include "ProfileTimer.h"
#include <vector>
#include <string>

namespace Nova
{
	namespace Profiler
	{
		class ProfileManager
		{
		public:
			ProfileManager();
			~ProfileManager();

			static void Begin(const std::string& name);
			static void End(const std::string& name);
			static void Display();

			static int GetNumProfilers();

			
		private:
			static std::vector<ProfileTimer> m_pTimers;
			static unsigned int				 m_FPS;

			friend class Nova;
		};

	}
}
