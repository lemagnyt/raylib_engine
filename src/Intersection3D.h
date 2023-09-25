#pragma once

#include "My3DPrimitives.h"

/*----------------------------------------------------------------------------------------------------------------------------
												 REFERENCIEL
----------------------------------------------------------------------------------------------------------------------------*/

Vector3 LocalToGlobalVect(Vector3 localVect, ReferenceFrame localRef); // Modification des coordonnées d'un vecteur du référentiel local au référentiel global
Vector3 GlobalToLocalVect(Vector3 globalVect, ReferenceFrame localRef); // Modification des coordonnées d'un vecteur du référentiel global au référentiel local

/*----------------------------------------------------------------------------------------------------------------------------
													INTERSECTION
----------------------------------------------------------------------------------------------------------------------------*/
bool IntersectLinePlane(Line line, Plane plane, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre une ligne et une plane
bool IntersectSegmentPlane(Segment seg, Plane plane, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et une plane
bool IntersectSegmentQuad(Segment seg, Quad quad, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et un quad
bool IntersectSegmentDisk(Segment seg, Disk disk, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et un disque
bool IntersectSegmentSphere(Segment seg, Sphere s, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et une sphère
bool IntersectSegmentInfiniteCylinder(Segment segment, InfiniteCylinder cyl, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et un cylindre infini
bool IntersectSegmentBox(Segment seg, Box box, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et une box
bool OBB(Segment seg, Box box); // Détection de l'OBB (segment dans la box ou en collision avec)
bool IntersectSegmentCylinder(Segment segment, Cylinder cyl, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et un cylindre
bool IntersectSegmentCapsule(Segment seg, Capsule capsule, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et une capsule
bool IntersectSegmentRoundedBox(Segment seg, RoundedBox rndBox, float& t, Vector3& interPt, Vector3& interNormal); // Intersection entre un segment et une roundedbox
bool GetSphereNewPositionAndVelocityIfCollidingWithRoundedBox(Sphere sphere, RoundedBox rndBox, Vector3 velocity, float deltaTime, float& colT, Vector3& colSpherePos, Vector3& colNormal, Vector3& newPosition, Vector3& newVelocity); // Intersection entre une sphère et une roundedbox avec changement de velocity et de position si collision
bool GetSphereNewPositionAndVelocityIfCollidingWithRoundedBoxes(Sphere sphere, const std::vector<RoundedBox>& rndBoxes, Vector3 velocity, float deltaTime, Vector3& posRndBoxBefore, float& colT, Vector3& colSpherePos, Vector3& colNormal, Vector3& newPosition, Vector3& newVelocity); // Intersection entre une sphère et une liste de roundedbox avec changement de velocity et de position si collision
bool GetSphereNewPositionAndVelocityIfMultiCollidingWithRoundedBoxes(Sphere sphere, const std::vector<RoundedBox>& rndBoxes, Vector3 velocity, float rotInertia, Vector3 angularMomentum, float deltaTime, int nMaxSuccessiveCollisions, Vector3& newPosition, Vector3& newVelocity, Vector3& newAngularMomentum); // Intersection entre une sphère et une liste de roundedbox avec changement de velocity et de position si collision pour multiples collision en une frame