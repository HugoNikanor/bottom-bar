#ifndef TERM_INFO_HEADER
#define TERM_INFO_HEADER

typedef struct {
	int width;
	int height;
	int chars;
} fontinfo;

void get_font_info(char* terminal, fontinfo*);

char* get_tty();

#endif // TERM_INFO_HEADER
