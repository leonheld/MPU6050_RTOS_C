/*
 * mpu6050.c
 *
 *  Created on: Jul 11, 2020
 *      Author: Leon


#include "mpu6050.h"
#include <stdbool.h>



     Take 20 samples and print them out onto the console
    uint8_t sample;
    for (sample = 0; sample < 20; sample++) {
        if (I2C_transfer(i2c, &i2cTransaction)) {

        }
        else {
            Display_printf(display, 0, 0, "I2C Bus fault.");
        }

         Sleep for 1 second
        sleep(1);
    }


*/
