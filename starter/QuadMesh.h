class QuadMesh
{
public:
	// Given a profile in the x,y plane, rotates it about the y axis to create the mesh.
	QuadMesh(double xProfile[], double yProfile[], int profileSize, int rotations);
	QuadMesh();
	~QuadMesh();
	void DrawFlat();
	void DrawSmooth();

	Point3 ** points;
	Vector3 ** pointNormals;
	Vector3 ** faceNormals;
	int M;
	int N;
};
