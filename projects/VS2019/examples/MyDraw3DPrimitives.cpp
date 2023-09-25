#include "MyDraw3DPrimitives.h"

/*----------------------------------------------------------------------------------------------------------------------------
													QUAD
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonQuad(Quad quad, Color color)
{
	int numVertex = 6;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	// BEGINNING OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
	// methods should be called in this order: rlTranslatef, rlRotatef & rlScalef
	// so that transformations occur in the opposite order: scale, then rotation, then translation
	rlPushMatrix();
	//TRANSLATION
	rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
	//ROTATION
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	//SCALING
	rlScalef(quad.extents.x, 1, quad.extents.z);
	// END OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(1, 0, 1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(1, 0, 1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, 1);
	rlEnd();
	//EVERY rlPushMatrix method call should be followed by a rlPopMatrix method call
	rlPopMatrix();
}

void MyDrawWireframeQuad(Quad quad, Color color)
{
	int numVertex = 10; //5 Lines donc 10 Points 
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(quad.extents.x, 1, quad.extents.z);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame 
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(1, 0, 1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(1, 0, 1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, 1);
	rlVertex3f(-1, 0, 1);
	rlVertex3f(1, 0, 1);
	rlEnd();
	rlPopMatrix();
}

void MyDrawQuad(Quad quad, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor)
{
	if (drawPolygon) MyDrawPolygonQuad(quad, polygonColor);
	if (drawWireframe)MyDrawWireframeQuad(quad, wireframeColor);
}

/*----------------------------------------------------------------------------------------------------------------------------
													DISK
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonDisk(Disk disk, int nSectors, Color color) {
	if (rlCheckBufferLimit(nSectors * 3)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(disk.radius, 1, disk.radius);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	for (int i = 0; i < nSectors; i++)
	{
		rlVertex3f(0, 0, 0);
		//On calcule les positions des points avec CylindricalToCartesian
		Vector3 CartPoint1 = CylindricalToCartesian({ 1, (i * (2 * PI)) / nSectors, 0 });
		Vector3 CartPoint2 = CylindricalToCartesian({ 1, ((i + 1) * (2 * PI)) / nSectors, 0 });
		rlVertex3f(CartPoint2.x, CartPoint2.y, CartPoint2.z);
		rlVertex3f(CartPoint1.x, CartPoint1.y, CartPoint1.z);
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeDisk(Disk disk, int nSectors, Color color) {
	if (rlCheckBufferLimit(nSectors * 4)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(disk.radius, 1, disk.radius);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);
	for (int i = 0; i < nSectors; i++)
	{
		rlVertex3f(0, 0, 0);
		//On calcule les positions des points avec CylindricalToCartesian
		Vector3 CartPoint1 = CylindricalToCartesian({ 1, (i * (2 * PI)) / nSectors, 0 });
		Vector3 CartPoint2 = CylindricalToCartesian({ 1, ((i + 1) * (2 * PI)) / nSectors, 0 });
		rlVertex3f(CartPoint2.x, CartPoint2.y, CartPoint2.z);
		rlVertex3f(CartPoint2.x, CartPoint2.y, CartPoint2.z);
		rlVertex3f(CartPoint1.x, CartPoint1.y, CartPoint1.z);
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawDisk(Disk disk, int nSectors, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonDisk(disk, nSectors, polygonColor);
	if (drawWireframe) MyDrawWireframeDisk(disk, nSectors, wireframeColor);
}
/*----------------------------------------------------------------------------------------------------------------------------
													BOX
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonBox(Box box, Color color) {
	int numVertex = 36; //6 Faces donc 36 Points
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(box.ref.origin.x, box.ref.origin.y, box.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(box.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(box.extents.x, box.extents.y, box.extents.z);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	//Draw Quads
#pragma region Quads
	ReferenceFrame ref = ReferenceFrame(
		{ 0,1,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	Quad quad = Quad{ ref, {1,0,1} };
	MyDrawPolygonQuad(quad);

	ref = ReferenceFrame({ 0,0,1 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
	quad = { ref ,{1,0,1} };
	MyDrawPolygonQuad(quad);

	ref = ReferenceFrame(
		{ 0,-1,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI));
	quad = { ref ,{1,0,1} };
	MyDrawPolygonQuad(quad);

	ref = ReferenceFrame(
		{ 0,0,-1 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), (3 * PI) / 2));
	quad = { ref ,{1,0,1} };
	MyDrawPolygonQuad(quad);

	ref = ReferenceFrame(
		{ 1,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), (3 * PI) / 2));
	quad = { ref ,{1,0,1} };
	MyDrawPolygonQuad(quad);

	ref = ReferenceFrame(
		{ -1,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
	quad = { ref ,{1,0,1} };
	MyDrawPolygonQuad(quad);
#pragma endregion

	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeBox(Box box, Color color) {
	int numVertex = 60; //6 Faces donc 60 Points
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(box.ref.origin.x, box.ref.origin.y, box.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(box.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(box.extents.x, box.extents.y, box.extents.z);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);

	//Draw Quads
#pragma region Quads
	ReferenceFrame ref = ReferenceFrame(
		{ 0,1,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	Quad quad = Quad{ ref, {1,0,1} };
	MyDrawWireframeQuad(quad);

	ref = ReferenceFrame({ 0,0,1 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
	quad = { ref ,{1,0,1} };
	MyDrawWireframeQuad(quad);

	ref = ReferenceFrame(
		{ 0,-1,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI));
	quad = { ref ,{1,0,1} };
	MyDrawWireframeQuad(quad);

	ref = ReferenceFrame(
		{ 0,0,-1 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), (3 * PI) / 2));
	quad = { ref ,{1,0,1} };
	MyDrawWireframeQuad(quad);

	ref = ReferenceFrame(
		{ 1,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), (3 * PI) / 2));
	quad = { ref ,{1,0,1} };
	MyDrawWireframeQuad(quad);

	ref = ReferenceFrame(
		{ -1,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
	quad = { ref ,{1,0,1} };
	MyDrawWireframeQuad(quad);
#pragma endregion

	rlEnd();
	rlPopMatrix();
}

void MyDrawBox(Box box, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonBox(box, polygonColor);
	if (drawWireframe) MyDrawWireframeBox(box, wireframeColor);
}

/*----------------------------------------------------------------------------------------------------------------------------
													SPHERE
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonSphere(Sphere sphere, int nMeridians, int nParallels, Color color) {
	if (rlCheckBufferLimit(nMeridians * nParallels * 6)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(sphere.radius, sphere.radius, sphere.radius);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	for (int i = 0; i < nParallels; i++)
	{
		for (int j = 0; j < nMeridians; j++) {
			//On calcule les 4 points d'une faces avec SphericalToCartesian
			Vector3 Point[] = {
				SphericalToCartesian({1, j * (2 * PI / nMeridians), i * (PI / nParallels)}),
				SphericalToCartesian({1, (j + 1) * (2 * PI / nMeridians), i * (PI / nParallels)}),
				SphericalToCartesian({1, j * (2 * PI / nMeridians), (i + 1) * (PI / nParallels)}),
				SphericalToCartesian({1, (j + 1) * (2 * PI / nMeridians), (i + 1) * (PI / nParallels)})
			};
			rlVertex3f(Point[0].x, Point[0].y, Point[0].z);
			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);

			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);
			rlVertex3f(Point[3].x, Point[3].y, Point[3].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);
		}
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color) {
	if (rlCheckBufferLimit(nMeridians * nParallels * 6)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(sphere.radius, sphere.radius, sphere.radius);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);
	for (int i = 0; i < nParallels; i++)
	{
		for (int j = 0; j < nMeridians; j++) {
			//On calcule les 4 points d'une faces avec SphericalToCartesian
			Vector3 Point[] = {
				SphericalToCartesian({1, j * (2 * PI / nMeridians), i * (PI / nParallels)}),
				SphericalToCartesian({1, (j + 1) * (2 * PI / nMeridians), i * (PI / nParallels)}),
				SphericalToCartesian({1, j * (2 * PI / nMeridians), (i + 1) * (PI / nParallels)}),
				SphericalToCartesian({1, (j + 1) * (2 * PI / nMeridians), (i + 1) * (PI / nParallels)})
			};
			rlVertex3f(Point[0].x, Point[0].y, Point[0].z);
			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);

			rlVertex3f(Point[0].x, Point[0].y, Point[0].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);

			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);
		}
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawSphere(Sphere sphere, int nMeridians, int nParallels, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonSphere(sphere, nMeridians, nParallels, polygonColor);
	if (drawWireframe) MyDrawWireframeSphere(sphere, nMeridians, nParallels, wireframeColor);
}

/*----------------------------------------------------------------------------------------------------------------------------
													CYLINDER PORTION
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, Color color) {
	if (rlCheckBufferLimit(nSectors * 6)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(cylinder.radius, cylinder.halfHeight, cylinder.radius);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	float deltaTheta = (endTheta - startTheta) / nSectors;

	//On calcule la taille des quads
	float dist = sqrt((1 - cos(deltaTheta)) / 2);

	//On calcule le rayon nécessaire pour afficher les quads
	float r = (float)sqrt(1 - (dist * dist));

	for (int i = 0; i < nSectors; i++)
	{
		//On créer les quads
		ReferenceFrame ref = ReferenceFrame(
			CylindricalToCartesian({ r, (deltaTheta * i) + deltaTheta / 2 + startTheta, 0 }),
			QuaternionFromEuler((deltaTheta * i) + deltaTheta / 2 + startTheta, 0, -PI / 2)
		);
		//On affiche les quads
		MyDrawPolygonQuad({ ref,{1,0,dist} }, color);
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, Color color) {
	if (rlCheckBufferLimit(nSectors * 6)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(cylinder.radius, cylinder.halfHeight, cylinder.radius);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);

	float deltaTheta = (endTheta - startTheta) / nSectors;

	//On calcule la taille des quads
	float dist = sqrt((1 - cos(deltaTheta)) / 2);

	//On calcule le rayon nécessaire pour afficher les quads
	float r = (float)sqrt(1 - (dist * dist));

	for (int i = 0; i < nSectors; i++)
	{
		//On créer les quads
		ReferenceFrame ref = ReferenceFrame(
			CylindricalToCartesian({ r, (deltaTheta * i) + deltaTheta / 2 + startTheta, 0 }),
			QuaternionFromEuler((deltaTheta * i) + deltaTheta / 2 + startTheta, 0, -PI / 2)
		);
		//On affiche les quads
		MyDrawWireframeQuad({ ref,{1,0,dist} }, color);
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonCylinderPortion(cylinder, nSectors, startTheta, endTheta, polygonColor);
	if (drawWireframe) MyDrawWireframeCylinderPortion(cylinder, nSectors, startTheta, endTheta, wireframeColor);
}

/*----------------------------------------------------------------------------------------------------------------------------
													CYLINDER
----------------------------------------------------------------------------------------------------------------------------*/

