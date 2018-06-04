#ifndef BUMPERS_H
#define BUMPERS_H
#include "asf.h"
#include "UARTsetup.h"

enum bumpers_enum{
	FRONT,
	BACK,
	RIGHT,
	LEFT
} bumpers_enum;

void init_bumpers();
int bumpers_is_pushed();
int read_bumper(enum bumpers_enum bumper);
#endif //BUMPERS_H