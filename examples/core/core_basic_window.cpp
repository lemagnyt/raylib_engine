/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <raymath.h>
#include "rlgl.h"
#include <math.h>
#include <float.h>
#include <vector>
#include "CoordinateSystem.h"
#include "My3DPrimitives.h"
#include "MyDraw3DPrimitives.h"
#include "Intersection3D.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define EPSILON 1.e-6f

#define REAL false

#define TEST 0
// TEST = 0 : test si la balle rebondit sur une box ou rounded box
// TEST > 0 : test l'intersection entre un segement et une structure
// TEST = 1 : test l'intersection entre un segment et un cylindre
// TEST = 2 : test l'intersection entre un segment et une box
// TEST = 3 : test l'intersection entre un segment et un quad
// TEST = 4 : test l'intersection entre un segment et un infinite cylinder
// TEST = 5 : test l'intersection entre un segment et une capsule
// TEST = 6 : test l'intersection entre un segment et une rounded box

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

/*----------------------------------------------------------------------------------------------------------------------------
													CAMERA
----------------------------------------------------------------------------------------------------------------------------*/

void MyUpdateOrbitalCamera(Camera* camera, float deltaTime, Vector2* prevMousePos)
{
	//differente constant
	const Spherical sphSpeed = { 20.0f,0.15f,0.15f };

	const float rhoMin = 4;
	const float rhoMax = 40;

	const float phiMin = PI/8;
	const float phiMax = 7*PI/8;

	Vector2 MousePosition = GetMousePosition(); //On récupere la position de la souris

	Spherical NewCamPos = CartesianToSpherical(camera->position); //On convertie la position de la camera en spherical pour faire les calculs

	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {//Si on a le click droit d'enfoncer on modifie le rho et theta de la camera en fonction du mouvement de la souris
		NewCamPos = SphericalAdd( //NewCamPos = NewCamPos + (deltaSouris * deltaTime) * sphSpeed
			NewCamPos,
			SphericalMultiplies(
				SphericalMultVector2(
					Spherical{ 0, deltaTime, deltaTime },
					Vector2Subtract(MousePosition, *prevMousePos)
				),
				sphSpeed
			)
		);
	}

	//On zoom en fonction du scroll de la souris
	NewCamPos = SphericalAdd( //NewCamPos = NewCamPos + Scroll * deltaTime * sphSpeed
		NewCamPos,
		Spherical{ GetMouseWheelMove() * deltaTime * sphSpeed.rho, 0, 0 }
	);

	NewCamPos.rho = Clamp(NewCamPos.rho, rhoMin, rhoMax); //On empéche la valeur rho de dépassé rhoMin et rhoMax
	NewCamPos.phi = Clamp(NewCamPos.phi, phiMin, phiMax); //On empéche la valeur phi de dépassé phiMin et phiMax

	camera->position = SphericalToCartesian(NewCamPos);//On mets à jour la position de la camera

	*prevMousePos = MousePosition; //Mise a jour de la position précedents
}

