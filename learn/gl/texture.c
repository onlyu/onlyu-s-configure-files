#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <OpenGL/glu.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32

#define SQUARE_WIDTH 40
#define SQUARE_HEIGHT 40

#define FRAME_PER_SECOND 60

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define FAILED -1

GLuint texture[1];

int load_GL_textures()
{
	SDL_Surface *image;
	GLint num_colors;
	GLenum format;
	
	image = SDL_LoadBMP("nehe.bmp");
	if (image == NULL) {
		printf ("Unable to load bitmap: %s\n", SDL_GetError());
		return FAILED;
	} else {
		if ( (image->w & (image->w -1)) != 0) {
			printf ("warning: image's width is not a power of 2\n");
		}

		if ( (image->w & (image->w -1)) != 0) {
			printf ("warning: image's height is not a power of 2\n");
		}

		num_colors = image->format->BytesPerPixel;
		if (num_colors == 4) {
			if (image->format->Rmask == 0x000000ff)
				format = GL_RGBA;
			else
				format = GL_BGRA;
		} else if (num_colors == 3) {
			if (image->format->Rmask == 0x000000ff)
				format = GL_RGB;
			else
				format = GL_BGR;
		} else {
			printf ("warning: the image is not ture color..\n");
		}
		
	}

	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D( GL_TEXTURE_2D, 0, num_colors, image->w, image->h, 0, 
				  format, GL_UNSIGNED_BYTE, image->pixels );

	if (image) {
		SDL_FreeSurface(image);
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return SUCCESS;
}

// texture mapping
BOOL init_GL()
{
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;

	glEnable(GL_TEXTURE_2D);
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0,0,0,0);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.6f, 1.0f, 1000.0f);

	if (glGetError() != GL_NO_ERROR) {
		printf("gl init failed");
		return FALSE;
	}

	load_GL_textures();
	return TRUE;
}

BOOL init()
{
	if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ){
		return FALSE;
	}

	if (SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL) == NULL) {
		return FALSE;
	}

	if (init_GL() == FALSE){
		return FALSE;
	}

	SDL_WM_SetCaption("polygon test", NULL);
	return TRUE;
}

static float triangle_rot = 0.0f;
static float quad_rot = 0.0f;

void draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	glRotatef(quad_rot, 0.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexCoord2i(0, 0);
	glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2i(1, 0);
	glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2i(1, 1);
	glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2i(0, 1);
	glVertex3f(-1.0f, -1.0f,  1.0f);

	glTexCoord2i(0, 0);
	glVertex3f(-1.0f,  1.0f, -1.0f);
	glVertex3f( 1.0f,  1.0f, -1.0f);
	glVertex3f( 1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);


	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glTexCoord2i(0, 0);
	glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2i(1, 0);
	glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2i(1, 1);
	glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2i(0, 1);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2i(0, 0);
	glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2i(1, 0);
	glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2i(1, 1);
	glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2i(0, 1);
	glVertex3f(-1.0f,  1.0f, -1.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glTexCoord2i(0, 0);
	glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2i(1, 0);
	glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2i(1, 1);
	glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2i(0, 1);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2i(0, 0);
	glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2i(1, 0);
	glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2i(1, 1);
	glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2i(0, 1);
	glVertex3f( 1.0f, -1.0f, -1.0f);


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
	BOOL quit = FALSE;
	SDL_Event event;
	
	init();	
	while (!quit) {
		while ( SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				quit = TRUE;
			}
		}
		draw_scene();
		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}
	SDL_Quit();
	return 0;
}
