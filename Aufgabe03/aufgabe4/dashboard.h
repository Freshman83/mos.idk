// --------------------------------------------------------------------------
// DashBoard - Aufgabe 4
//
// Bearbeitet von:
//     Sascha Niklas, 257146
//     David Rotärmel, 258201
//
// --------------------------------------------------------------------------

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>

struct dashboard
{
	float kmh;
	float rpm;
};

typedef struct dashboard dashboard_t;
#define DASHBOARD_ZEROINIT { 0, 0, }

#endif
