#include "util.h"

void replace_line_break_with_null(char* str) {
	char* c;
	int i = 0;
	while ( *(c = str + i++) != 0 ) {
		if (*c == '\n')
			*c = 0;
	}
}
