#include <vector>
#include <assert.h>
#include <GL/freeglut.h>
#include <cmath>
using namespace std;
#include "vector.h"
#include "QuadMesh.h"

// Use return value as follows:
// objects[i][j] where i goes to M and j goes to N.
// in memory, j moves most rapidly.
// Call like this, to make a 2D array of Point objects:
// Point **points = Allocate2D_ArrayOfObjects(rotations, profileSize, Point());
// Remember to call DeAllocate2D_ArrayOfObjects
template <class T>
T** Allocate2D_ArrayOfObjects(int M, int N, T type) {
	T** objects;

	// Dynamic allocation of 2D array.
	objects = new T * [M];
	for (int i = 0; i < M; i++) 	{
		objects[i] = new T[N];
	}

	return objects;
}

template <class T>
void DeAllocate2D_ArrayOfObjects(T**& objects, int M) {
	assert(objects != NULL);

	for (int i = 0; i < M; i++) 	{
		delete[]objects[i];
		objects[i] = NULL;
	}
	delete[] objects;
	objects = NULL;
}

Vector3 FindTriangleNormal(const Point3& p0, const Point3& p1, const Point3& p2) {
	Vector3 v1 = p1 - p0;
	Vector3 v2 = p2 - p0;
	Vector3 v3 = CrossProduct(v1, v2);
	v3.Normalize();
	return v3;
}

QuadMesh::QuadMesh(double xProfile[], double yProfile[], int profileSize, int rotations) {
	M = rotations;
	N = profileSize;
	points = Allocate2D_ArrayOfObjects(rotations, profileSize, Point3()); // points is a rotations by profileSize array, or a 12 by 5 array. Works like: Point3 points[12][5];
	pointNormals = Allocate2D_ArrayOfObjects(rotations, profileSize, Vector3());
	faceNormals = Allocate2D_ArrayOfObjects(rotations, profileSize - 1, Vector3());

	// Set first slice  of points - the original profile.
	for (int j = 0; j < profileSize; j++) 	{
		points[0][j].p[0] = xProfile[j];
		points[0][j].p[1] = yProfile[j];
		points[0][j].p[2] = 0.0;
	}

	// Set all other rotations (slices) by rotating first slice.
	double degreesIncrement = 360.0 / rotations;
	double thetaIncrement = degreesIncrement / 180.0 * 3.141592654;
	for (int i = 1; i < rotations; i++) 	{
		double th = i * thetaIncrement;
		for (int j = 0; j < profileSize; j++) 		{
			points[i][j].p[0] = points[0][j].p[0] * cos(th) + points[0][j].p[2] * sin(th);	// x2 = x1*cos(th) + z1*sin(th)
			points[i][j].p[1] = points[0][j].p[1];										// y2 = y1;
			points[i][j].p[2] = -points[0][j].p[0] * sin(th) + points[0][j].p[2] * cos(th);	// z2 = -x1*sin(th) + z1*cos(th)
		}
	}

	// Set face normals
	for (int i = 0; i < M; i++) 	{
		for (int j = 0; j < N - 1; j++) 		{
			if (j == 0) 			{
				faceNormals[i][j] = FindTriangleNormal(points[i][j], points[(i + 1) % M][j + 1], points[i][j + 1]);
			} 			else 			{
				faceNormals[i][j] = FindTriangleNormal(points[i][j], points[(i + 1) % M][j + 0], points[i][j + 1]);
			}
		}
	}

	// Set point normals
	for (int i = 0; i < M; i++) 	{
		int i2 = i == 0 ? M - 1 : i - 1; // for wrapping

		for (int j = 0; j < N; j++) 		{
			if (j == 0) 			{
				pointNormals[i][j] = faceNormals[i][j] + faceNormals[i2][j];
			} 			else if (j == N - 1) 			{
				pointNormals[i][j] = faceNormals[i][j - 1] + faceNormals[i2][j - 1];
			} 			else 			{
				pointNormals[i][j] = faceNormals[i][j] + faceNormals[i2][j] + faceNormals[i][j - 1] + faceNormals[i2][j - 1];
			}
			pointNormals[i][j].Normalize();
		}
	}
	// if top points are identical, reset their pointNormals to their average
	bool same = true;
	for (int i = 0; i < M; i++) 	{
		int i2 = i == 0 ? M - 1 : i - 1; // for wrapping
		if (points[i][N - 1] != points[i2][N - 1]) 		{
			same = false;
			break;
		}
	}
	if (same) 	{
		Vector3 avg;
		for (int i = 0; i < M; i++)
			avg = avg + pointNormals[i][N - 1];
		avg.Normalize();
		for (int i = 0; i < M; i++)
			pointNormals[i][N - 1] = avg;
	}
	// if bottom points are identical, reset their pointNormals to their average
	same = true;
	for (int i = 0; i < M; i++) 	{
		int i2 = i == 0 ? M - 1 : i - 1; // for wrapping
		if (points[i][0] != points[i2][0]) 		{
			same = false;
			break;
		}
	}
	if (same) 	{
		Vector3 avg;
		for (int i = 0; i < M; i++)
			avg = avg + pointNormals[i][0];
		avg.Normalize();
		for (int i = 0; i < M; i++)
			pointNormals[i][0] = avg;
	}
}

void QuadMesh::DrawFlat() {
	glBegin(GL_QUADS);
	for (int i = 0; i < M; i++) 	{
		for (int j = 0; j < N - 1; j++) 		{
			glNormal3dv(faceNormals[i][j].v);
			glVertex3dv(points[i][j].p);
			glVertex3dv(points[i][j + 1].p);
			glVertex3dv(points[(i + 1) % M][j + 1].p);
			glVertex3dv(points[(i + 1) % M][j].p);
		}
	}
	glEnd();
}

void QuadMesh::DrawSmooth() {
	glBegin(GL_QUADS);
	for (int i = 0; i < M; i++) 	{
		for (int j = 0; j < N - 1; j++) 		{
			glNormal3dv(pointNormals[i][j].v);
			glVertex3dv(points[i][j].p);
			glNormal3dv(pointNormals[i][j + 1].v);
			glVertex3dv(points[i][j + 1].p);
			glNormal3dv(pointNormals[(i + 1) % M][j + 1].v);
			glVertex3dv(points[(i + 1) % M][j + 1].p);
			glNormal3dv(pointNormals[(i + 1) % M][j].v);
			glVertex3dv(points[(i + 1) % M][j].p);
		}
	}
	glEnd();
}

QuadMesh::QuadMesh() {
	M = 0;
	N = 0;
	points = NULL;
	pointNormals = NULL;
	faceNormals = NULL;
}
QuadMesh::~QuadMesh() {
	DeAllocate2D_ArrayOfObjects(points, M);
	DeAllocate2D_ArrayOfObjects(pointNormals, M);
	DeAllocate2D_ArrayOfObjects(faceNormals, M);
}
