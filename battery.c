#include <stdio.h>
#include <stdlib.h>

#include "battery.h"

/*
 * This is currently really slow, since it reads both files every time,
 * and doesn't keep the file handlers
 */
double get_battery() {
	FILE* fullFile =
		fopen("/sys/class/power_supply/BAT0/charge_full", "r");
	FILE* currentFile =
		fopen("/sys/class/power_supply/BAT0/charge_now", "r");

	int full, current;
	fscanf(fullFile, "%i", &full);
	fscanf(currentFile, "%i", &current);

	double ratio = (double) current / full;
	//printf("%i / %i = %f\n", current, full, ratio);

	fclose(fullFile);
	fclose(currentFile);

	return ratio;
}
