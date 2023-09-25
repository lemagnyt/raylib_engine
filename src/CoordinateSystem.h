#pragma once

#include "raylib.h"
#include <raymath.h>
#include "rlgl.h"
#include <math.h>
#include <float.h>
#include <vector>

/*----------------------------------------------------------------------------------------------------------------------------
													STRUCT
----------------------------------------------------------------------------------------------------------------------------*/
struct Polar {
	float rho;
	float theta;
};

struct Cylindrical {
	float rho;
	float theta;
	float y;
};

struct Spherical {
	float rho;
	float theta;
	float phi;
};

/*----------------------------------------------------------------------------------------------------------------------------
													CONVERSION
----------------------------------------------------------------------------------------------------------------------------*/
Polar CartesianToPolar(Vector2 cart, bool keepThetaPositive = true);
Vector2 PolarToCartesian(Polar polar);
Cylindrical CartesianToCylindrical(Vector3 cart, bool keepThetaPositive = true);
Vector3 CylindricalToCartesian(Cylindrical cylind);
Spherical CartesianToSpherical(Vector3 cart, bool keepThetaPositive = true);
Vector3 SphericalToCartesian(Spherical spherical);

/*----------------------------------------------------------------------------------------------------------------------------
													MATH WITH SPHERICAL
----------------------------------------------------------------------------------------------------------------------------*/
Spherical SphericalAdd(Spherical a, Spherical b); //Ajoute deux Spherical ensemble
Spherical SphericalMultiplies(Spherical a, Spherical b); //Multiplie deux Spherical ensemble
Spherical SphericalMultVector2(Spherical a, Vector2 b); //Multiplie le theta et phi d'un Spherical par un Vector2
