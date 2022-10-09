#ifndef BATTER_SHADER_HEADER
#define BATTER_SHADER_HEADER

#include "shader_common.h"

// Make sure that this is actually defined somewhere
// Currently it works because main defines it. And
// that's the only place which includes this file.
extern batteryData batData;

void init_shader ();

// TODO should const also be noted here?
void hsvGradient(byte[4], uint, uint, ulong);
void batteryShader(byte[4], uint, uint, ulong);

void create_battery_shader(shader*);

#endif // BATTER_SHADER_HEADER
