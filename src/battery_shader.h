#ifndef BATTER_SHADER_HEADER
#define BATTER_SHADER_HEADER

#include "shader_common.h"

extern batteryData batData;

// TODO should const also be noted here?
void hsvGradient(byte*, uint, uint, ulong);
void batteryShader(byte*, uint, uint, ulong);

#endif // BATTER_SHADER_HEADER
