// (Bart The Great)
// Chess animation starter kit.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;
#include <GL/freeglut.h>
#include "graphics.h"
#include "vector.h"
#include "QuadMesh.h"


enum PIECES { PAWN = 10, KNIGHT, BISHOP, ROOK, KING, QUEEN };


// Global Variables
// Some colors you can use, or make your own and add them
// here and in graphics.h
GLdouble redMaterial[] = { 0.7, 0.1, 0.2, 1.0 };
GLdouble greenMaterial[] = { 0.1, 0.7, 0.4, 1.0 };
GLdouble brightGreenMaterial[] = { 0.1, 0.9, 0.1, 1.0 };
GLdouble blueMaterial[] = { 0.1, 0.2, 0.7, 1.0 };
GLdouble whiteMaterial[] = { 1.0, 1.0, 1.0, 1.0 };

double screen_x = 600;
double screen_y = 500;


// Outputs a string of text at the specified location.
void text_output(double x, double y, char* string) {
	void* font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}

// Given the three triangle points x[0],y[0],z[0],
//		x[1],y[1],z[1], and x[2],y[2],z[2],
//		Finds the normal vector n[0], n[1], n[2].
void FindTriangleNormal(double x[], double y[], double z[], double n[]) {
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = x[1] - x[0];
	v1[1] = y[1] - y[0];
	v1[2] = z[1] - z[0];
	v2[0] = x[2] - x[0];
	v2[1] = y[2] - y[0];
	v2[2] = z[2] - z[0];

	// Take the cross product of v1 and v2, to find the vector perpendicular to both.
	n[0] = v1[1] * v2[2] - v1[2] * v2[1];
	n[1] = -(v1[0] * v2[2] - v1[2] * v2[0]);
	n[2] = v1[0] * v2[1] - v1[1] * v2[0];

	double size = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	n[0] /= -size;
	n[1] /= -size;
	n[2] /= -size;
}

// Loads the given data file and draws it at its default position.
// Call glTranslate before calling this to get it in the right place.
void DrawPiece(char filename[]) {
	// Try to open the given file.
	char buffer[200];
	ifstream in(filename);
	if (!in) {
		cerr << "Error. Could not open " << filename << endl;
		exit(1);
	}

	double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices.
	int done = false;
	int verts = 0; // vertices in the current polygon
	int polygons = 0; // total polygons in this file.
	do {
		in.getline(buffer, 200); // get one line (point) from the file.
		int count = sscanf(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
		done = in.eof();
		if (!done) {
			if (count == 3) // if this line had an x,y,z point.
			{
				verts++;
			} else // the line was empty. Finish current polygon and start a new one.
			{
				if (verts >= 3) {
					glBegin(GL_POLYGON);
					double n[3];
					FindTriangleNormal(x, y, z, n);
					glNormal3dv(n);
					for (int i = 0; i < verts; i++) {
						glVertex3d(x[i], y[i], z[i]);
					}
					glEnd(); // end previous polygon
					polygons++;
					verts = 0;
				}
			}
		}
	} while (!done);

	if (verts > 0) {
		cerr << "Error. Extra vertices in file " << filename << endl;
		exit(1);
	}

}

//
// GLUT callback functions
//

void Interpolate(double t, double t1, double t2,
	double& v, double v1, double v2) {
	if (t < t1)
		t = t1;
	if (t > t2)
		t = t2;
	double ratio = (t - t1) / (t2 - t1);
	v = v1 + ratio * (v2 - v1);
}

void DrawAxes(float length) {
	glDisable(GL_LIGHTING);
	glLineWidth(3.0);
	glBegin(GL_LINES);

	// x axis in red
	glColor3f(1, 0, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(length, 0.0, 0.0);
	// y axis in green
	glColor3f(0, 1, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, length, 0.0);
	// z axis in blue
	glColor3f(0, 0, 1);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, length);

	glEnd();
	glLineWidth(1.0);
	glEnable(GL_LIGHTING);
}

void DrawSphere(float x, float y, float z, float radius) {
	glPushMatrix();
	glTranslatef(x, y, z);
	int slices = 40;
	int stacks = 40;
	glutSolidSphere(radius, slices, stacks);
	//	glutWireTeapot(radius);
	glPopMatrix();
}

void DrawBox(double x1, double y1, double z1,
	double x2, double y2, double z2) {
	glBegin(GL_QUADS);

	// front face
	glNormal3d(0, 0, -1);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x1, y2, z1);
	// right face
	glNormal3d(1, 0, 0);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y1, z2);
	glVertex3d(x2, y2, z2);
	glVertex3d(x2, y2, z1);
	// top face
	glNormal3d(0, 1, 0);
	glVertex3d(x1, y2, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);
	// bottom face
	glNormal3d(0, -1, 0);
	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y1, z2);
	glVertex3d(x2, y1, z2);
	glVertex3d(x2, y1, z1);
	// back face
	glNormal3d(0, 0, 1);
	glVertex3d(x1, y1, z2);
	glVertex3d(x1, y2, z2);
	glVertex3d(x2, y2, z2);
	glVertex3d(x2, y1, z2);
	// left face
	glNormal3d(-1, 0, 0);
	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y2, z1);
	glVertex3d(x1, y2, z2);
	glVertex3d(x1, y1, z2);

	glEnd();
}

