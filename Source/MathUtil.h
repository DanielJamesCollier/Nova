#pragma once
#include <math.h>	

#define PI 3.141592565

namespace Nova
{
	class MathUtil
	{
	public:
		static float DegreesToRadiansf(float deg)
		{
			return static_cast<float>(deg * (PI / 180));
		}

		static double DegreesToRadiansd(double deg)
		{
			return deg * (PI / 180);
		}
	};
}