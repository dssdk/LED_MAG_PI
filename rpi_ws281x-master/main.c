#include "my_lib.h" // -- lib, func and var

#define LED 24

// -- X
unsigned short int xl;
unsigned short int xh;
short int           x;

// -- Y
unsigned short int yl;
unsigned short int yh;
short int           y;

// -- Z
unsigned short int zl;
unsigned short int zh;
short int           z;

// -- LED 
int width 	    = WIDTH;
int height 	    = HEIGHT;
int led_count 	= LED_COUNT;

ws2811_led_t *matrix;

ws2811_t ledstring = {
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = LED_COUNT,
            .strip_type = STRIP_TYPE,
            .brightness = 255,
        },
        [1] =
        {
            .gpionum = 0,
            .invert = 0,
            .count = 0,
            .brightness = 0,
        },
    },
};

int matrix_init() {
    ws2811_return_t ret;

    matrix = malloc(sizeof(ws2811_led_t) * width * height);

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }
    
}

void matrix_render(void) {
    int x, y;

    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            ledstring.channel[0].leds[(y * width) + x] = matrix[y * width + x];
        }
    }
}

int main(int argc, char *argv[]) {
    matrix_init();           // -- led init
    int fd = HMC5983_Init(); // -- HMC5983 init
    
    while(1) {
	
		wiringPiI2CWriteReg8(fd, 0x02, 0X00); // -- HMC5983 mode
		
        // -- read x
		xh = wiringPiI2CReadReg8(fd, HMC5983_ADD_DATAX_MSB);
		xl = wiringPiI2CReadReg8(fd, HMC5983_ADD_DATAX_LSB);
		
        // -- read y
		yh = wiringPiI2CReadReg8(fd, HMC5983_ADD_DATAY_MSB);	
		yl = wiringPiI2CReadReg8(fd, HMC5983_ADD_DATAY_LSB);

        // -- read z
		zh = wiringPiI2CReadReg8(fd, HMC5983_ADD_DATAZ_MSB);
		zl = wiringPiI2CReadReg8(fd, HMC5983_ADD_DATAZ_LSB);
		
        x = (xh << 8) | xl; // -- max ~360 
		y = (yh << 8) | yl; // -- max ~313
		z = (zh << 8) | zl; // -- max ~335
		
        // -- map mag val
		x = map(x, -360, 360, -255, 255);
		y = map(y, -313, 313, -255, 255);
		z = map(z, -335, 335, -255, 255);

        // -- only positive
		if (x < 0) { x = x * (-1); }
		if (y < 0) { y = y * (-1); }
		if (z < 0) { z = z * (-1); }
		
        // -- "RGB" to HEX
        int HEXRes = rgbToHex(x, y, z);
        
        for (int i = 0; i < LED; i++) {
            ledstring.channel[0].leds[i] = HEXRes;    
            ws2811_render(&ledstring);
        }

        printf("RGB(%d, %d, %d) in HEX: %06X\n", x, y, z, HEXRes);
        delay(10);
	}
    return 0;
}