void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps, Color color) {

	if (drawCaps) {
		if (rlCheckBufferLimit(nSectors * 6 * 2)) rlglDraw();
	}
	else {
		if (rlCheckBufferLimit(nSectors * 6)) rlglDraw();
	}
	rlPushMatrix();
	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(1, 1, 1);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	ReferenceFrame ref = ReferenceFrame(
		{ 0,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));

	MyDrawPolygonCylinderPortion({ ref, cylinder.halfHeight, cylinder.radius }, nSectors, 0, 2 * PI);

	for (int i = 0; i < nSectors && drawCaps; i++)
	{
		//On calcule les différents points d'une faces avec CylindricalToCartesian
		Vector3 CartPoint1 = CylindricalToCartesian({ cylinder.radius, (i * (2 * PI)) / nSectors, -cylinder.halfHeight });
		Vector3 CartPoint2 = CylindricalToCartesian({ cylinder.radius, ((i + 1) * (2 * PI)) / nSectors, -cylinder.halfHeight });
		Vector3 CartPoint3 = CylindricalToCartesian({ cylinder.radius, (i * (2 * PI)) / nSectors, cylinder.halfHeight });
		Vector3 CartPoint4 = CylindricalToCartesian({ cylinder.radius, ((i + 1) * (2 * PI)) / nSectors, cylinder.halfHeight });

		rlVertex3f(0, -cylinder.halfHeight, 0);
		rlVertex3f(CartPoint1.x, CartPoint1.y, CartPoint1.z);
		rlVertex3f(CartPoint2.x, CartPoint2.y, CartPoint2.z);

		rlVertex3f(0, cylinder.halfHeight, 0);
		rlVertex3f(CartPoint4.x, CartPoint4.y, CartPoint4.z);
		rlVertex3f(CartPoint3.x, CartPoint3.y, CartPoint3.z);

	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps, Color color) {
	float n = 0;
	if (drawCaps)n = 4;
	if (rlCheckBufferLimit(nSectors * 8 + n)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(cylinder.radius, cylinder.halfHeight, cylinder.radius);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);

	for (int i = 0; i < nSectors; i++)
	{
		Vector3 CartPoint1 = CylindricalToCartesian({ 1, (i * (2 * PI)) / nSectors, -1 });
		Vector3 CartPoint2 = CylindricalToCartesian({ 1, ((i + 1) * (2 * PI)) / nSectors, -1 });
		Vector3 CartPoint3 = CylindricalToCartesian({ 1, (i * (2 * PI)) / nSectors, 1 });
		Vector3 CartPoint4 = CylindricalToCartesian({ 1, ((i + 1) * (2 * PI)) / nSectors, 1 });

		if (drawCaps) {
			rlVertex3f(0, -1, 0);
			rlVertex3f(CartPoint1.x, CartPoint1.y, CartPoint1.z);

			rlVertex3f(0, 1, 0);
			rlVertex3f(CartPoint3.x, CartPoint3.y, CartPoint3.z);
		}

		rlVertex3f(CartPoint2.x, CartPoint2.y, CartPoint2.z);
		rlVertex3f(CartPoint1.x, CartPoint1.y, CartPoint1.z);

		rlVertex3f(CartPoint1.x, CartPoint1.y, CartPoint1.z);
		rlVertex3f(CartPoint3.x, CartPoint3.y, CartPoint3.z);

		rlVertex3f(CartPoint3.x, CartPoint3.y, CartPoint3.z);
		rlVertex3f(CartPoint4.x, CartPoint4.y, CartPoint4.z);

		rlVertex3f(CartPoint1.x, CartPoint1.y, CartPoint1.z);
		rlVertex3f(CartPoint4.x, CartPoint4.y, CartPoint4.z);
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawCylinder(Cylinder cylinder, int nSectors, bool drawPolygon, bool drawWireframe) {
	if (drawPolygon) MyDrawPolygonCylinder(cylinder, nSectors);
	if (drawWireframe) MyDrawWireframeCylinder(cylinder, nSectors);
}

/*----------------------------------------------------------------------------------------------------------------------------
													CAPSULE
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonCapsule(Capsule capsule, int nSectors, int nParallels, Color color) {
	MyDrawPolygonCylinder({ capsule.ref, capsule.halfHeight, capsule.radius }, nSectors, false, color);
	if (rlCheckBufferLimit(nSectors * nParallels * 6)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(capsule.ref.origin.x, capsule.ref.origin.y, capsule.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(capsule.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);


	//On Draw deux demi sphere
	ReferenceFrame refUp = ReferenceFrame(
		{ 0,capsule.halfHeight,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));

	MyDrawPolygonSpherePortion({ refUp, capsule.radius}, nSectors, nParallels, 0, 2 * PI, 0, PI / 2, color);
	
	ReferenceFrame refDown = ReferenceFrame(
		{ 0,-capsule.halfHeight,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));

	MyDrawPolygonSpherePortion({ refDown, capsule.radius }, nSectors, nParallels, 0 ,2 * PI, 3 * PI / 2, PI, color);

	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeCapsule(Capsule capsule, int nSectors, int nParallels, Color color) {
	MyDrawWireframeCylinder({ capsule.ref, capsule.halfHeight, capsule.radius }, nSectors, false, color);
	if (rlCheckBufferLimit(nSectors * nParallels * 6)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(capsule.ref.origin.x, capsule.ref.origin.y, capsule.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(capsule.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);

	//On Draw deux demi sphere
	ReferenceFrame refUp = ReferenceFrame(
		{ 0,capsule.halfHeight,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));

	MyDrawWireframeSpherePortion({ refUp, capsule.radius }, nSectors, nParallels, 0, 2 * PI, 0, PI / 2, color);

	ReferenceFrame refDown = ReferenceFrame(
		{ 0,-capsule.halfHeight,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));

	MyDrawWireframeSpherePortion({ refDown, capsule.radius }, nSectors, nParallels, 0, 2 * PI, 3 * PI / 2, PI, color);
	rlEnd();
	rlPopMatrix();
}

void MyDrawCapsule(Capsule capsule, int nSectors, int nParallels, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonCapsule(capsule, nSectors, nParallels, polygonColor);
	if (drawWireframe) MyDrawWireframeCapsule(capsule, nSectors, nParallels, wireframeColor);
}

/*----------------------------------------------------------------------------------------------------------------------------
													SPHERE PORTION
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, Color color) {
	float qTheta = (2 * PI) / (endTheta - startTheta);
	float qPhi = PI / (endPhi - startPhi);

	if (rlCheckBufferLimit(nMeridians * nParallels * 6 / qTheta / qPhi)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(sphere.radius, sphere.radius, sphere.radius);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	for (int i = 0; i < nParallels; i++)
	{
		for (int j = 0; j < nMeridians; j++) {
			//On calcules les points de la face avec SphericalToCartesian en fonction des Theta et Phi
			Vector3 Point[] = {
				SphericalToCartesian({1, startTheta + j * ((endTheta - startTheta) / nMeridians), startPhi + i * ((endPhi - startPhi) / nParallels)}),
				SphericalToCartesian({1, startTheta + (j + 1) * ((endTheta - startTheta) / nMeridians),startPhi + i * ((endPhi - startPhi) / nParallels)}),
				SphericalToCartesian({1, startTheta + j * ((endTheta - startTheta) / nMeridians),startPhi + (i + 1) * ((endPhi - startPhi) / nParallels)}),
				SphericalToCartesian({1, startTheta + (j + 1) * ((endTheta - startTheta) / nMeridians),startPhi + (i + 1) * ((endPhi - startPhi) / nParallels)})
			};
			rlVertex3f(Point[0].x, Point[0].y, Point[0].z);
			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);

			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);
			rlVertex3f(Point[3].x, Point[3].y, Point[3].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);
		}
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, Color color) {
	float qTheta = (2 * PI) / (endTheta - startTheta);
	float qPhi = PI / (endPhi - startPhi);
	if (rlCheckBufferLimit(nMeridians * nParallels * 6)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(sphere.radius, sphere.radius, sphere.radius);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);
	for (int i = 0; i < nParallels; i++)
	{
		for (int j = 0; j < nMeridians; j++) {
			//On calcules les points de la face avec SphericalToCartesian en fonction des Theta et Phi
			Vector3 Point[] = {
				SphericalToCartesian({1, startTheta + j * ((endTheta - startTheta) / nMeridians), startPhi + i * ((endPhi - startPhi) / nParallels)}),
				SphericalToCartesian({1, startTheta + (j + 1) * ((endTheta - startTheta) / nMeridians),startPhi + i * ((endPhi - startPhi) / nParallels)}),
				SphericalToCartesian({1, startTheta + j * ((endTheta - startTheta) / nMeridians),startPhi + (i + 1) * ((endPhi - startPhi) / nParallels)}),
				SphericalToCartesian({1, startTheta + (j + 1) * ((endTheta - startTheta) / nMeridians),startPhi + (i + 1) * ((endPhi - startPhi) / nParallels)})
			};
			rlVertex3f(Point[0].x, Point[0].y, Point[0].z);
			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);

			rlVertex3f(Point[0].x, Point[0].y, Point[0].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);

			rlVertex3f(Point[1].x, Point[1].y, Point[1].z);
			rlVertex3f(Point[2].x, Point[2].y, Point[2].z);
		}
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonSpherePortion(sphere, nMeridians, nParallels, startTheta, endTheta, startPhi, endPhi, polygonColor);
	if (drawWireframe) MyDrawWireframeSpherePortion(sphere, nMeridians, nParallels, startTheta, endTheta, startPhi, endPhi, wireframeColor);
}

/*----------------------------------------------------------------------------------------------------------------------------
													ROUNDED BOX
----------------------------------------------------------------------------------------------------------------------------*/
void MyDrawPolygonRoundedBox(RoundedBox roundedBox, int nSectors, Color color) {
	int numVertex = 6 * 6; // Nombre de points des quads
	numVertex += 12 * nSectors * 6; //  Nombre de points des cylindres
	numVertex += 8 * 6 * nSectors * nSectors; //  Nombre de points des spheres
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(roundedBox.ref.origin.x, roundedBox.ref.origin.y, roundedBox.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(roundedBox.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	float radius = roundedBox.radius;
	//Draw quads
#pragma region Quads
	//Top
	ReferenceFrame ref = ReferenceFrame(
		{ 0,roundedBox.extents.y + radius,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	Quad quad = Quad{ ref, {roundedBox.extents.x,0,roundedBox.extents.z} };
	MyDrawPolygonQuad(quad, color);

	//Forward
	ref = ReferenceFrame({ 0,0,roundedBox.extents.z + radius },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
	quad = { ref ,{roundedBox.extents.x,0,roundedBox.extents.y} };
	MyDrawPolygonQuad(quad, color);
	//Bottom
	ref = ReferenceFrame(
		{ 0,-roundedBox.extents.y - radius,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI));
	quad = { ref ,{roundedBox.extents.x,0,roundedBox.extents.z} };
	MyDrawPolygonQuad(quad, color);
	//BackWard
	ref = ReferenceFrame(
		{ 0,0,-roundedBox.extents.z - radius },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), (3 * PI) / 2));
	quad = { ref ,{roundedBox.extents.x,0,roundedBox.extents.y} };
	MyDrawPolygonQuad(quad, color);
	//Right
	ref = ReferenceFrame(
		{ roundedBox.extents.x + radius,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), (3 * PI) / 2));
	quad = { ref ,{roundedBox.extents.y,0,roundedBox.extents.z} };
	MyDrawPolygonQuad(quad, color);
	//Left
	ref = ReferenceFrame(
		{ -roundedBox.extents.x - radius,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
	quad = { ref ,{roundedBox.extents.y,0,roundedBox.extents.z} };
	MyDrawPolygonQuad(quad, color);
#pragma endregion

	//Draw the cylinder portions
#pragma region Cylindres
	// Bottom + BackWard
	ref = ReferenceFrame(
		{ -roundedBox.extents.x,-roundedBox.extents.y,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,1,1 }), PI));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 3 * PI / 2, 2 * PI, color);
	// Top + BackWard
	ref = ReferenceFrame(
		{ -roundedBox.extents.x,roundedBox.extents.y,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,1,1 }), PI));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 0, PI / 2, color);
	//Bottom + Right
	ref = ReferenceFrame(
		{ 0,-roundedBox.extents.y,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,1,0 }), PI));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, PI / 2, PI, color);
	//Top + Right
	ref = ReferenceFrame(
		{ 0,roundedBox.extents.y,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,1,0 }), PI));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, 0, PI / 2, color);
	//Bottom + Forward
	ref = ReferenceFrame(
		{ roundedBox.extents.x,-roundedBox.extents.y,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 0, PI / 2, color);
	//Top + Forward
	ref = ReferenceFrame(
		{ roundedBox.extents.x,roundedBox.extents.y,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), 3 * PI / 2));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 0, PI / 2, color);
	//Bottom + Left
	ref = ReferenceFrame(
		{ 0,-roundedBox.extents.y,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), 3 * PI / 2));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, 0, PI / 2, color);
	//Top + Left
	ref = ReferenceFrame(
		{ 0,roundedBox.extents.y,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, 0, PI / 2, color);
	//Forward + Left
	ref = ReferenceFrame(
		{ roundedBox.extents.x,0,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
	//Forward + Right
	ref = ReferenceFrame(
		{ roundedBox.extents.x,0,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,1,0 }), PI / 2));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
	//Backward + Right
	ref = ReferenceFrame(
		{ -roundedBox.extents.x,0,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,1,0 }), PI));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
	//Backward + Left
	ref = ReferenceFrame(
		{ -roundedBox.extents.x,0,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,1,0 }), (3 * PI) / 2));
	MyDrawPolygonCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
