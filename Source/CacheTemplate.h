#pragma once
#include <string>

namespace Nova
{
	class CacheTemplate
	{
	public:
		CacheTemplate() = default;

		virtual void Init() = 0;
		virtual void Dispose() = 0;
		virtual void Dispose(const std::string& itemToDelete) = 0;
		virtual void PrintCacheContent() = 0;

	protected:
		bool m_initialised = false;
		bool m_disposed    = true;
	};
}