void DrawBoard() {
	GLfloat board1[] = { 0.9, 0.8, 0.9, 1.0 };
	GLfloat board2[] = { 0.2, 0.3, 0.1, 1.0 };
	glBegin(GL_QUADS);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, board1);
			else
				glMaterialfv(GL_FRONT, GL_DIFFUSE, board2);
			glNormal3d(0, 1, 0);
			double offset_x = 500;
			double offset_z = 500;
			glVertex3d(i * 1000 + offset_x, 0, j * 1000 + offset_z);
			glVertex3d(i * 1000 + offset_x, 0, (j + 1) * 1000 + offset_z);
			glVertex3d((i + 1) * 1000 + offset_x, 0, (j + 1) * 1000 + offset_z);
			glVertex3d((i + 1) * 1000 + offset_x, 0, j * 1000 + offset_z);
		}
	glEnd();
}

// This routine does NOT use the QuadMesh, like the two below.
void DrawRevolvedPawn() {
	double x[] = { 500, 125,  125,  200, 250,  200, 0 };
	double y[] = { 0,   600, 1200, 1400, 1600, 1800, 2000 };
	double z[] = { 0,   0,   0,   0,   0,   0,   0 };

	int dInc = 10;
	/*Draw the profile only
	for (int d=0; d<360; d+=dInc)
	{
		glBegin(GL_LINE_STRIP);
		for(int i=0; i<7; i++)
		{
			double th = d/180.0*3.1415926; //d*180/3.1415926;
			double x2 = x[i]*cos(th) - z[i]*sin(th);
			double z2 = x[i]*sin(th) + z[i]*cos(th);
			glVertex3d(x2, y[i], z2);
		}
		glEnd();
	} */

	glBegin(GL_QUADS);
	for (int d = 0; d < 360; d += dInc) {
		for (int i = 0; i < 6; i++) {
			double th = d / 180.0 * 3.1415926;
			double th2 = (d + dInc) / 180.0 * 3.1415926;
			double x1 = x[i] * cos(th) - z[i] * sin(th);
			double y1 = y[i];
			double z1 = x[i] * sin(th) + z[i] * cos(th);
			double x2 = x[i] * cos(th2) - z[i] * sin(th2);
			double y2 = y[i];
			double z2 = x[i] * sin(th2) + z[i] * cos(th2);
			double x3 = x[i + 1] * cos(th2) - z[i + 1] * sin(th2);
			double y3 = y[i + 1];
			double z3 = x[i + 1] * sin(th2) + z[i + 1] * cos(th2);
			double x4 = x[i + 1] * cos(th) - z[i + 1] * sin(th);
			double y4 = y[i + 1];
			double z4 = x[i + 1] * sin(th) + z[i + 1] * cos(th);
			double xx[3] = { x1, x2, x3 };
			double yy[3] = { y1, y2, y3 };
			double zz[3] = { z1, z2, z3 };
			double n[3];
			FindTriangleNormal(xx, yy, zz, n);
			glNormal3dv(n);
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
			glVertex3d(x3, y3, z3);
			glVertex3d(x4, y4, z4);
		}
	}
	glEnd();

}

