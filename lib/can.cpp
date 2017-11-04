// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

#include <stdio.h>        // printf(), perror()
#include <stdint.h>       // uint32_t
#include <stdlib.h>       // exit()
#include <string.h>       // strcpy()
#include <errno.h>        // errno

#include <net/if.h>       // struct ifreq
#include <sys/ioctl.h>    // ioctl(), SIOCGIFINDEX
#include <linux/can.h>    // PF_CAN, CAN_RAW, struct sockaddr_can

int can_open(const char *interface)
{
	// Auf NULL-Pointer prüfen
	if (!interface) {
		errno = EINVAL;
		perror("can_open");
		exit(-1);
	}

	// Generischen CAN-Bus Socket erstellen.
	int fd_can;
	fd_can = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (fd_can < 0) {
		perror("can_open.socket");
		exit(-1);
	}

	// ifreq (Interface-Request) Datenstruktur initialisieren.
	struct ifreq ifreq;
	strcpy(ifreq.ifr_name, interface);

	// Nach unserem Interface mit einem ioctl-Aufruf suchen...
	int ret;
	ret = ioctl(fd_can, SIOCGIFINDEX, &ifreq);
	if (ret) {
		perror("can_open.ioctl");
		exit(-1);
	}
	// ...nach erfolgreicher Suche ist ifreq.ifr_ifindex gesetzt.

	// sockaddr_can Datenstruktur initialisieren.
	struct sockaddr_can addr;
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifreq.ifr_ifindex;
	addr.can_addr.tp = { 0, 0 };

	// Den Socket mit dem Interface verknüpfen...
	ret = bind(fd_can, (struct sockaddr *)&addr, sizeof(addr));
	if (ret) {
		perror("can_open.bind");
		exit(-1);
	}
	// ...bei Erfolg kann mit read() und write()
	// auf das Interface zugegriffen werden.


	// Filedescriptor zurückgeben.
	return fd_can;
}
