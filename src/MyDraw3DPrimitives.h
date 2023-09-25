#pragma once

#include "My3DPrimitives.h"

/*----------------------------------------------------------------------------------------------------------------------------
													QUAD
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonQuad(Quad quad, Color color = LIGHTGRAY);
void MyDrawWireframeQuad(Quad quad, Color color = DARKGRAY);
void MyDrawQuad(Quad quad, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);

/*----------------------------------------------------------------------------------------------------------------------------
													DISK
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonDisk(Disk disk, int nSectors, Color color = LIGHTGRAY);
void MyDrawWireframeDisk(Disk disk, int nSectors, Color color = DARKGRAY);
void MyDrawDisk(Disk disk, int nSectors, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);

/*----------------------------------------------------------------------------------------------------------------------------
													BOX
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonBox(Box box, Color color = LIGHTGRAY);
void MyDrawWireframeBox(Box box, Color color = DARKGRAY);
void MyDrawBox(Box box, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);

/*----------------------------------------------------------------------------------------------------------------------------
													SPHERE
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonSphere(Sphere sphere, int nMeridians, int nParallels, Color color = LIGHTGRAY);
void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY);
void MyDrawSphere(Sphere sphere, int nMeridians, int nParallels, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);

/*----------------------------------------------------------------------------------------------------------------------------
													CYLINDER PORTION
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, Color color = LIGHTGRAY);
void MyDrawWireframeCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, Color color = DARKGRAY);
void MyDrawCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);

/*----------------------------------------------------------------------------------------------------------------------------
													CYLINDER
----------------------------------------------------------------------------------------------------------------------------*/

void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps = true, Color color = LIGHTGRAY);
void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = DARKGRAY);
void MyDrawCylinder(Cylinder cylinder, int nSectors, bool drawPolygon = true, bool drawWireframe = true);

/*----------------------------------------------------------------------------------------------------------------------------
													CAPSULE
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonCapsule(Capsule capsule, int nSectors, int nParallels, Color color = LIGHTGRAY);
void MyDrawWireframeCapsule(Capsule capsule, int nSectors, int nParallels, Color color = LIGHTGRAY);
void MyDrawCapsule(Capsule capsule, int nSectors, int nParallels, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);

/*----------------------------------------------------------------------------------------------------------------------------
													SPHERE PORTION
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, Color color = LIGHTGRAY);
void MyDrawWireframeSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, Color color = LIGHTGRAY);
void MyDrawSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);

/*----------------------------------------------------------------------------------------------------------------------------
													ROUNDED BOX
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonRoundedBox(RoundedBox roundedBox, int nSectors, Color color = LIGHTGRAY);
void MyDrawWireframeRoundedBox(RoundedBox roundedBox, int nSectors, Color color = DARKGRAY, bool light = false);
void MyDrawRoundedBox(RoundedBox roundedBox, int nSectors, bool light = false, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY);