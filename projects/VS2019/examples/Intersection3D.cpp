#include "Intersection3D.h"

/*----------------------------------------------------------------------------------------------------------------------------
												 REFERENCIEL
----------------------------------------------------------------------------------------------------------------------------*/

Vector3 LocalToGlobalVect(Vector3 localVect, ReferenceFrame localRef) {
	Vector3 NewVector = Vector3RotateByQuaternion(localVect, localRef.q); // Changement quaternion
	return Vector3Add(NewVector, localRef.origin); // Changement origine
}

Vector3 GlobalToLocalVect(Vector3 globalVect, ReferenceFrame localRef) {
	Vector3 NewVector = Vector3Subtract(globalVect, localRef.origin); // Changement origine
	return Vector3RotateByQuaternion(NewVector, QuaternionInvert(localRef.q)); // Changement quaternion
}


/*----------------------------------------------------------------------------------------------------------------------------
													INTERSECTION
----------------------------------------------------------------------------------------------------------------------------*/
bool IntersectLinePlane(Line line, Plane plane, float& t, Vector3& interPt, Vector3& interNormal)
{
	// no intersection if line is parallel to the plane
	float dotProd = Vector3DotProduct(plane.n, line.dir);
	if (fabsf(dotProd) < 0.1f) return false;
	// intersection: t, interPt & interNormal
	t = (plane.d - Vector3DotProduct(plane.n, line.pt)) / dotProd;
	interPt = Vector3Add(line.pt, Vector3Scale(line.dir, t)); // OM = OA+tAB
	interNormal = Vector3Scale(plane.n, Vector3DotProduct(Vector3Subtract(line.pt, interPt), plane.n) < 0 ? -1.f : 1.f);
	return true;
}

bool IntersectSegmentPlane(Segment seg, Plane plane, float& t, Vector3& interPt, Vector3& interNormal) {
	Line line = { seg.pt1,Vector3Subtract(seg.pt2,seg.pt1) }; // Conversion segment en ligne
	if (!IntersectLinePlane(line, plane, t, interPt, interNormal)) // pas d'intersection avec la ligne implique pas d'intersection avec le segment
		return false;
	return (t >= 0 && t <= 1); //Si intersection avec ligne il faut qu'elle soit dans le segment donc on vérifie le t
}

bool IntersectSegmentQuad(Segment seg, Quad quad, float& t, Vector3& interPt, Vector3& interNormal) {
	Vector3 p1 = GlobalToLocalVect(seg.pt1, quad.ref); // On passe les coordonnées de nos 2 points de segment en référentiel du quad
	Vector3 p2 = GlobalToLocalVect(seg.pt2, quad.ref);
	//Pas d'intersection si les deux points sont du même coté du Quad
	if (p1.y * p2.y > 0) return false;
	t = (-p1.y) / (p2.y - p1.y);
	interPt = Vector3Add(p1, Vector3Scale(Vector3Subtract(p2, p1), t)); //OM = OA+tAB
	float n;
	if (Vector3DotProduct(Vector3Subtract(p2, p1), { 0,1,0 }) > 0) n = -1.f;
	else n = 1.f;
	interNormal = Vector3RotateByQuaternion({ 0,n,0 }, quad.ref.q);
	bool insideX = (interPt.x < quad.extents.x&& interPt.x > -quad.extents.x); // Vérifie que la position x du point de collision est dans le bon intervalle
	bool insideZ = (interPt.z < quad.extents.z&& interPt.z > -quad.extents.z); // Vérifie que la position z du point de collision est dans le bon intervalle
	interPt = LocalToGlobalVect(interPt, quad.ref); // On passe le point de collision en référentiel local
	return (insideX && insideZ);
}

bool IntersectSegmentDisk(Segment seg, Disk disk, float& t, Vector3& interPt, Vector3& interNormal) {
	Vector3 p1 = GlobalToLocalVect(seg.pt1, disk.ref); // On passe les coordonnées de nos 2 points de segment en référentiel du disque
	Vector3 p2 = GlobalToLocalVect(seg.pt2, disk.ref);
	//Pas d'intersection si les deux points sont du même coté du Disque
	if (p1.y * p2.y > 0) return false;
	t = (-p1.y) / (p2.y - p1.y);
	interPt = Vector3Add(p1, Vector3Scale(Vector3Subtract(p2, p1), t)); //OM = OA+tAB
	float n;
	if (Vector3DotProduct(Vector3Subtract(p2, p1), { 0,1,0 }) > 0) n = -1.f;
	else n = 1.f;
	interNormal = Vector3RotateByQuaternion({ 0,n,0 }, disk.ref.q);
	bool collision = Vector2Distance({ interPt.x, interPt.z }, { 0,0 }) < disk.radius; // Vérifie que le point de collision est à l'intérieur du disque
	interPt = LocalToGlobalVect(interPt, disk.ref); // On passe le point de collision en référentiel local
	return collision;
}

bool IntersectSegmentSphere(Segment seg, Sphere s, float& t, Vector3& interPt, Vector3& interNormal) {
	Vector3 p1 = GlobalToLocalVect(seg.pt1, s.ref); // On passe les coordonnées de nos 2 points de segment en référentiel de la sphère
	Vector3 p2 = GlobalToLocalVect(seg.pt2, s.ref);
	// On calcule pour quel point M on as 
	//OM = OA + T*AB
	//|OM|² = R²
	//Donc |OA + T * AB|² - R² = 0

	float xPrime = p2.x - p1.x;
	float yPrime = p2.y - p1.y;
	float zPrime = p2.z - p1.z;
	float a = pow(xPrime, 2) + pow(yPrime, 2) + pow(zPrime, 2);
	float b = 2 * ((p1.x * xPrime) + (p1.y * yPrime) + (p1.z * zPrime));
	float c = pow(p1.x, 2) + pow(p1.y, 2) + pow(p1.z, 2) - pow(s.radius, 2);
	float delta = pow(b, 2) - 4 * (a * c);
	//Si notre equation du second degré n'as pas de solution alors il n'y as pas d'intersection
	if (delta < 0) return false;
	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);
	//Si aucun des points d'intersection n'appartient a la ligne alors il n'y as pas d'intersection
	if (!(t1 >= 0 && t1 <= 1) && !(t2 >= 0 && t2 <= 1)) return false;
	//Sinon on retourne le point appartenant a la ligne le plus proche de a donc tmin
	if (t1 >= 0 && t1 <= 1) {
		if (t2 >= 0 && t2 <= 1) {
			t = t1;
			if (t2 < t1) t = t2;
		}
		else {
			t = t1;
		}
	}
	else {
		t = t2;
	}
	interPt = Vector3Add(p1, Vector3Scale(Vector3Subtract(p2, p1), t)); //OM = OA+tAB
	interNormal = Vector3RotateByQuaternion(Vector3Normalize(interPt), s.ref.q);
	interPt = LocalToGlobalVect(interPt, s.ref);
	return true;
}

bool IntersectSegmentInfiniteCylinder(Segment segment, InfiniteCylinder cyl, float& t, Vector3& interPt, Vector3& interNormal) {
	Vector3 p1 = GlobalToLocalVect(segment.pt1, cyl.ref);// On passe les coordonnées de nos 2 points de segment en référentiel de la sphère
	Vector3 p2 = GlobalToLocalVect(segment.pt2, cyl.ref);

	// On calcule pour quel point M on as 
	//OM = OA + T*AB
	//Mx²+Mz² = R²
	//Donc on doit résoudre une équation du second degré
	float xPrime = p2.x - p1.x;
	float zPrime = p2.z - p1.z;
	float a = pow(xPrime, 2) + pow(zPrime, 2);
	float b = 2 * ((p1.x * xPrime) + (p1.z * zPrime));
	float c = pow(p1.x, 2) + pow(p1.z, 2) - pow(cyl.radius, 2);
	float delta = pow(b, 2) - 4 * (a * c);
	//Si notre equation du second degré n'as pas de solution alors il n'y as pas d'intersection
	if (delta < 0) return false;
	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);
	//Si aucun des points d'intersection n'appartient a la ligne alors il n'y as pas d'intersection
	if (!(t1 >= 0 && t1 <= 1) && !(t2 >= 0 && t2 <= 1)) return false;
	//Sinon on retourne le point appartenant a la ligne le plus proche de a donc tmin
	if (t1 >= 0 && t1 <= 1) {
		if (t2 >= 0 && t2 <= 1) {
			t = t1;
			if (t2 < t1) t = t2;
		}
		else {
			t = t1;
		}
	}
	else {
		t = t2;
	}
	interPt = Vector3Add(p1, Vector3Scale(Vector3Subtract(p2, p1), t)); //OM = OA+tAB
	interNormal = Vector3RotateByQuaternion(Vector3Normalize({ interPt.x, 0, interPt.z }), cyl.ref.q);
	interPt = LocalToGlobalVect(interPt, cyl.ref);
	return true;
}

bool IntersectSegmentBox(Segment seg, Box box, float& t, Vector3& interPt, Vector3& interNormal) {

	// On crée les différents quads de notre box afin d'ensuite tester pour chacun la collision avec le segment

	ReferenceFrame ref1 = { LocalToGlobalVect({ 0,box.extents.y,0 }, box.ref),box.ref.q };
	Quad quad1 = { ref1, {box.extents.x,0,box.extents.z} };

	ReferenceFrame ref2 = { LocalToGlobalVect({ 0,0,box.extents.z },box.ref),box.ref.q };
	ref2.q = QuaternionMultiply(ref2.q, QuaternionFromEuler(PI / 2, 0, 0));
	Quad quad2 = { ref2 ,{box.extents.x,0,box.extents.y} };

	ReferenceFrame ref3 = { LocalToGlobalVect({ 0, -box.extents.y,0 },box.ref),box.ref.q };
	ref3.q = QuaternionMultiply(ref3.q, QuaternionFromEuler(0, 0, PI));
	Quad quad3 = { ref3 ,{box.extents.x,0,box.extents.z} };

	ReferenceFrame ref4 = { LocalToGlobalVect({ 0,0,-box.extents.z },box.ref),box.ref.q };
	ref4.q = QuaternionMultiply(ref4.q, QuaternionFromEuler(-PI / 2, 0, 0));
	Quad quad4 = { ref4 ,{box.extents.x,0,box.extents.y} };

	ReferenceFrame ref5 = { LocalToGlobalVect({ box.extents.x,0,0 },box.ref),box.ref.q };
	ref5.q = QuaternionMultiply(ref5.q, QuaternionFromEuler(0, 0, -PI / 2));
	Quad quad5 = { ref5 ,{box.extents.y,0,box.extents.z} };

	ReferenceFrame ref6 = { LocalToGlobalVect({ -box.extents.x,0,0 },box.ref),box.ref.q };
	ref6.q = QuaternionMultiply(ref6.q, QuaternionFromEuler(0, 0, PI / 2));
	Quad quad6 = { ref6 ,{box.extents.y,0,box.extents.z} };

	//l'indice minimum est initialisé à 0
	int iMin = 0;
	Quad quadTable[6] = { quad1,quad2,quad3,quad4,quad5,quad6 }; // tableau de nos quads
	float tTable[6] = { 2,2,2,2,2,2 }; // tableau des t pour chaque quad (t = 2>1 car  t<=1 si collision)

	for (int i = 0; i < 6; i++) { // Pour chaque quad on teste l'intersection, on prendra celui avec le plus petit t car c'est celui qui a le point d'intersection le plus proche du point de départ du segment
		if (IntersectSegmentQuad(seg, quadTable[i], t, interPt, interNormal)) {
			tTable[i] = t;
			if (t < tTable[iMin]) {
				iMin = i;
			}
		}
	}
	if (tTable[iMin] > 1) return false; // Si t>1 il n'y a donc pas eu de collision
	return IntersectSegmentQuad(seg, quadTable[iMin], t, interPt, interNormal); // Sinon on retourne la bonne intersection avec le plus petit t
}

bool OBB(Segment seg, Box box) {
	Vector3 p1 = GlobalToLocalVect(seg.pt1, box.ref); // On passe les coordonnées de nos 2 points de segment en référentiel de la box
	Vector3 p2 = GlobalToLocalVect(seg.pt2, box.ref);
	Vector3 p[2] = { p1,p2 };
	for (int i = 0; i < 2; i++) { //On teste d'abord si l'un de nos 2 points est à l'intérieur de la box
		bool isInX = (p[i].x <= box.extents.x && p[i].x >= -box.extents.x);
		bool isInY = (p[i].y <= box.extents.y && p[i].y >= -box.extents.y);
		bool isInZ = (p[i].z <= box.extents.z && p[i].z >= -box.extents.z);
		if (isInX && isInY && isInZ)return true; // Si oui on retourne true
	}
	float t;
	Vector3 interPt;
	Vector3 interNormal;
	return IntersectSegmentBox(seg, box, t, interPt, interNormal); // Sinon on teste l'intersection entre le segment et la box

}

bool IntersectSegmentCylinder(Segment segment, Cylinder cyl, float& t, Vector3& interPt, Vector3& interNormal) {
	Box box = { cyl.ref,{cyl.radius,cyl.halfHeight,cyl.radius} }; // Création de la box qui contient le cylindre pour l'OBB
	if (OBB(segment, box)) { // On teste l'OBB, si elle est fausse on retourne true, sinon on continue nos tests
		ReferenceFrame d1ref = { LocalToGlobalVect({0,cyl.halfHeight,0}, cyl.ref), cyl.ref.q };
		Disk d1 = { d1ref, cyl.radius }; // On crée les 2 disque du haut et du bas de notre cylindre
		ReferenceFrame d2ref = { LocalToGlobalVect({0,-cyl.halfHeight,0}, cyl.ref), cyl.ref.q };
		d2ref.q = QuaternionMultiply(d2ref.q, QuaternionFromEuler(PI, 0, 0));
		Disk d2 = { d2ref, cyl.radius };
		float t1 = 2; //On va en priorité retoruner l'intersection des disques donc leur t initial est inférieur à celui du cylindre infini
		float t2 = 2; // Les t doivent être supérieur à 1 tant qu'il n'y a pas d'intersection
		float t3 = 3;
		InfiniteCylinder InfCyl = { cyl.ref, cyl.radius }; // Création du cylindre infini
		if (IntersectSegmentDisk(segment, d1, t, interPt, interNormal)) { // On teste l'intersection pour nos deux disques et on stock le t
			t1 = t;
		}
		if (IntersectSegmentDisk(segment, d2, t, interPt, interNormal)) {
			t2 = t;
		}
		if (IntersectSegmentInfiniteCylinder(segment, InfCyl, t, interPt, interNormal)) { // On teste l'intersection avec le cylindre infini
			Vector3 buf = GlobalToLocalVect(interPt, cyl.ref); // Si il y a un point d'intersection on le passe en local
			if (buf.y < cyl.halfHeight && buf.y > -cyl.halfHeight) t3 = t; // Pour vérifier que son y se trouve bien dans l'intervalle de hauteur du cylindre, si oui on stock le t
		}
		if (t1 < t2 && t1 < t3) return IntersectSegmentDisk(segment, d1, t, interPt, interNormal); //On retourne l'intersection avec le t le plus petit à condition que l'un des t est inférieur à 1 et donc qu'il y ait collision
		else if (t2 < t3) return IntersectSegmentDisk(segment, d2, t, interPt, interNormal);
		else if (t3 <= 1) return IntersectSegmentInfiniteCylinder(segment, InfCyl, t, interPt, interNormal);
	}
	return false;
}
bool IntersectSegmentCapsule(Segment seg, Capsule capsule, float& t, Vector3& interPt, Vector3& interNormal)
{
	Box box = { capsule.ref,{capsule.radius,capsule.halfHeight + capsule.radius,capsule.radius} }; // Création de la box qui contient la capsule pour l'OBB
	if (OBB(seg, box)) // On teste l'OBB, si elle est fausse on retourne true, sinon on continue nos tests
	{
		ReferenceFrame d1ref = { LocalToGlobalVect({0,capsule.halfHeight,0}, capsule.ref), capsule.ref.q }; // On crée les 2 sphère pour tester les demi sphère du haut et du bas de notre capsule
		Sphere s1 = { d1ref, capsule.radius };
		ReferenceFrame d2ref = { LocalToGlobalVect({0,-capsule.halfHeight,0}, capsule.ref), capsule.ref.q };
		Sphere s2 = { d2ref, capsule.radius };
		float t1 = 2; //On va en priorité retoruner l'intersection des sphères donc leur t initial est inférieur à celui du cylindre infini
		float t2 = 2; // Les t doivent être supérieur à 1 tant qu'il n'y a pas d'intersection
		float t3 = 3;
		InfiniteCylinder InfCyl = { capsule.ref, capsule.radius }; // Création du cylindre infini
		if (IntersectSegmentSphere(seg, s1, t, interPt, interNormal)) {// On teste l'intersection pour nos deux sphères et on stock le t
			t1 = t;
		}
		if (IntersectSegmentSphere(seg, s2, t, interPt, interNormal)) {
			t2 = t;
		}
		if (IntersectSegmentInfiniteCylinder(seg, InfCyl, t, interPt, interNormal)) { // On teste l'intersection avec le cylindre infini
			Vector3 buf = GlobalToLocalVect(interPt, capsule.ref); // Si il y a un point d'intersection on le passe en local
			if (buf.y < capsule.halfHeight && buf.y > -capsule.halfHeight) t3 = t; // Pour vérifier que son y se trouve bien dans l'intervalle de hauteur du cylindre, si oui on stock le t
		}
		if (t1 < t2 && t1 < t3 && t1 <= 1) return IntersectSegmentSphere(seg, s1, t, interPt, interNormal); //On retourne l'intersection avec le t le plus petit à condition que l'un des t est inférieur à 1 et donc qu'il y ait collision
		else if (t2 < t3 && t2 <= 1) return IntersectSegmentSphere(seg, s2, t, interPt, interNormal);
		else if (t3 <= 1) return IntersectSegmentInfiniteCylinder(seg, InfCyl, t, interPt, interNormal);
	}
	return false;
}
bool IntersectSegmentRoundedBox(Segment seg, RoundedBox rndBox, float& t, Vector3& interPt, Vector3& interNormal)
{
	Box box = { rndBox.ref,{rndBox.extents.x + rndBox.radius,rndBox.extents.y + rndBox.radius,rndBox.extents.z + rndBox.radius} }; // Création de la box qui contient la roundedbox pour l'OBB

	if (OBB(seg, box)) // On teste l'OBB, si elle est fausse on retourne true, sinon on continue nos tests
	{
		IntersectSegmentBox(seg, box, t, interPt, interNormal); // On cherche si il y a un point de collision avec la box
		Vector3 buf = GlobalToLocalVect(interPt, rndBox.ref);
		bool isInX = (buf.x <= rndBox.extents.x && buf.x >= -rndBox.extents.x);
		bool isInY = (buf.y <= rndBox.extents.y && buf.y >= -rndBox.extents.y);
		bool isInZ = (buf.z <= rndBox.extents.z && buf.z >= -rndBox.extents.z);
		if ((isInX && isInY) || (isInY && isInZ) || (isInX && isInZ)) { // avec ce point d'intersection, on teste si il se trouve sur l'une des faces c'est à dire que deux de ses trois coordonnées se trouve dans les intervalles des extents de la roundedbox
			return true; // Si oui le point d'intersection correspond à la box car il se trouve sur une des faces, on retourne donc true
		}
		 //Sinon on teste si il se trouve sur une des arrêtes ou angles de la roundedbox à l'aide de capsule
		ReferenceFrame ref1 = { LocalToGlobalVect({-rndBox.extents.x,-rndBox.extents.y,0}, rndBox.ref), rndBox.ref.q }; // On crée toutes nos capsules
		ref1.q = QuaternionMultiply(ref1.q, QuaternionFromEuler(PI / 2, 0, 0));
		Capsule cap1 = { ref1,rndBox.extents.z,rndBox.radius };

		ReferenceFrame ref2 = { LocalToGlobalVect({-rndBox.extents.x,rndBox.extents.y,0}, rndBox.ref), rndBox.ref.q };
		ref2.q = QuaternionMultiply(ref2.q, QuaternionFromEuler(PI / 2, 0, 0));
		Capsule cap2 = { ref2,rndBox.extents.z,rndBox.radius };

		ReferenceFrame ref3 = { LocalToGlobalVect({rndBox.extents.x,-rndBox.extents.y,0}, rndBox.ref), rndBox.ref.q };
		ref3.q = QuaternionMultiply(ref3.q, QuaternionFromEuler(PI / 2, 0, 0));
		Capsule cap3 = { ref3,rndBox.extents.z,rndBox.radius };

		ReferenceFrame ref4 = { LocalToGlobalVect({rndBox.extents.x,rndBox.extents.y,0}, rndBox.ref), rndBox.ref.q };
		ref4.q = QuaternionMultiply(ref4.q, QuaternionFromEuler(PI / 2, 0, 0));
		Capsule cap4 = { ref4,rndBox.extents.z,rndBox.radius };

		ReferenceFrame ref5 = { LocalToGlobalVect({0,-rndBox.extents.y,-rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref5.q = QuaternionMultiply(ref5.q, QuaternionFromEuler(0, 0, PI / 2));
		Capsule cap5 = { ref5,rndBox.extents.x,rndBox.radius };

		ReferenceFrame ref6 = { LocalToGlobalVect({0,-rndBox.extents.y,rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref6.q = QuaternionMultiply(ref6.q, QuaternionFromEuler(0, 0, PI / 2));
		Capsule cap6 = { ref6,rndBox.extents.x,rndBox.radius };

		ReferenceFrame ref7 = { LocalToGlobalVect({0,rndBox.extents.y,-rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref7.q = QuaternionMultiply(ref7.q, QuaternionFromEuler(0, 0, PI / 2));
		Capsule cap7 = { ref7,rndBox.extents.x,rndBox.radius };

		ReferenceFrame ref8 = { LocalToGlobalVect({0,rndBox.extents.y,rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref8.q = QuaternionMultiply(ref8.q, QuaternionFromEuler(0, 0, PI / 2));
		Capsule cap8 = { ref8,rndBox.extents.x,rndBox.radius };

		ReferenceFrame ref9 = { LocalToGlobalVect({-rndBox.extents.x,0,-rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref9.q = QuaternionMultiply(ref9.q, QuaternionFromEuler(0, PI / 2, 0));
		Capsule cap9 = { ref9,rndBox.extents.y,rndBox.radius };

		ReferenceFrame ref10 = { LocalToGlobalVect({rndBox.extents.x,0,-rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref10.q = QuaternionMultiply(ref10.q, QuaternionFromEuler(0, PI / 2, 0));
		Capsule cap10 = { ref10,rndBox.extents.y,rndBox.radius };

		ReferenceFrame ref11 = { LocalToGlobalVect({-rndBox.extents.x,0,rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref11.q = QuaternionMultiply(ref11.q, QuaternionFromEuler(0, PI / 2, 0));
		Capsule cap11 = { ref11,rndBox.extents.y,rndBox.radius };

		ReferenceFrame ref12 = { LocalToGlobalVect({rndBox.extents.x,0,rndBox.extents.z}, rndBox.ref), rndBox.ref.q };
		ref12.q = QuaternionMultiply(ref12.q, QuaternionFromEuler(0, PI / 2, 0));
		Capsule cap12 = { ref12,rndBox.extents.y,rndBox.radius };

		//On les mets tous dans un tableaux puis on teste si il y a collision pour chacune
		Capsule tableCapsule[12] = { cap1,cap2,cap3,cap4,cap5,cap6,cap7,cap8,cap9,cap10,cap11,cap12 };
		float tTable[12] = { 2,2,2,2,2,2,2,2,2,2,2,2 };
		int iMin = 0;
		for (int i = 0; i < 12; i++) {
			if (IntersectSegmentCapsule(seg, tableCapsule[i], t, interPt, interNormal)) {
				tTable[i] = t;
				if (t < tTable[iMin]) {
					iMin = i;
				}
			}
		}
		if (tTable[iMin] > 1) { // si le t minimu est > 1 il n'y en a donc pas
			return false;
		}
		return IntersectSegmentCapsule(seg, tableCapsule[iMin], t, interPt, interNormal); //Sinon on retourne la plus proche
	}


	return false;
}

bool GetSphereNewPositionAndVelocityIfCollidingWithRoundedBox(Sphere sphere, RoundedBox rndBox, Vector3 velocity, float deltaTime, float& colT, Vector3& colSpherePos, Vector3& colNormal, Vector3& newPosition, Vector3& newVelocity)
{
	Vector3 interPt;
	Vector3 interNormal;
	float t;
	RoundedBox rndBox2 = { rndBox.ref,rndBox.extents,rndBox.radius + sphere.radius }; // On crée notre rounded box de Voronoï
	Segment seg = { sphere.ref.origin,Vector3Add(sphere.ref.origin,Vector3Scale(velocity,deltaTime)) }; // On crée le segment de direction notre vitesse et de taille la distance parcouru en une frame
	if (IntersectSegmentRoundedBox(seg, rndBox2, t, interPt, interNormal)) { // Si il y a intersection entre ce segment et la rounded box de voronoï il y a donc collision entre la sphère et la roundebox
		colSpherePos = interPt; //Centre de la sphere au moment de la collision
		colNormal = interNormal;
		newVelocity = Vector3Reflect(velocity, interNormal); // On prend le symétrique du vecteur vitesse par rapport à la normal
		float distanceAfterCollision = Vector3Length(Vector3Scale(velocity, deltaTime)) - Vector3Length(GlobalToLocalVect(colSpherePos, sphere.ref));//On enlève la distance déjà parcourue pour arriver au centre de la sphère
		newPosition = Vector3Add(colSpherePos, Vector3Scale(Vector3Normalize(newVelocity), distanceAfterCollision)); //On change la position avec la direction de la nouvelle vitesse et la distance qu'il reste à parcourir
		return true;
	}
	return false;

}

bool GetSphereNewPositionAndVelocityIfCollidingWithRoundedBoxes(Sphere sphere, const std::vector<RoundedBox>& rndBoxes, Vector3 velocity, float deltaTime, Vector3& posRndBoxBefore, float& colT, Vector3& colSpherePos, Vector3& colNormal, Vector3& newPosition, Vector3& newVelocity)
{
	bool isCollision = false;
	float colT2; // On duplique toutes nos variables pour pouvoir les tester avec nos différentes rounded box
	Vector3 colSpherePos2;
	Vector3 colNormal2;
	Vector3 newPosition2;
	Vector3 newVelocity2;
	Vector3 posRndBoxBefore2 = posRndBoxBefore;
	float distanceMin = INFINITY; // On initialise la distance minimum à Infini
	for (const RoundedBox& rndBox : rndBoxes) // On teste la collision entre la sphère et toutes nos roundedbox
	{
		if (GetSphereNewPositionAndVelocityIfCollidingWithRoundedBox(sphere, rndBox, velocity, deltaTime, colT2, colSpherePos2, colNormal2, newPosition2, newVelocity2))
		{
			// On va chercher la collision la plus proche de la sphère, c'est-à dire celle ou la distance entre la position de notre sphère et celle de la sphère durant la collision est la plus petite 
			if (distanceMin > Vector3Distance(sphere.ref.origin, colSpherePos2) && Vector3Distance(colSpherePos2, sphere.ref.origin) > EPSILON)//car dans dernière fonction ref devient colSpherePos2
			{
				if (Vector3Distance(posRndBoxBefore, { 0,-100,0 }) <= EPSILON || Vector3Distance(posRndBoxBefore, rndBox.ref.origin) > EPSILON) {
					colT = colT2;
					colSpherePos = colSpherePos2;
					colNormal = colNormal2;
					newPosition = newPosition2;
					newVelocity = newVelocity2;
					posRndBoxBefore2 = rndBox.ref.origin;
					isCollision = true; //On met en vrai pour dire qu'il y a eu au moins une collision
				}
			}
		}
	}
	posRndBoxBefore = posRndBoxBefore2; // On stock cette position pour ne pas avoir deux fois de suite une collision avec la même box dans la fonction de dessous
	return isCollision;
}

bool GetSphereNewPositionAndVelocityIfMultiCollidingWithRoundedBoxes(
	Sphere sphere,
	const std::vector<RoundedBox>& rndBoxes,
	Vector3 velocity,
	float rotInertia,
	Vector3 angularMomentum,
	float deltaTime,
	int nMaxSuccessiveCollisions,
	Vector3& newPosition,
	Vector3& newVelocity,
	Vector3& newAngularMomentum)
{
	float colT;
	Vector3 colSpherePos2;
	Vector3 colNormal;
	int nSuccessiveCollisions = 0;
	bool isCollision = false;
	Sphere sphere2 = sphere; // On clone la sphère pour nos tests
	const float kf = 5; // On intialise notre coef de force de frottement à 5 pour ralentir l'objet
	float distance = Vector3Length(Vector3Scale(velocity, deltaTime)); // On initialise notre distance parcouru par une frame à partir de la vitesse
	Vector3 posRndBoxBefore = { 0,-100,0 }; // On initialise la position de la rounde box avec laquelle il y a eu une collision avant avec une position qui n'existe pas dansn otre projet car au début on n'a pas d'intersection
	while (nSuccessiveCollisions < nMaxSuccessiveCollisions && GetSphereNewPositionAndVelocityIfCollidingWithRoundedBoxes(sphere2, rndBoxes, velocity, deltaTime, posRndBoxBefore, colT, colSpherePos2, colNormal, newPosition, newVelocity))
	{ // On continue de chercher des collisions tant que le nombre de collisions max testé n'est pas dépassé et qu'il y a collision
		distance -= Vector3Length(GlobalToLocalVect(colSpherePos2, sphere2.ref)); // On diminue la distance restante avec la distance par rapport à notre nouvelle collision
		if (distance <= EPSILON) {
			distance += Vector3Length(GlobalToLocalVect(colSpherePos2, sphere2.ref)); // On récupère l'ancienne distance puisque celle ci est mauvaise
			break; // Si elle est trop petite ou négative on arrête car la distance restante n'est pas assez grande pour notre nouvelle collision
		}
		if (!isCollision)isCollision = true; // On déclare qu'il y a eu collision si ce n'est pas déjà fait
		Vector3 rot = Vector3Scale(angularMomentum, 1 / rotInertia); // On déclare notre vecteur rotation
		sphere2.ref.origin = colSpherePos2; // On déplace notre sphère clone à la position de la collision
		Vector3 omegaC = Vector3Negate(Vector3Scale(colNormal, sphere.radius)); // On récupère notre vecteur oméga C
		Vector3 Scalar = Vector3CrossProduct(
			omegaC,
			Vector3Add(
				Vector3Subtract(
					velocity,
					Vector3Scale(
						colNormal,
						Vector3DotProduct(
							velocity,
							colNormal
						)
					)
				),
				Vector3CrossProduct(
					rot,
					omegaC
				)
			)
		);
		newAngularMomentum = Vector3Subtract(
			angularMomentum,
			Vector3Scale(
				Scalar,
				kf * deltaTime
			)
		); // On calcule notre nouveau moment d'inertie avec la formule du cour
		velocity = newVelocity; // On change notre vitesse
		angularMomentum = newAngularMomentum; // On change notre moment d'inertie
		nSuccessiveCollisions += 1; 
	}
	if (isCollision)newPosition = Vector3Add(newPosition, Vector3Scale(Vector3Normalize(velocity), distance)); // Si il y a eu collision on change la position avec celle obtenu après nos multiples collisions et notre distance restante

	return isCollision;
}
