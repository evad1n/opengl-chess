#include <GL/freeglut.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include "camera.h"
#include "input.h"

double screenX = 1500;
double screenY = 800;
double deltaTime = 0; // In milliseconds
Camera camera;


void init(void) {
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	// glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
	// glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);
	// glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0);
	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void updateLights() {
	float lightx = 0;
	float lighty = 10.0;
	float lightz = 0;
	GLfloat light1_position[] = { lightx, lighty, lightz, 1.0 };
	float dirx = 0;
	float diry = lighty;
	float dirz = 0;
	GLfloat spot1_direction[] = { dirx, -diry, dirz };
	glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot1_direction);

	glPushMatrix();
	glTranslatef(lightx, lighty, lightz);
	double rotY = atan2(dirx, dirz) * 180 / M_PI;
	glRotatef(rotY, 0, 1, 0);
	double rotX = atan2(diry, dirz) * 180 / M_PI;
	glRotatef(-rotX, 1, 0, 0);
	double rotZ = atan2(diry, dirx) * 180 / M_PI;
	glRotatef(-rotZ, 0, 0, 1);
	glutSolidCone(2, 5, 5, 5);
	glPopMatrix();


}

// Draw coordinate axes at the origin
void drawAxes(float length) {
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

void updateTime() {
	static int frame = 0;
	static auto start = std::chrono::high_resolution_clock::now();
	static auto prev = start;
	frame++;
	auto curr = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> ms = curr - prev;
	deltaTime = ms.count();
	prev = curr;

	// // FPS printer
	// double fps = 1000.0 / deltaTime;
	// std::cout << "FPS: " << fps << std::endl;

	// std::cout << deltaTime << std::endl;
}

extern double fov;


void display(void) {
	updateTime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)screenX / (GLdouble)screenY;
	gluPerspective(fov, aspectRatio, .1, 2000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.Update();
	camera.Display();

	glPushMatrix();
	glTranslatef(-20, 0, -20);
	drawAxes(10);
	glPopMatrix();



	// Draw ground
	glShadeModel(GL_SMOOTH);
	static const double size = 40;
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	// glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	// glMaterialf(GL_FRONT, GL_SPECULAR, 0);
	glPushMatrix();
	glTranslatef(0, -size, 0);
	glutSolidSphere(size, 10, 10);
	glPopMatrix();




	// glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glPushMatrix();
	glTranslatef(20, 5, 0);
	glutSolidCube(10);
	glPopMatrix();

	glFlush();

	updateLights();


	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w,
			1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5 * (GLfloat)w / (GLfloat)h,
			1.5 * (GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenX, screenY);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glutSetCursor(GLUT_CURSOR_NONE);


	camera = Camera(10, 10, 4);
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseMove);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(0.1, 0.1, 0.1, 1); // background color

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)screenX / (GLdouble)screenY;
	gluPerspective(
		/* field of view in degree */ fov,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 2000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutMainLoop();
	return 0;
}