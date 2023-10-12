#include "../Math/Vector.h"



#include "../Math/Interval.h"

#include "Ray.h"


Point3 Ray::at(const double t) const
{
	return origin + (direction * t);
}


