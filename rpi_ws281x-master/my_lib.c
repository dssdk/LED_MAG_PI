#include "my_lib.h"

#define HMC5983_ADRESS 0x1E // -- HMC5983 i2c adress


int HMC5983_Init() { 
    int fd = wiringPiI2CSetup(HMC5983_ADRESS);
	delay(250);

	wiringPiI2CWriteReg8(fd, 0x00, 0X78); // -- HMC5983 reg 1
	wiringPiI2CWriteReg8(fd, 0x01, 0XA0); // -- HMC5983 reg 2
    return fd;
}

float map(float in, float inMin, float inMax, float outMin, float outMax) {
	if (inMin < inMax) {
		if (in <= inMin) {
			return outMin;
		}
		if (in >= inMax) {
			return outMax;
		}
	}
	else {
		if (in >= inMin) {
			return outMin;
		}
		if (in <= inMax) {
			return outMax;
		}		
	}
	float scale = (in - inMin) / (inMax - inMin);
	return outMin + scale * (outMax - outMin);
}

int rgbToHex(int red, int green, int blue) {
    int hexValue = (red << 16) | (green << 8) | blue;

    return hexValue;
}