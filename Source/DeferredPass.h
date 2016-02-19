#pragma once
#include "ShaderProgram.h"
#include "ShaderBinder.h"

#define UNIFORM_NOT_FOUND -1

namespace Nova
{
	class DeferredPass
	{
	public:
		DeferredPass();
		virtual ~DeferredPass();

		virtual bool Init()
		{
			m_initialised = true;
			return true;
		}
		
		void Enable();

		bool IsInitialised();

	protected:
		ShaderProgram* m_program;

		bool m_initialised = false;
	
	};


}
