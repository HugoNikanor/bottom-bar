#ifndef TETRIS_SHADER_HEADER
#define TETRIS_SHADER_HEADER

#include "shader_common.h"

void setup_shader();
void tetris_shader(byte* pixel, uint x, uint y, ulong loop);

void create_tetris_shader(shader*);

#endif // TETRIS_SHADER_HEADER
