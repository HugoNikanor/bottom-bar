#include <unistd.h>
#include <pthread.h>

#include <stdio.h>

#include "game.h"

#include "tetris.h"

void* game_thread(void* args) {
	game_loop(USABLE_LINES, WIDTH);
	//game_loop(WIDTH, USABLE_LINES);
	puts("This should never appear!");
}

void setup_shader() {
	pthread_t cThread;
	pthread_create(&cThread, NULL, game_thread, NULL);

	while(_g_board_live != true)
		usleep(100000);
}

void tetris_shader(byte* pixel, uint x, uint y, ulong loop) {
	color (*board)[USABLE_LINES] = (color (*)[USABLE_LINES]) _g_board;
	
	// x, y
	// max, max -> 0, max
	// 1280, 8 -> 0, 1280
	//
	// max, 0 -> max, max
	// 1280, 0 -> 8, 1280

	// x \in [0, 8)
	// USABLE_LINES = 8
	//if (board[WIDTH - x][USABLE_LINES - y] != EMPTY) {
	//if (board[y][x] != EMPTY) {
	switch (board[x][y]) {
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