void DrawProfile(double x[], double y[], int profileSize) {
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j < profileSize; j++) {
		glVertex3d(x[j], y[j], 0);
	}
	glEnd();
}


void DrawCoolPawn() {
	const int PAWN_PROFILE_SIZE = 500;
	int rotations = 72;
	double x[PAWN_PROFILE_SIZE];
	double y[PAWN_PROFILE_SIZE];

	int index = 0;
	x[index] = 500; y[index] = 0; index++;
	x[index] = 400; y[index] = 500; index++;
	x[index] = 200; y[index] = 600; index++;
	x[index] = 200; y[index] = 900; index++;
	x[index] = 300; y[index] = 1000; index++;
	x[index] = 300; y[index] = 1050; index++;
	x[index] = 100; y[index] = 1100; index++;
	x[index] = 200; y[index] = 1200; index++;
	x[index] = 260; y[index] = 1300; index++;
	x[index] = 300; y[index] = 1400; index++;
	x[index] = 250; y[index] = 1500; index++;
	x[index] = 175; y[index] = 1600; index++;
	x[index] = 000; y[index] = 1700; index++;

	//DrawProfile(x, y, index);

	QuadMesh pawn(x, y, index, rotations);
	pawn.DrawSmooth();
	//pawn.DrawFlat();
}

void DrawCoolSphere(double r) {
	int rotations = 24;

	const int PAWN_PROFILE_SIZE = 500;
	double x[PAWN_PROFILE_SIZE];
	double y[PAWN_PROFILE_SIZE];

	int index = 0;
	for (double deg = 270; deg >= 90; deg -= 15) {
		double th = deg / 180 * 3.1415926;
		x[index] = r * cos(th);
		y[index] = r * sin(th);
		index += 1;
	}

	//DrawProfile(x, y, index);

	QuadMesh sphere(x, y, index, rotations);
	//sphere.DrawFlat();
	sphere.DrawSmooth();
}


