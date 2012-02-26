#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32

#define SQUARE_WIDTH 40
#define SQUARE_HEIGHT 40

#define FRAME_PER_SECOND 60

class Square
{
private:
	int x, y;
	int xVel, yVel;

public:
	Square()
	{
		x = 100;
		y = 100;
	}

	void handle_input()
	{
	}

	void move()
	{
	}

	void show()
	{
		glTranslatef(x, y, 0);
		glBegin(GL_QUADS);
		glColor4f(1.0,1.0,1.0,1.0);
		glVertex3f(0, 0, 0);
		glVertex3f(SQUARE_WIDTH, 0, 0);
		glVertex3f(SQUARE_WIDTH, SQUARE_HEIGHT, 0);
		glVertex3f(0, SQUARE_HEIGHT, 0);
		glEnd();
	}
};

bool init_GL()
{
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (glGetError() != GL_NO_ERROR) {
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

	SDL_WM_SetCaption("opengl test", NULL);
	return true;
}

int main()
{
	init();

	Square square;
	bool quit = false;
	SDL_Event event;
	
	while (!quit) {
		while ( SDL_PollEvent(&event)) {
			square.handle_input();

			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		square.move();
		glClear(GL_COLOR_BUFFER_BIT);
		square.show();
		glLoadIdentity();
		SDL_GL_SwapBuffers();
		SDL_Delay(100);
	}
	SDL_Quit();
	return 0;
}
