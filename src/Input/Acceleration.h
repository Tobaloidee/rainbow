#ifndef ACCELERATION_H_
#define ACCELERATION_H_

#include "Common/Vec3.h"

#define kFilteringFactor 0.1

/// Structure representing immediate, three-dimensional acceleration data.
///
/// Implements a simple high-pass filter that removes the gravity component from
/// the acceleration data.
///
/// Copyright 2011-12 Bifrost Entertainment. All rights reserved.
/// \author Tommy Nguyen
class Acceleration
{
public:
	double timestamp;         ///< The relative time at which the acceleration event occurred.
	const double &x, &y, &z;  ///< Filtered acceleration data.

	inline Acceleration();

	/// Update acceleration data.
	/// \param x,y,z  Raw acceleration data.
	/// \param t      The relative time at which the acceleration event occurred.
	inline void update(const double x, const double y, const double z, const double t);

private:
	Vec3d pass;  ///< Filtered acceleration data.
};

Acceleration::Acceleration() :
	timestamp(0.0), x(pass.x), y(pass.y), z(pass.z) { }

void Acceleration::update(const double x, const double y, const double z, const double t)
{
	// Calculate the low-pass value and subtract it from the measured value.
	this->pass.x = x - ((x * kFilteringFactor) + (this->pass.x * (1.0 - kFilteringFactor)));
	this->pass.y = y - ((y * kFilteringFactor) + (this->pass.y * (1.0 - kFilteringFactor)));
	this->pass.z = z - ((z * kFilteringFactor) + (this->pass.z * (1.0 - kFilteringFactor)));
	this->timestamp = t;
}

#endif
