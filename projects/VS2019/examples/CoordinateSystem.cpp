#include "CoordinateSystem.h"

/*----------------------------------------------------------------------------------------------------------------------------
													CONVERSION
----------------------------------------------------------------------------------------------------------------------------*/
Polar CartesianToPolar(Vector2 cart, bool keepThetaPositive)
{
	Polar polar = { Vector2Length(cart),atan2f(cart.y,cart.x) };
	if (keepThetaPositive && polar.theta < 0)polar.theta += 2 * PI;
	return polar;
}

Vector2 PolarToCartesian(Polar polar)
{
	return Vector2Scale({ cosf(polar.theta),sinf(polar.theta) }, polar.rho);
}

Cylindrical CartesianToCylindrical(Vector3 cart, bool keepThetaPositive) {
	Polar polar = CartesianToPolar(Vector2{ cart.x, cart.z }, keepThetaPositive);
	return Cylindrical{ polar.rho, polar.theta, cart.y };
}

Vector3 CylindricalToCartesian(Cylindrical cylind) {
	Vector2 cart = PolarToCartesian(Polar{ cylind.rho, cylind.theta });
	return Vector3{ cart.x, cylind.y, cart.y };
}

Spherical CartesianToSpherical(Vector3 cart, bool keepThetaPositive) {
	float theta = atan2f(cart.z, cart.x);
	if (keepThetaPositive && theta < 0)theta += 2 * PI;
	return Spherical{
		Vector3Length(cart),
		theta,
		acosf(cart.y / Vector3Length(cart))
	};
}
Vector3 SphericalToCartesian(Spherical spherical) {
	return Vector3{
		spherical.rho * sinf(spherical.phi) * cosf(spherical.theta) ,
		spherical.rho * cosf(spherical.phi) ,
		spherical.rho * sinf(spherical.phi) * sinf(spherical.theta)
	};
}

/*----------------------------------------------------------------------------------------------------------------------------
													MATH WITH SPHERICAL
----------------------------------------------------------------------------------------------------------------------------*/
Spherical SphericalAdd(Spherical a, Spherical b) { //Ajoute deux Spherical ensemble
	return Spherical{
		a.rho + b.rho,
		a.theta + b.theta,
		a.phi + b.phi
	};
}

Spherical SphericalMultiplies(Spherical a, Spherical b) { //Multiplie deux Spherical ensemble
	return Spherical{
		a.rho * b.rho,
		a.theta * b.theta,
		a.phi * b.phi
	};
}

Spherical SphericalMultVector2(Spherical a, Vector2 b) { //Multiplie le theta et phi d'un Spherical par un Vector2
	return Spherical{
		a.rho,
		a.theta * b.x,
		a.phi * b.y
	};
}