#pragma endregion

	//Draw the Angles
#pragma region Angles
	ref = ReferenceFrame(
		{ roundedBox.extents.x,roundedBox.extents.y,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, 0, PI / 2, 0, PI / 2, color);
	ref = ReferenceFrame(
		{ roundedBox.extents.x,-roundedBox.extents.y,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, 0, PI / 2, PI / 2, PI, color);

	ref = ReferenceFrame(
		{ -roundedBox.extents.x,roundedBox.extents.y,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, PI / 2, PI, 0, PI / 2, color);
	ref = ReferenceFrame(
		{ -roundedBox.extents.x,-roundedBox.extents.y,roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, PI / 2, PI, PI / 2, PI, color);

	ref = ReferenceFrame(
		{ -roundedBox.extents.x,roundedBox.extents.y,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, PI, 3 * PI / 2, 0, PI / 2, color);
	ref = ReferenceFrame(
		{ -roundedBox.extents.x,-roundedBox.extents.y,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, PI, 3 * PI / 2, PI / 2, PI, color);

	ref = ReferenceFrame(
		{ roundedBox.extents.x,roundedBox.extents.y,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, 3 * PI / 2, 2 * PI, 0, PI / 2, color);
	ref = ReferenceFrame(
		{ roundedBox.extents.x,-roundedBox.extents.y,-roundedBox.extents.z },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	MyDrawPolygonSpherePortion({ ref,radius }, nSectors, nSectors, 3 * PI / 2, 2 * PI, PI / 2, PI, color);
#pragma endregion
	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeRoundedBox(RoundedBox roundedBox, int nSectors, Color color, bool light) {
	int numVertex = 6 * 10; // Nombre de points des quads
	numVertex += 12 * nSectors * 8; //  Nombre de points des cylindres
	numVertex += 8 * ((6 * nSectors * nSectors) + (2 * nSectors) + (2 * nSectors)); //  Nombre de points des spheres
	rlPushMatrix();
	rlTranslatef(roundedBox.ref.origin.x, roundedBox.ref.origin.y, roundedBox.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(roundedBox.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlBegin(RL_LINES); // On utilise RL_LINES pour le WireFrame
	rlColor4ub(color.r, color.g, color.b, color.a);
	float radius = roundedBox.radius;
	//Draw quads
#pragma region Quads
	//Top
	ReferenceFrame ref = ReferenceFrame(
		{ 0,roundedBox.extents.y + radius,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
	Quad quad = Quad{ ref, {roundedBox.extents.x,0,roundedBox.extents.z} };
	MyDrawWireframeQuad(quad, color);

	//Forward
	ref = ReferenceFrame({ 0,0,roundedBox.extents.z + radius },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
	quad = { ref ,{roundedBox.extents.x,0,roundedBox.extents.y} };
	MyDrawWireframeQuad(quad, color);
	//Bottom
	ref = ReferenceFrame(
		{ 0,-roundedBox.extents.y - radius,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI));
	quad = { ref ,{roundedBox.extents.x,0,roundedBox.extents.z} };
	MyDrawWireframeQuad(quad, color);
	//BackWard
	ref = ReferenceFrame(
		{ 0,0,-roundedBox.extents.z - radius },
		QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), (3 * PI) / 2));
	quad = { ref ,{roundedBox.extents.x,0,roundedBox.extents.y} };
	MyDrawWireframeQuad(quad, color);
	//Right
	ref = ReferenceFrame(
		{ roundedBox.extents.x + radius,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), (3 * PI) / 2));
	quad = { ref ,{roundedBox.extents.y,0,roundedBox.extents.z} };
	MyDrawWireframeQuad(quad, color);
	//Left
	ref = ReferenceFrame(
		{ -roundedBox.extents.x - radius,0,0 },
		QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
	quad = { ref ,{roundedBox.extents.y,0,roundedBox.extents.z} };
	MyDrawWireframeQuad(quad, color);
#pragma endregion

	if (!light) {
		//Draw the cylinder portions
#pragma region Cylindres
// Bottom + BackWard
		ref = ReferenceFrame(
			{ -roundedBox.extents.x,-roundedBox.extents.y,0 },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,1,1 }), PI));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 3 * PI / 2, 2 * PI, color);
		// Top + BackWard
		ref = ReferenceFrame(
			{ -roundedBox.extents.x,roundedBox.extents.y,0 },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,1,1 }), PI));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 0, PI / 2, color);
		//Bottom + Right
		ref = ReferenceFrame(
			{ 0,-roundedBox.extents.y,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 1,1,0 }), PI));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, PI / 2, PI, color);
		//Top + Right
		ref = ReferenceFrame(
			{ 0,roundedBox.extents.y,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 1,1,0 }), PI));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, 0, PI / 2, color);
		//Bottom + Forward
		ref = ReferenceFrame(
			{ roundedBox.extents.x,-roundedBox.extents.y,0 },
			QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 0, PI / 2, color);
		//Top + Forward
		ref = ReferenceFrame(
			{ roundedBox.extents.x,roundedBox.extents.y,0 },
			QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), 3 * PI / 2));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.z,radius }, nSectors, 0, PI / 2, color);
		//Bottom + Left
		ref = ReferenceFrame(
			{ 0,-roundedBox.extents.y,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), 3 * PI / 2));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, 0, PI / 2, color);
		//Top + Left
		ref = ReferenceFrame(
			{ 0,roundedBox.extents.y,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.x,radius }, nSectors, 0, PI / 2, color);
		//Forward + Left
		ref = ReferenceFrame(
			{ roundedBox.extents.x,0,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
		//Forward + Right
		ref = ReferenceFrame(
			{ roundedBox.extents.x,0,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,1,0 }), PI / 2));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
		//Backward + Right
		ref = ReferenceFrame(
			{ -roundedBox.extents.x,0,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,1,0 }), PI));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
		//Backward + Left
		ref = ReferenceFrame(
			{ -roundedBox.extents.x,0,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,1,0 }), (3 * PI) / 2));
		MyDrawWireframeCylinderPortion({ ref,roundedBox.extents.y,radius }, nSectors, 0, PI / 2, color);
