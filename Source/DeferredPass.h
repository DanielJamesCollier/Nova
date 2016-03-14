#pragma once
#include "ShaderProgram.h"
#include "ShaderBinder.h"


namespace Nova
{
	class DeferredPass
	{
	public:
		virtual ~DeferredPass()
		{
		}

		virtual bool Init()
		{
			m_initialised = true;
			return true;
		}
		
		void Enable();
		bool IsInitialised();

	protected:
		ShaderProgram* m_program     = nullptr;
		bool           m_initialised = false;
	};


}
