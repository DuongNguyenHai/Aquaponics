#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "ds18B20IAR.h"

float ReadTempt() {
	return ReadTemperature();
}

#endif
