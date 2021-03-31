// Vector.h
// By Barton Stander
// April 2001
// For CS 3600 Graphics Programming

class Point3
{
public:
	Point3() {p[0] = p[1] = p[2] = 0.0;}
	Point3(double x, double y, double z) {p[0]=x; p[1]=y; p[2]=z;}
	double p[3];
	bool operator==(const Point3 & rhs); // checks for equality within a tolerance.
	bool operator!=(const Point3 & rhs); // checks for inequality within a tolerance.
};

class Point4
{
public:
	double p[4];
};

class Vector3
{
public:
	Vector3() {v[0] = v[1] = v[2] = 0.0;}
	double v[3];
	double Length(); // return vectors length
	double Normalize(); // normalize vector and return length;
};

Vector3 operator-(const Point3 &head, const Point3 &tail);
Vector3 operator-(const Point4 &head, const Point3 &tail);
Vector3 operator*(const Vector3 & v, double s);
Vector3 operator*(double s, const Vector3 & v);
Point3 operator+(const Point3 & p, const Vector3 & v);
Point3 operator+(const Vector3 & v, const Point3 & p);
Point3 operator-(const Point3 & p, const Vector3 & v);
Vector3 CrossProduct(const Vector3 &v1, const Vector3 &v2);
double DotProduct(const Vector3 &v1, const Vector3 &v2);
Point3 AffineSum(const Point3 &start, const Point3 & end, double t);
double DistanceBetweenPoints(const Point3 & p1, const Point3 & p2);
Vector3 operator+(const Vector3 & v1, const Vector3 & v2);

