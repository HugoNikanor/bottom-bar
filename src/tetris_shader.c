#include "tetris_shader.h"

#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#include <tetris.h>

struct tetris_shader {
	shader shader;
	pthread_t thread;
};

static void init_shader(tetris_shader*);
static void free_shader(tetris_shader*);
static void run_shader(tetris_shader*, byte pixel[4], uint x, uint y, ulong loop);

int width;
int height;
// Makes the board smaller, and the displayed tetraminos larger.
// A non intereger result from (USABLE_LINES * scale) is undesirable.
double scale = 0.5;

void* game_thread(void* args) {
	tetris_settings settings = {
		.width = width,
		.height = height,
		.dropspeed = 1,
		.move_func = &move_generator
	};
	game_loop(&settings);
	puts("This should never appear!");
	return NULL;
}

static void init_shader(tetris_shader *shader) {

	width = USABLE_LINES * scale;
	height = WIDTH * scale;

	pthread_create(&shader->thread, NULL, game_thread, NULL);

	while(_g_board_live != true)
		usleep(100000);
}

static void free_shader(tetris_shader *shader) {
	pthread_cancel(shader->thread);
	free(shader);
}

static void run_shader(tetris_shader *shader, byte pixel[4], uint x, uint y, ulong loop) {
	color (*board)[width] = (color (*)[width]) _g_board;
	
	int xp = x * scale;
	int yp = y * scale;
	switch (board[xp][yp]) {
		case GREEN:
			pixel [R] = 0;
			pixel [G] = 0xFF;
			pixel [B] = 0;
			break;
		case RED:
			pixel [R] = 0xFF;
			pixel [G] = 0;
			pixel [B] = 0;
			break;
		case ORANGE:
			pixel [R] = 0xFF;
			pixel [G] = 0x7F;
			pixel [B] = 0;
			break;
		case YELLOW:
			pixel [R] = 0xFF;
			pixel [G] = 0xFF;
			pixel [B] = 0;
			break;
		case BLUE:
			pixel [R] = 0;
			pixel [G] = 0;
			pixel [B] = 0xFF;
			break;
		case MAGENTA:
			pixel [R] = 0xFF;
			pixel [G] = 0;
			pixel [B] = 0xFF;
			break;
		case CYAN:
			pixel [R] = 0;
			pixel [G] = 0xFF;
			pixel [B] = 0xFF;
			break;
		case EMPTY:
		default:
			pixel [R] = 0;
			pixel [G] = 0;
			pixel [B] = 0;
	}
}

shader *create_tetris_shader () {
	tetris_shader *sh = malloc(sizeof(*sh));
	sh->shader.init = (init_t) init_shader;
	sh->shader.cleanup = (cleanup_t) free_shader;
	sh->shader.shader = (run_t) run_shader;
	return (shader*) sh;
}

