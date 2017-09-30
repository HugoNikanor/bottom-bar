#ifndef BATTERY_HEADER
#define BATTERY_HEADER

typedef enum { UNKWON, CHARGING, DISCHARGING, FULL } chargeStatus;

typedef struct {
	double rate;
	chargeStatus status;
} batteryData;

double get_battery();

chargeStatus get_charge_status();

void set_bat_values(batteryData*);

#endif // BATTERY_HEADER
