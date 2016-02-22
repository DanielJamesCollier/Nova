#include "ProfileManager.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>
#include <sstream>

namespace Nova
{
	namespace Profiler
	{
		std::vector<ProfileTimer> ProfileManager::m_pTimers;
		unsigned int ProfileManager::m_FPS;

		ProfileManager::ProfileManager()
		{
		}


		ProfileManager::~ProfileManager()
		{
		}

		void ProfileManager::Begin(const std::string& name)
		{

			if (m_pTimers.empty())
			{
				m_pTimers.push_back(ProfileTimer(name));
				return;
			}

			bool found = false;

			for (ProfileTimer& p : m_pTimers)
			{
				if (p.m_name == name)
				{
					found = true;
				}
			}

			if (!found)
			{
				m_pTimers.push_back(ProfileTimer(name));
			}
					
		}

		void ProfileManager::End(const std::string& name)
		{
			for (ProfileTimer& p : m_pTimers)
			{
				if (p.m_name == name)
				{
					p.End();
					return;
				}
			}
		}
		
		void ProfileManager::Display()
		{
			double frameTime = static_cast<double>(1.0) / static_cast<double>(m_FPS);

			Logger& log = Logger::GetInstance();

			log.InfoBlockBegin("Profiler Info");

			std::ostringstream fps; fps << "FPS: " << m_FPS << std::endl;

			log.InfoBlockMessage(fps.str());

			for (ProfileTimer p : m_pTimers)
			{
				double percent = (p.m_timeSpan.count() / frameTime) * 100.0;

				std::ostringstream buffer;

				

				buffer << p.m_name << ": " << p.m_timeSpan.count() << "(" << percent << "%)" << std::endl;

				log.InfoBlockMessage(buffer.str());
			}
			log.InfoBlockEnd();

			m_pTimers.clear();
		}

		int ProfileManager::GetNumProfilers()
		{
			return m_pTimers.size();
		}

	}
}
