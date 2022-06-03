#include <ecs.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <cglm/cglm.h>

SDL_Window *window;
int running;

int init(int width, int height);
void end(void);
void gameloop(void);
void render(void);
void handle_event(SDL_Event e);
void populate_ecs(void);

int
init(int width, int height)
{
	/* start ECS memory manager */
	ecs_init();
	/* begin SDL2 */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(
			stderr,
			"SDL2 failed to initialize. Exiting.\nSDL_Error: %s\n",
			SDL_GetError()
		);
		goto STAGE1_ERROR;
	}

	/* set openGL version for SDL2 */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	 /* create window */
	window = SDL_CreateWindow(
		"cgfw",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		fprintf(
			stderr,
			"Failed to create window. Exiting.\nSDL_Error: %s\n",
			SDL_GetError(
		));
		goto STAGE2_ERROR;
	}
	/* create graphics context */
	SDL_GLContext g_context = SDL_GL_CreateContext(window);
	if (g_context == NULL) {
		fprintf(stderr, "Failed to create graphics context. Exiting.\nSDL_Error: %s\n", SDL_GetError());
		goto STAGE3_ERROR;
	}

	/* initialize GLEW */
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW. Exiting.\nGLEW_Error: %s\n", glewGetErrorString(glewError));
		goto STAGE3_ERROR;
	}

	return 0;

	STAGE3_ERROR:
		SDL_DestroyWindow(window);
	STAGE2_ERROR:
		SDL_Quit();
	STAGE1_ERROR:
		return -1;
}

void
end(void)
{
	SDL_DestroyWindow(window);
	ecs_exit();
}

void
handle_event(SDL_Event e)
{
	if (e.type == SDL_QUIT) {
		running = 0;
	}
}

void
gameloop(void)
{
	SDL_Event e;

	running = 1;
	while (running) {
		while (SDL_PollEvent(&e) != 0) {
			handle_event(e);
		}
		render();
	}
}


void
render(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(window);
}

int
main (int argc, char *argv[])
{
	init(1080, 720);
	gameloop();
	end();
	return 0;
}
