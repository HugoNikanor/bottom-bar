#include <unistd.h>
#include <pthread.h>

#include <stdio.h>

#include <tetris.h>

#include "tetris_shader.h"

int width;
int height;
// Makes the board smaller, and the displayed tetraminos larger.
// A non intereger result from (USABLE_LINES * scale) is undesirable.
double scale = 0.5;

void* game_thread(void* args) {
	game_loop(width, height, 1, &move_generator);
	puts("This should never appear!");
	return NULL;
}

void setup_shader() {

	width = USABLE_LINES * scale;
	height = WIDTH * scale;

	pthread_t cThread;
	pthread_create(&cThread, NULL, game_thread, NULL);

	while(_g_board_live != true)
		usleep(100000);
}

void tetris_shader(byte* pixel, uint x, uint y, ulong loop) {
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
