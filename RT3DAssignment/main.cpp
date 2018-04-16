
#include <GL/freeglut.h>
#include <exception>
#include <iostream>
#include "Game.h"
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <thread>

long getCurTimeMs()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


Game* game;
long lastTime;
float timeDelta = 0.02f;
long accumulator = 0; // time accumulator
bool running = true;
bool wireframe = false;

void Display();
void Reshape(int width, int height);
void Keyboard(unsigned char c, int x, int y);
void KeyboardUp(unsigned char c, int x, int y);
void Special(int c, int x, int y);


int main(int argc, char** argv)
{
	// initiate openal (utility)
	alutInit(&argc, argv);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Treasure Hunter (B00176646)");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(Special);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);
	//glutFullScreen();

#ifdef _DEBUG
	SetCurrentDirectory("..\\Game\\");
#endif

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, 1.0, 0.1, 10000.0);

	try
	{
		game = new Game("gamefile.txt");
	}
	catch(const char* msg)
	{
		std::cout << msg;
		exit(0);
	}

	lastTime = getCurTimeMs();


	while(running)
	{
		glutMainLoopEvent();
	}

	// delete the game
	delete game;

	return 0;
}

int counter = 0;
float fpsA = 0.0;
float totalFps = 0.0;


void Display()
{
	// get the current time
	long curTime = getCurTimeMs();

	long delta = curTime - lastTime;

	float fps = 1.0f / (delta / 1000.0f);

	fpsA += fps;
	counter++;
	if(counter > 50)
	{
		totalFps = fpsA / 50.0;
		counter = 0;
		fpsA = 0.0;
	}

	std::stringstream ss;
	ss << "FPS: " << totalFps << "\n";

	// get the time delta
	accumulator += delta;
	lastTime = curTime;

	while(accumulator > (timeDelta * 1000.0f))
	{
		game->Update(timeDelta);
		accumulator -= timeDelta * 1000.0f;

		if(game->IsComplete())
		{
			running = false;
			return;
		}
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	game->Draw();

	glColor3f(1, 0, 0);
	Game::DrawString(ss.str().c_str(), 10, 90, GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(1,1,1);


	glutSwapBuffers();
	glutPostRedisplay();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float yFov = 60.0f;
	if(height == 0.0f)
	{
		gluPerspective(yFov, width, 0.1, 10000.0);
	}
	else
	{
		gluPerspective(yFov, width / (float)height, 0.1, 10000.0);
	}
	glMatrixMode(GL_MODELVIEW);
}

void Keyboard(unsigned char c, int x, int y)
{
	if(c == 27) // escape key
	{
		glutLeaveMainLoop();
	}

	game->KeyPressed(c);
}

void KeyboardUp(unsigned char c, int x, int y)
{
	game->KeyReleased(c);
}


void Special(int c, int x, int y)
{
	switch(c)
	{
		case GLUT_KEY_F1:
		{
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		}
		break;
	}
}
