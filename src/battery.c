#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

chargeStatus get_charge_status() {
	FILE* file =
		fopen("/sys/class/power_supply/BAT0/status", "r");
	char str[100];
	fscanf(file, "%s", str);
	fclose(file);
	if (strcmp(str, "Charging") == 0) {
		return CHARGING;
	} else if (strcmp(str, "Discharging") == 0) {
		return DISCHARGING;
	} else {
		return UNKWON;
	}
}

void set_bat_values(batteryData* data) {
	data->rate = get_battery();
	data->status = get_charge_status();
}