/*----------------------------------------------------------------------------------------------------------------------------
													MAIN
----------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	float screenSizeCoef = .9f;
	const int screenWidth = 1920 * screenSizeCoef;
	const int screenHeight = 1080 * screenSizeCoef;

	InitWindow(screenWidth, screenHeight, "ESIEE - E3FI - 2022 - 2023 -Maths 3D");

	SetTargetFPS(60);

	//CAMERA
	static Spherical cameraPos = { 30, PI / 4.0f, PI / 4.0f };
	Vector3 pos = SphericalToCartesian(cameraPos);
	Camera camera = { 0 };
	camera.position = pos;
	camera.target = { 0.0f, 0.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 90.0f;
	camera.type = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_CUSTOM);  // Set an orbital camera mode


	//--------------------------------------------------------------------------------------
	ReferenceFrame ref = { { 0,0,0 }, QuaternionFromAxisAngle({0, 0, 0}, 0) };

	ReferenceFrame refSphere = { {0,20,0},QuaternionFromAxisAngle({0,0,0},0) };
	const ReferenceFrame refRndBox1 = { {0 , 0 , 0}, QuaternionFromEuler(0,0,0) };
	RoundedBox rndBox1 = { refRndBox1, {20,1,20},1,YELLOW };
	const ReferenceFrame refRndBox2 = { {0 , 10 , 20}, QuaternionFromEuler(0,0,0) };
	RoundedBox rndBox2 = { refRndBox2, {20,10,1},1,LIGHTGRAY };
	const ReferenceFrame refRndBox3 = { {0 , 10 , -20}, QuaternionFromEuler(0,0,0) };
	RoundedBox rndBox3 = { refRndBox3, {20,10,1},1 ,LIGHTGRAY };
	const ReferenceFrame refRndBox4 = { {20 , 10 , 0}, QuaternionFromEuler(0,0,0) };
	RoundedBox rndBox4 = { refRndBox4, {1,10,20},1 };
	const ReferenceFrame refRndBox5 = { {-20 , 10 , 0}, QuaternionFromEuler(0,0,0) };
	RoundedBox rndBox5 = { refRndBox5, {1,10,20},1 };


	Vector3 velocity = { 0,0,0 };
	const Vector3 g = { 0,-9.81,0 };
	const float radius = 1.25;
	const float y0 = 0;
	const float m = 1;
	float E = m * Vector3Length(g) * (refSphere.origin.y - y0);
	float I = 2.f / 5.f * m * pow(radius, 2);
	float VAngular = 2;
	Vector3 LG = { 0,0,1 };
	const Color colors[11] = {RED, BEIGE, BLUE, GREEN, BROWN, ORANGE, DARKBLUE, PURPLE, DARKPURPLE, WHITE, PINK};
	std::vector<RoundedBox> rndBoxes = { rndBox1,rndBox2,rndBox3,rndBox4,rndBox5};
	for (int i = 0; i < 20; i++)
	{
		bool wellPlaced = false;
		Color color = colors[int(GetRandomValue(0, 11))];
		float lx = GetRandomValue(1.0f, 2.f);
		float ly = GetRandomValue(1.0f, 2.f);
		float lz = GetRandomValue(1.0f, 2.f);
		float randomRadius = GetRandomValue(0.5f, 1.5f);
		float lmax = lx;
		if (lmax < ly)lmax = ly;
		if (lmax < lz)lmax = lz;
		lmax += randomRadius;
		float qx = GetRandomValue(0.f, 2.f * PI);
		float qy = GetRandomValue(0.f, 2.f * PI);
		float qz = GetRandomValue(0.f, 2.f * PI);
		float x;
		float y;
		float z;
		while (!wellPlaced) {
			x = GetRandomValue(-17.f + lx, 17.f - lx);
			z = GetRandomValue(-17.f + lz, 17.f - lz);
			y = GetRandomValue(2.f + ly, 15.f - ly);

			wellPlaced = true;
			for (int i = 5; i < rndBoxes.size(); i++) {
				RoundedBox rndBoxI = rndBoxes[i];
				float lmax2 = rndBoxI.extents.x;
				if (lmax2 < y)lmax2 = rndBoxI.extents.y;
				if (lmax2 < z)lmax2 = rndBoxI.extents.z;
				lmax2 += rndBox2.radius;
				//printf("lmax : %f - lmax2 : %f\n", lmax, lmax2);
				if (Vector3Distance(rndBoxI.ref.origin, { x,y,z }) < (lmax + lmax2+1))wellPlaced = false;
			}
		}


		
		ReferenceFrame refRndBox = {{x,y,z},QuaternionFromEuler(qx,qy,qz) };
		RoundedBox rndBox = { refRndBox,{lx,ly,lz},randomRadius,color };
		rndBoxes.push_back(rndBox);
	}

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		float deltaTime = GetFrameTime();
		float time = (float)GetTime();
		static Vector2 prevMousePos = GetMousePosition();

		MyUpdateOrbitalCamera(&camera, deltaTime, &prevMousePos);

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);
		{
			//

			//3D REFERENTIAL
			DrawGrid(20, 1.0f);        // Draw a grid
			DrawLine3D({ 0 }, { 0,10,0 }, DARKGRAY);
			DrawSphere({ 10,0,0 }, .2f, RED);
			DrawSphere({ 0,10,0 }, .2f, GREEN);
			DrawSphere({ 0,0,10 }, .2f, BLUE);

#if TEST == 0
			float colT;
			Vector3 newVelocity;
			Vector3 newPosition;
			Vector3 colSpherePos;
			Vector3 colNormal;
			Vector3 newLg;
			Sphere sphere = { refSphere, radius};
			velocity  = Vector3Add(velocity,Vector3Scale(g,deltaTime));
			float scalarVelocity = 0;
			if(E> m * Vector3Length(g) * (refSphere.origin.y - y0)+EPSILON) {
				scalarVelocity = sqrt(2 * (E - m * Vector3Length(g) * (refSphere.origin.y - y0)) / m);
			}
			
			if(Vector3Length(velocity) > 0.5) velocity = Vector3Scale(Vector3Normalize(velocity), scalarVelocity);// On vérifie que la vitesse ne soit pas trop petite

			Color color = LIGHTGRAY;

			if (GetSphereNewPositionAndVelocityIfMultiCollidingWithRoundedBoxes(sphere, rndBoxes, velocity, I, LG, deltaTime, 20, newPosition, newVelocity, newLg))
			{
				velocity = newVelocity;
				refSphere.origin = newPosition;
				LG = newLg;
				VAngular = sqrtf(Vector3Length(LG) / I);

				#if REAL == true
				float NewE = E * 0.95;
				if (NewE > m * Vector3Length(g) * (refSphere.origin.y - y0)) E = NewE;
				#endif
			}
			else refSphere.origin = Vector3Add(refSphere.origin, Vector3Scale(velocity, deltaTime));
			
			if(VAngular>EPSILON) LG = Vector3Scale(Vector3Normalize(LG), I * pow(VAngular,2));
			refSphere.q = QuaternionMultiply(refSphere.q, QuaternionFromAxisAngle(LG, VAngular * deltaTime));
			MyDrawSphere(sphere, 10, 10);
			for (RoundedBox rndBox : rndBoxes) {
				MyDrawRoundedBox(rndBox, 10,true,true,true,rndBox.color);
			}
#endif

#if TEST > 0
			Vector3 interPt;
			Vector3 interNormal;
			float t;
			Segment segment = { {-5,8,3},{5,-8,3} };
			Plane plane = { Vector3RotateByQuaternion({0,0,1}, QuaternionFromAxisAngle({0,1,0},time * .5f)), 2 };
			ReferenceFrame refQuad = { Vector3Scale(plane.n, plane.d), QuaternionFromVector3ToVector3({0,1,0},plane.n) };
			DrawLine3D(segment.pt1, segment.pt2, BLACK);
			MyDrawPolygonSphere({ {segment.pt1,QuaternionIdentity()},.15f }, 16, 8, RED);
			MyDrawPolygonSphere({ {segment.pt2,QuaternionIdentity()},.15f }, 16, 8, GREEN);
#endif
#if TEST == 1
			Cylinder cyl = { refQuad, 4, 5 };
			MyDrawCylinder(cyl, 15);
			if (IntersectSegmentCylinder(segment, cyl, t, interPt, interNormal))
			{
				MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
				DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
			}
#elif TEST == 2
			Box box = { refQuad,{1,6,1}};
			ReferenceFrame ref1 = { LocalToGlobalVect({ 0,box.extents.y,0 }, box.ref),box.ref.q };
			MyDrawBox(box);
			if (IntersectSegmentBox(segment, box, t, interPt, interNormal))
			{
				MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
				DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
			}
#elif TEST == 3
			Quad quad = { refQuad,{5,5,5} };
			MyDrawQuad(quad);
			if (IntersectSegmentQuad(segment, quad, t, interPt, interNormal))
			{
				MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
				DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
			}
#elif TEST == 4
			InfiniteCylinder Infcyl = { refQuad,1 };
			Cylinder cyl = { refQuad,5,1 };
			MyDrawCylinder(cyl, 20, 20);
			if (IntersectSegmentInfiniteCylinder(segment, Infcyl, t, interPt, interNormal))
			{
				MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
				DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
			}
#elif TEST == 5
			Capsule capsule = { refQuad,4.5,1 };
			MyDrawCapsule(capsule, 20, 20);
			if (IntersectSegmentCapsule(segment, capsule, t, interPt, interNormal))
			{
				MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
				DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
			}
#elif TEST == 6
			RoundedBox rndBox = { refRndBox1,{5,1,5},1 };
			MyDrawRoundedBox(rndBox, 10);
			if (IntersectSegmentRoundedBox(segment, rndBox, t, interPt, interNormal))
			{
				MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
				DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
			}

#endif

			
		}
		EndMode3D();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}