#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <stdlib.h> // rand
#include <time.h>
#include <chrono>
#include <iostream>

#include "helper.h"
#include "drawing.h"
#include "bouncyball.h"

// Global Variables
int START_BALLS = 100;
int COLLISION_PRECISION;

const double SLING_POWER = 5;
const double MAX_SPEED = 10;
const double TIME_SCALE = 1000000000.0;


int screenX = 1500;
int screenY = 800;

Vector2 mouse;
Vector2 mouseDownPos;
bool mouseDown = false;

double resistance = 0;

// List of balls
std::vector<BouncyBall> balls = {};

double randDouble(double start = 0, double end = 1) {
	// Between 0 and 1
	double r = ((double)rand() / (RAND_MAX));
	r *= abs(end - start);
	return r + start;
}

void checkCollisions(double dt) {
	for (size_t i = 0; i < balls.size(); i++) {
		for (size_t j = i + 1; j < balls.size(); j++) {
			if (isColliding(balls[i], balls[j], dt)) {
				handleCollision(balls[i], balls[j], dt);
			}
		}
	}
}

bool anyCollisions(double dt, BouncyBall b) {
	for (size_t i = 0; i < balls.size(); i++) {
		if (isColliding(balls[i], b, dt)) {
			return true;
		}
	}
	return false;
}

BouncyBall createBall(Vector2 startPos, Vector2 startVel, double gravity = 9.8, std::string color = "") {
	// Radius
	int r = rand() % 30 + 10;
	// Color
	COLOR c = { randDouble(), randDouble(), randDouble() };
	if (color == "red")
		c = { 1, 0, 0 };
	if (color == "blue")
		c = { 0, 0, 1 };


	startPos.x = clamp(startPos.x, r, screenX - r);
	startPos.y = clamp(startPos.y, r, screenY - r);

	return BouncyBall(startPos, startVel, r, c, gravity);
}

//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void) {
	// Get frames/deltaTime
	static auto start = std::chrono::system_clock::now();
	static auto prev = start;
	static int frame = 0;
	auto now = std::chrono::system_clock::now();
	frame++;

	double dt = (now - prev).count() / TIME_SCALE;

	// Current fps
	double fps = 1 / dt;
	std::cout << fps << std::endl;
	// Current time
	// std::cout << (now - start).count() / TIME_SCALE << std::endl;
	prev = now;

	dt *= 10;

	glClear(GL_COLOR_BUFFER_BIT);

	// In reality this is peformed with infinite precision, but we make do with what we have
	for (int i = 0; i < COLLISION_PRECISION; i++) {
		checkCollisions(dt);
	}


	// Draw balls
	for (auto ball = balls.begin(); ball != balls.end(); ++ball) {
		ball->update(dt, screenX, screenY, resistance);
	}

	// Draw slingshot launch vector
	if (mouseDown) {
		glColor3d(0, 0, 0);
		DrawArrow(mouse, mouseDownPos);
	}

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
	case 32: // Space
		balls.clear();
		break;
	case 'x': // Toggle gravity
		for (size_t i = 0; i < balls.size(); i++) {
			balls[i].gravity *= -1;
		}
		break;
	case 'o': // Increase resistance
		resistance += .05;
		if (resistance >= 1)
			resistance = 0.99;
		break;
	case 'p': // Reset resistance to 0
		resistance = 0;
		break;
	default:
		// std::cout << (int)c << std::endl;
		return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h) {
	// Reset our global variables to the new width and height.
	screenX = w;
	screenY = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouseClick(int mouse_button, int state, int x, int y) {
	y = screenY - y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseDownPos.x = (double)x;
		mouseDownPos.y = (double)y;
		mouseDown = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		Vector2 startVel = Vector2((mouseDownPos.x - mouse.x) / SLING_POWER, (mouseDownPos.y - mouse.y) / SLING_POWER);
		balls.push_back(createBall(mouseDownPos, startVel));
		mouseDown = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
	}
	glutPostRedisplay();
}

void mouseMove(int x, int y) {
	y = screenY - y;
	mouse.x = (double)x;
	mouse.y = (double)y;
}

void addBall(double gravity = 9.8, std::string color = "") {
	// Starting position
	Vector2 startPos = Vector2(rand() % (screenX), rand() % (screenY));
	// Starting speed
	Vector2 startVel = Vector2(randDouble(-MAX_SPEED, MAX_SPEED), randDouble(-MAX_SPEED, MAX_SPEED));
	BouncyBall b = createBall(startPos, startVel, gravity, color);
	// Make sure the ball doesn't spawn inside another
	while (anyCollisions(0, b)) {
		startPos.x = rand() % (screenX);
		startPos.y = rand() % (screenY);
		startVel.x = randDouble(-MAX_SPEED, MAX_SPEED);
		startVel.y = randDouble(-MAX_SPEED, MAX_SPEED);
		b = createBall(startPos, startVel);
	}
	balls.push_back(b);
}

// Create balls
void initBalls(int num) {
	for (int i = 0; i < num; i++) {
		// Starting position
		Vector2 startPos = Vector2(rand() % (screenX), rand() % (screenY));
		// Starting speed
		Vector2 startVel = Vector2(randDouble(-MAX_SPEED, MAX_SPEED), randDouble(-MAX_SPEED, MAX_SPEED));
		BouncyBall b = createBall(startPos, startVel);
		// Make sure the ball doesn't spawn inside another
		while (anyCollisions(0, b)) {
			startPos.x = rand() % (screenX);
			startPos.y = rand() % (screenY);
			startVel.x = randDouble(-MAX_SPEED, MAX_SPEED);
			startVel.y = randDouble(-MAX_SPEED, MAX_SPEED);
			b = createBall(startPos, startVel);
		}
		balls.push_back(b);
	}
}

void initBallsTest1() {
	// Red
	for (int i = 0; i < 5; i++) {
		addBall(9.8, "red");
	}
	// Blue
	for (int i = 0; i < 5; i++) {
		addBall(-9.8, "blue");
	}
}


// Your initialization code goes here.
void InitializeMyStuff(int numBalls) {
	srand(time(NULL));

	initBalls(numBalls);
	// initBallsTest1();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenX, screenY);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) {
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} else {
		glutCreateWindow("Bounce1");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);

	glColor3d(0, 0, 0);				// forground color
	glClearColor(0.8, 0.9, 0.8, 0); // background color

	// Parse user args
	if (argc > 1) {
		START_BALLS = atoi(argv[1]);
	}
	COLLISION_PRECISION = START_BALLS > 0 ? 1000 / START_BALLS : 1000;
	if (COLLISION_PRECISION == 0) {
		COLLISION_PRECISION = 1;
	}
	InitializeMyStuff(START_BALLS);

	glutMainLoop();

	return 0;
}