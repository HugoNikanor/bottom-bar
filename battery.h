#ifndef BATTERY_HEADER
#define BATTERY_HEADER

typedef enum { UNKWON, CHARGING, DISCHARGING } chargeStatus;

typedef struct {
	double rate;
	chargeStatus status;
} batteryData;

double get_battery();

chargeStatus get_charge_status();

void setBatValues(batteryData*);

#endif // BATTERY_HEADER
