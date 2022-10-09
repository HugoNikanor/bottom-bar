#define _POSIX_C_SORUCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "term_info.h"

void get_font_info(char* terminal, fontinfo* info) {

	char cmd[80];
	strcpy(cmd, "showconsolefont -iC ");
	strcat(cmd, terminal);
	FILE* fp;
	fp = popen(cmd, "r");
	if (fp == NULL) {
		/*
		printf("Failed to run command\n");
		exit(1);
		*/
		info->width  = -1;
		info->height = -1;
		info->chars  = -1;
	}

	int a, b, c;
	fscanf(fp, "%ix%ix%i", &a, &b, &c);
	pclose(fp);

	info->width  = a;
	info->height = b;
	info->chars  = c;
}

/*
 * if directly in a tty returns that, if in tmux retutrns the tty
 * that tmux runs in.
 *
 * Note that this might fail horribly if no tty is available in the
 * ways mentioned above;
 */
char* get_tty() {
	// environment $TMUX to check how to get tty name
	// system("tty") => /dev/tty1 || /dev/pts/0
	// system("tmux display-message -p '#{client_tty}') => /dev/tty1
	// system("showconsolefont -iC /dev/tty1") => 8x12x256

	const char* cmd;
	if (getenv("TMUX") != NULL) {
		cmd = "tmux display-message -p '#{client_tty}'";
	} else {
		cmd = "tty";
	}
	FILE* fp = popen(cmd, "r");
	if (fp == NULL) {
		/*
		printf("Failed to run command\n");
		exit(1);
		*/
		return "ERROR";
	}
	char* ret = NULL;
	size_t len = 0;
	getline(&ret, &len, fp);
	pclose(fp);

	ret[strnlen(ret, len) - 1] = '\0';

	return ret;
}
