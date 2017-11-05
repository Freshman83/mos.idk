// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>

// Lampen
#define DASHBOARD_ABS         0x001
#define DASHBOARD_ESP         0x002
#define DASHBOARD_HANDBRAKE   0x004
#define DASHBOARD_ENGINE      0x008
#define DASHBOARD_WARN        0x010
#define DASHBOARD_WARN_BLINK  0x020
#define DASHBOARD_FUEL_LOW    0x040
#define DASHBOARD_LEFT        0x080
#define DASHBOARD_RIGHT       0x100

// Alarm
#define DASHBOARD_LOW_OIL        0x01
#define DASHBOARD_TIRE_PRESSURE  0x02
#define DASHBOARD_TIRE_DEFECT    0x04

struct dashboard
{
	float kmh;
	float rpm;
	float temp;
	float fuel;
	uint32_t lamps;
	uint32_t alerts;
	uint32_t flags;

	const char *dev;
	const char *type;
};

typedef struct dashboard dashboard_t;
#define DASHBOARD_ZEROINIT { 0, 0, 0, 0,  0, 0, 0,  0, 0 }

#endif /* LOESUNG_DEMO_GLOBAL_H_ */