#pragma endregion

		//Draw the Angles
#pragma region Angles
		ref = ReferenceFrame(
			{ roundedBox.extents.x,roundedBox.extents.y,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, 0, PI / 2, 0, PI / 2, color);
		ref = ReferenceFrame(
			{ roundedBox.extents.x,-roundedBox.extents.y,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, 0, PI / 2, PI / 2, PI, color);

		ref = ReferenceFrame(
			{ -roundedBox.extents.x,roundedBox.extents.y,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, PI / 2, PI, 0, PI / 2, color);
		ref = ReferenceFrame(
			{ -roundedBox.extents.x,-roundedBox.extents.y,roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, PI / 2, PI, PI / 2, PI, color);

		ref = ReferenceFrame(
			{ -roundedBox.extents.x,roundedBox.extents.y,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, PI, 3 * PI / 2, 0, PI / 2, color);
		ref = ReferenceFrame(
			{ -roundedBox.extents.x,-roundedBox.extents.y,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, PI, 3 * PI / 2, PI / 2, PI, color);

		ref = ReferenceFrame(
			{ roundedBox.extents.x,roundedBox.extents.y,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, 3 * PI / 2, 2 * PI, 0, PI / 2, color);
		ref = ReferenceFrame(
			{ roundedBox.extents.x,-roundedBox.extents.y,-roundedBox.extents.z },
			QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
		MyDrawWireframeSpherePortion({ ref,radius }, nSectors, nSectors, 3 * PI / 2, 2 * PI, PI / 2, PI, color);
#pragma endregion
	}
	rlEnd();
	rlPopMatrix();
}

void MyDrawRoundedBox(RoundedBox roundedBox, int nSectors, bool light, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonRoundedBox(roundedBox, nSectors, polygonColor);
	if (drawWireframe) MyDrawWireframeRoundedBox(roundedBox, nSectors, wireframeColor, light);
}