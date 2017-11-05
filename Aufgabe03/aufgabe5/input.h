// --------------------------------------------------------------------------
//  Copyright © 2014  CS -  All rights reserved
// --------------------------------------------------------------------------

// Sicherstellen, dass die Header-Datei nur einmal eingebunden wird.
#ifndef INPUT_H_
#define INPUT_H_ 1

#include <linux/can.h>  // PF_CAN, CAN_RAW, struct sockaddr_can
#include "dashboard.h"  // dashboard_t

// Funktionsprototypen
void* input_main(void *);
 int  can_parse_mki(struct can_frame *, dashboard_t *);
 int  can_parse_astra(struct can_frame *, dashboard_t *);
 int  can_parse_bmw(struct can_frame *, dashboard_t *);

#endif