// NOTE: Y is the UP direction for the chess pieces.
double eye[3] = { 4500, 8000, -4000 }; // pick a nice vantage point.
double at[3] = { 2000, 0,     2000 };

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void) {
	static clock_t start = clock();
	clock_t finish = clock();
	double t = (double)(finish - start) / CLOCKS_PER_SEC;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	static double angle = 0.0;
	angle += .1;
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 1, 0); // Y is up!

	// Rotate the View about the At point:
	// (must do eye transformations right after gluLookAt)
	glTranslated(at[0], at[1], at[2]);
	glRotated(angle, 0, 1, 0);
	glTranslated(-at[0], -at[1], -at[2]);

	DrawAxes(1000);

	// Set the color for one side (white), and draw its 16 pieces.
	GLfloat mat_amb_diff1[] = { 0.8, 0.9, 0.5, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_amb_diff1);

	glPushMatrix();
	glTranslatef(2000, 0, 2000);
	//glScaled(.5,.5,.5);
	DrawRevolvedPawn();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6000, 0, 2000);
	DrawCoolPawn();
	glPopMatrix();


	double bish_rot = 0;
	Interpolate(t, 0, 1000, bish_rot, 0, 360 * 1000);
	glPushMatrix();
	glTranslatef(3000, 0, 1000);
	glRotated(bish_rot, 0, 1, 0);
	glRotated(10, 0, 0, 1);
	glCallList(BISHOP);
	DrawBox(-300, 0, -300, 300, 1000, 300);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(4000, 0, 1000);
	double king_y_height = 1;
	Interpolate(t, 5, 7, king_y_height, 1, 0);
	glScaled(1, king_y_height, 1);
	//DrawPiece("KING.POL");
	glCallList(KING);
	glPopMatrix();


	double queen_z = 1000;
	Interpolate(t, 2, 4, queen_z, 1000, 4000);
	glPushMatrix();
	glTranslatef(5000, 0, queen_z);
	//DrawPiece("QUEEN.POL");
	glCallList(QUEEN);
	glPopMatrix();


	double bishop_x = 6000;
	double bishop_z = 1000;
	double bishop_rot = 0;
	Interpolate(t, 0, 6, bishop_x, 6000, -4000);
	Interpolate(t, 0, 6, bishop_z, 1000, 4000);
	Interpolate(t, 0, 6, bishop_rot, 0, 360 * 5);
	glPushMatrix();
	glTranslatef(bishop_x, 0, bishop_z);
	glTranslatef(0, 700, 0);
	glRotated(bishop_rot, 0, 0, 1);
	glTranslatef(0, -700, 0);
	//DrawPiece("BISHOP.POL");
	glCallList(BISHOP);
	glPopMatrix();


	// Set the color for one side (black), and draw its 16 pieces.
	GLfloat mat_amb_diff2[] = { 0.1, 0.5, 0.8, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_amb_diff2);

	glPushMatrix();
	glTranslatef(4000, 0, 8000);
	double deg = 0;
	Interpolate(t, 3, 6, deg, 0, -90);
	glRotated(deg, 0, 0, 1);
	glCallList(KING);
	glPopMatrix();

	for (int x = 2000; x <= 7000; x += 1000) {
		glPushMatrix();
		glTranslatef(x, 0, 7000);
		//DrawPiece("PAWN.POL");
		glCallList(PAWN);
		glPopMatrix();
	}

	DrawBoard();

	glPushMatrix();
	glTranslatef(4000, 800, 4000);
	DrawCoolSphere(800);
	//glutSolidTeapot(800);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0000, 800, 4000);
	glutSolidSphere(800, 50, 50);
	//glutSolidTeapot(800);
	glPopMatrix();

	// Set the light position, and draw it yellow.
	double lightx = 2000;
	double lighty = 1500;
	double lightz = 4000;
	GLfloat yellow[] = { 1.0, 1.0, 0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
	glPushMatrix();
	glTranslatef(lightx, lighty, lightz);
	glutSolidSphere(200, 50, 50);
	glPopMatrix();

	// MUST do light position here at the end.
	GLfloat light_position[] = { 1,2,-.1, 0 }; // light comes FROM this vector direction, when last parameter is zero.
//	GLfloat light_position[] = { lightx, lighty, lightz, 1 }; // light comes FROM this vector direction, when last parameter is zero.
															// light is AT this position when last parameter is one.
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light

	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y) {
	switch (c) {
	case 27: // escape character means to quit the program
		exit(0);
		break;
	default:
		return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}



void SetPerspectiveView(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 45.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ 100, /* Z far */ 30000.0);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h) {
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w, h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y) {
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff() {
	// set material's specular properties
	GLfloat mat_specular[] = { .9,.9,.9, 1.0 };
	GLfloat mat_shininess[] = { 90.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// set light properties
	GLfloat white_light[] = { 1,1,1,1 };
	GLfloat low_light[] = { .3,.3,.3,1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);

	glEnable(GL_DEPTH_TEST); // turn on depth buffering
	glEnable(GL_LIGHTING);	// enable general lighting
	glEnable(GL_LIGHT0);	// enable the first light.

	glNewList(PAWN, GL_COMPILE);
	DrawPiece("PAWN.POL");
	glEndList();

	glNewList(KING, GL_COMPILE);
	DrawPiece("KING.POL");
	glEndList();

	glNewList(QUEEN, GL_COMPILE);
	DrawPiece("QUEEN.POL");
	glEndList();

	glNewList(BISHOP, GL_COMPILE);
	DrawPiece("BISHOP.POL");
	glEndList();

}


int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	int fullscreen = 0;
	if (fullscreen) {
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} else {
		glutCreateWindow("2018 Revolving and Lighting");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glClearColor(1, 1, 1, 1);
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
