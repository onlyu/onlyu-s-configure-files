#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <OpenGL/glu.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32

#define SQUARE_WIDTH 40
#define SQUARE_HEIGHT 40

#define FRAME_PER_SECOND 60


bool init_GL()
{
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	
	//glShadeModel(GL_SMOOTH);
	glClearColor(0,0,0,0);
	//glClearDepth(1.0f);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.6f, 1.0f, 1000.0f);

	if (glGetError() != GL_NO_ERROR) {
		printf("gl init failed");
		return false;
	}
	return true;
}

bool init()
{
	if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ){
		return false;
	}

	if (SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL) == NULL) {
		return false;
	}

	if (init_GL() == false){
		return false;
	}

	SDL_WM_SetCaption("polygon test", NULL);
	return true;
}

static float triangle_rot = 0.0f;
static float quad_rot = 0.0f;

void draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glRotatef(triangle_rot, 0.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(1.5f, 0.0f, -6.0f);
	glRotatef(quad_rot, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
	glVertex3f(-3.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -3.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glEnd();

	triangle_rot += 1.0f;
	quad_rot += 2.0f;
}

int main()
{
	bool quit = false;
	SDL_Event event;
	
	init();	
	while (!quit) {
		while ( SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		draw_scene();
		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}
	SDL_Quit();
	return 0;
}
