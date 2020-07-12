/*
 *  ======== mpu6050.c ========
 */

/*
 * NOTES:
 *
 *
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Sensor Headers */
#include "mpu6050.h"
#include "mpu6050_convert.h"

/* Example/Board Header files */
#include "Board.h"

#define TASKSTACKSIZE       640

static Display_Handle display;

I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

/*
 * MPU6050 self-test
 *
 */
//==================================================================================================================================
bool mpu6050_test(I2C_Handle handle){
    uint8_t txData = MPU6050_REG_WHO_AM_I;
    uint8_t rxData = 0x00;

    i2cTransaction.slaveAddress = MPU6050_ADDRESS;
    i2cTransaction.writeBuf     = &txData;
    i2cTransaction.writeCount   = sizeof(uint8_t);
    i2cTransaction.readBuf      = &rxData;
    i2cTransaction.readCount    = sizeof(uint8_t);
    transferStatus = I2C_transfer(handle, &i2cTransaction);

    if(transferStatus && rxData == MPU6050_ADDRESS){
        Display_printf(display, 0, 0, "MPU6050: PASS SELF TEST");
        return true;
    }
    else {
        Display_printf(display, 0, 0, "MPU6050: FAIL SELF TEST");
        while(1);
    }
}
//==================================================================================================================================
/*
 * Write a byte
 * */
bool mpu6050_writeByte(char reg, char value){
    char txData[] = {reg, value};
    uint8_t rxData = 0x00;

    Display_printf(display, 0, 0, "MPU6050: STARTED WRITING BYTE");
    i2cTransaction.slaveAddress = MPU6050_ADDRESS;
    i2cTransaction.writeBuf     = txData;
    i2cTransaction.writeCount   = sizeof(txData);
    i2cTransaction.readBuf      = &rxData;
    i2cTransaction.readCount    = sizeof(uint8_t);

    if(I2C_transfer(i2c, &i2cTransaction)){
        Display_printf(display, 0, 0, "MPU6050: PASS WRITING REGISTER");
        return true;
    }

    else Display_printf(display, 0, 0, "MPU6050: FAIL WRITING REGISTER");
    return false;
}
//==================================================================================================================================

/*
 * Read a Byte
 *
 * */
int8_t mpu6050_readByte(char reg){
    char txData[] = {reg};

    uint8_t rxData = 0x00;

    i2cTransaction.writeBuf     = txData;
    i2cTransaction.writeCount   = sizeof(txData);
    i2cTransaction.readBuf      = &rxData;
    i2cTransaction.readCount    = sizeof(rxData);
    transferStatus = I2C_transfer(i2c, &i2cTransaction);

    if(transferStatus)
        return rxData;

    return -10;
}
//==================================================================================================================================

/*
 * Reset the MPU6050 internal state
 *
 * */
bool mpu6050_reset(){
    Display_printf(display, 0, 0, "MPU6050: STARTED RESET");

    if(mpu6050_writeByte(MPU6050_REG_PWR_MGMT_1, 0x0)){

        Display_printf(display, 0, 0, "MPU6050: PASSED RESET");
        return true;
    }
    Display_printf(display, 0, 0, "MPU6050: FAIL RESET");
    return false;
}
//==================================================================================================================================

/*
 * Config the Accelerometer
 *
 * */
bool mpu6050_configAccel(mpu6050_range_t range){
    Display_printf(display, 0, 0, "MPU6050: CONFIGURING ACCELEROMETER");
    switch (range) {
    case MPU6050_RANGE_2G:
        rangeAccel = 2;
        scaleAccel = 1/16384.0;
        break;
    case MPU6050_RANGE_4G:
        rangeAccel = 4;
        scaleAccel = 1/8192.0;
        break;
    case MPU6050_RANGE_8G:
        rangeAccel = 8;
        scaleAccel = 1/4096.0;
        break;
    case MPU6050_RANGE_16G:
        rangeAccel = 16;
        scaleAccel = 1/2048.0;
        break;
    default:
        break;
    }

    if(mpu6050_writeByte(MPU6050_REG_ACCEL_CONFIG, (range << 3))){
        Display_printf(display, 0, 0, "MPU6050: ACCEL CONFIGURED");
        return true;
    }
    Display_printf(display, 0, 0, "MPU6050: FAILED TO CONFIGURE ACCEL");
    return false;

}
//==================================================================================================================================

/*
 * Config the Gyro
 *
 * */
bool mpu6050_configGyro(mpu6050_dps_t range){
    Display_printf(display, 0, 0, "MPU6050: CONFIGURING GYRO");
    switch (range) {
    case MPU6050_SCALE_250DPS:
        rangeGyro = 250;
        scaleGyro = 1/131.0;
        break;
    case MPU6050_SCALE_500DPS:
        rangeGyro = 500;
        scaleGyro = 1/65.5;
        break;
    case MPU6050_SCALE_1000DPS:
        rangeGyro = 1000;
        scaleGyro = 1/32.8;
        break;
    case MPU6050_SCALE_2000DPS:
        rangeGyro = 2000;
        scaleGyro = 1/16.4;
        break;
    default:
        break;
    }

    if(mpu6050_writeByte(MPU6050_REG_GYRO_CONFIG, (range << 3))){
        Display_printf(display, 0, 0, "MPU6050: GYRO CONFIGURED");
        return true;
    }
    Display_printf(display, 0, 0, "MPU6050: FAILED TO CONFIGURE GYRYO");
    return false;

}
//==================================================================================================================================
/*
 * Get axis' acceleration
 *
 * */
float getAccel(mpu6050_axis axis){
    uint16_t raw;
    switch (axis) {
        case MPU6050_X_AXIS:
            raw = (mpu6050_readByte(MPU6050_REG_ACCEL_XOUT_H) << 8) | (mpu6050_readByte(MPU6050_REG_ACCEL_XOUT_L));
            break;
        case MPU6050_Y_AXIS:
            raw = (mpu6050_readByte(MPU6050_REG_ACCEL_YOUT_H) << 8) | (mpu6050_readByte(MPU6050_REG_ACCEL_YOUT_L));
            break;
        case MPU6050_Z_AXIS:
            raw = (mpu6050_readByte(MPU6050_REG_ACCEL_ZOUT_H) << 8) | (mpu6050_readByte(MPU6050_REG_ACCEL_ZOUT_L));
            break;
        default:
            break;
    }
    return raw;
}

//==================================================================================================================================

/*
 * Get X axis Gyro
 *
 * */

float getGyro(mpu6050_axis axis){
    uint16_t raw;
    switch(axis) {
        case MPU6050_X_AXIS:
            raw = (mpu6050_readByte(MPU6050_REG_GYRO_XOUT_H) << 8) | (mpu6050_readByte(MPU6050_REG_GYRO_XOUT_L));
            break;
        case MPU6050_Y_AXIS:
            raw = (mpu6050_readByte(MPU6050_REG_GYRO_YOUT_H) << 8) | (mpu6050_readByte(MPU6050_REG_GYRO_YOUT_L));
            break;
        case MPU6050_Z_AXIS:
            raw = (mpu6050_readByte(MPU6050_REG_GYRO_ZOUT_H) << 8) | (mpu6050_readByte(MPU6050_REG_GYRO_ZOUT_L));
            break;
        default:
            break;
    }
        return raw;

}
//==================================================================================================================================

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
