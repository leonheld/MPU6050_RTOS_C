/*
 *  ======== mpu6050.c ========
 */

/*
 * NOTES:
 *
 *
 */
#include "i2ctmp.h"

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Sensor Headers */
#include "mpu6050.h"

/* Example/Board Header files */
#include "Board.h"

I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

Display_Handle display;


#define TASKSTACKSIZE       640

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    Display_init();
    GPIO_init();
    I2C_init();

    /* Configure the LED */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Open the HOST display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }

    /* Turn on user LED */
    //GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);
    Display_printf(display, 0, 0, "Starting the accelerometer.");

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(Board_I2C_TMP, &i2cParams);
    if (i2c == NULL) {
        Display_printf(display, 0, 0, "Error Initializing I2C\n");
        while (1);
    }
    else {
        Display_printf(display, 0, 0, "I2C Initialized!\n");
    }

    if(mpu6050_test(i2c)){
        mpu6050_reset();
        mpu6050_configAccel(MPU6050_RANGE_4G); //just testing
        mpu6050_configGyro(MPU6050_SCALE_250DPS);
    }

    uint8_t i;
    for(i = 0; i < 20; i++){
        float AccelX_tmp = getAccel(MPU6050_X_AXIS);
        Display_printf(display, 0, 0, "X Accel is %f", AccelX_tmp);

        float GyroX_tmp = getGyro(MPU6050_X_AXIS);
        Display_printf(display, 0, 0, "X Gyro is %f", GyroX_tmp);
    }

    I2C_close(i2c);
    Display_printf(display, 0, 0, "I2C closed!");

    return (NULL);
}
