#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/freeglut.h>
#include <iostream>
#include <map>
#include <chrono>
#include <cmath>
#include "graphics.h"
#include "input.h"
#include "drawing.h"
#include "camera.h"
#include "chess.h"
#include "moves.h"

// Global Variables
double screenX = 1500;
double screenY = 800;
double deltaTime = 0;

Camera camera;
extern double fov;
double fps;

// Textures
const int num_textures = 3;
// Textures folder will be prefixed later
std::string imageFiles[num_textures] = { "ground.jpg", "wood.jpg", "table.jpg" };
std::map<std::string, unsigned int> texMap;
unsigned int texName[num_textures];


// Functions

void setPerspectiveView() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)screenX / (GLdouble)screenY;
	gluPerspective(fov, aspectRatio, .1, 2000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	fps = 1000.0 / deltaTime;
	// std::cout << "FPS: " << fps << std::endl;

	// std::cout << deltaTime << std::endl;
}

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void) {
	updateTime();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenX, 0, screenY);
	glMatrixMode(GL_MODELVIEW);

	camera.DisplayInfo(fps);
	setPerspectiveView();
	camera.Update();
	camera.Display();


	// Set up scene
	drawAxes(1000);
	drawTable();
	drawBoard();

	// Draw pieces
	drawPieces();


	updateLights();

	glutSwapBuffers();
	glutPostRedisplay();
}



// This callback function gets ccdalled by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h) {
	// Reset our global variables to the new width and height.
	screenX = w;
	screenY = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	setPerspectiveView();
}



void loadTextures() {
	glGenTextures(num_textures, texName);

	for (int i = 0; i < num_textures; i++) {
		texMap[imageFiles[i]] = texName[i];
		std::cout << imageFiles[i] << ", " << texName[i] << std::endl;
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(("textures/" + imageFiles[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		} else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
}

void initLights() {
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
	// glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);
	// glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 8.0);

	glEnable(GL_LIGHT0);


	GLfloat light1_ambient[] = { 1, 1, 1, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0);
	glEnable(GL_LIGHT1);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

// Your initialization code goes here.
void InitializeMyStuff() {
	srand(time(NULL));
	loadPieces();
	initLights();
	loadTextures();
	camera = Camera(500, 5000, 500, CENTER, BOARD_HEIGHT * 2, CENTER);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenX, screenY);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) {
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} else {
		glutCreateWindow("Chess");
		glutSetCursor(GLUT_CURSOR_NONE);
	}

	// Input.cpp
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseMove);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Enable transparency from alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	setPerspectiveView();
	InitializeMyStuff();

	glColor3d(1, 0, 0);			  // foreground color
	glClearColor(1, 1, 1, 0); // background color

	glutMainLoop();

	return 0;
}