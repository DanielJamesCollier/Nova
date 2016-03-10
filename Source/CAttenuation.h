#pragma once

namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			namespace Light
			{
				struct CAttenuation
				{
					CAttenuation(float constant = 1.0f, float linear = 0.001f, float exp = 10.0f)
					{
						this->constant = constant;
						this->linear = linear;
						this->exp = exp;
					}

					float constant;
					float linear;
					float exp;
				};
			}
		}
	}
}