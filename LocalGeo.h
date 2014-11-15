#pragma once

#include "Eigen/Dense"

class LocalGeo
{
public:
	Eigen::Vector3f p;
	Eigen::Vector3f n;

	LocalGeo(const Eigen::Vector3f & point, const Eigen::Vector3f & normal)
		:p(point),n(normal)
	{
		n.normalize();
	}
};