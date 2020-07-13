#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Sensor Headers */
#include "mpu6050.h"

/* Board-specific header*/
#include "i2ctmp.h"

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
#ifdef VERBOSE
        Display_printf(display, 0, 0, "MPU6050: PASS SELF TEST");
#endif
        return true;
    }
    else {
#ifdef VERBOSE
        Display_printf(display, 0, 0, "MPU6050: FAIL SELF TEST");
#endif
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
#ifdef VERBOSE
    Display_printf(display, 0, 0, "MPU6050: STARTED WRITING BYTE");
#endif
    i2cTransaction.slaveAddress = MPU6050_ADDRESS;
    i2cTransaction.writeBuf     = txData;
    i2cTransaction.writeCount   = sizeof(txData);
    i2cTransaction.readBuf      = &rxData;
    i2cTransaction.readCount    = sizeof(uint8_t);

    if(I2C_transfer(i2c, &i2cTransaction)){
#ifdef VERBOSE
        Display_printf(display, 0, 0, "MPU6050: PASS WRITING REGISTER");
#endif
        return true;
    }
#ifdef VERBOSE
    else
        Display_printf(display, 0, 0, "MPU6050: FAIL WRITING REGISTER");
#endif

    return false;
}
//==================================================================================================================================

/*
 * Read a Byte
 *
 * */
//No printfs here because it would get too crammed. It works pretty nicely, though.
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
#ifdef VERBOSE
    Display_printf(display, 0, 0, "MPU6050: STARTED RESET");
#endif

    if(mpu6050_writeByte(MPU6050_REG_PWR_MGMT_1, 0x0)){
#ifdef VERBOSE
        Display_printf(display, 0, 0, "MPU6050: PASSED RESET");
#endif
        return true;
    }
#ifdef VERBOSE
    Display_printf(display, 0, 0, "MPU6050: FAIL RESET");
#endif
    return false;
}
//==================================================================================================================================

/*
 * Config the Accelerometer
 *
 * */
bool mpu6050_configAccel(mpu6050_range_t range){
#ifdef VERBOSE
    Display_printf(display, 0, 0, "MPU6050: CONFIGURING ACCELEROMETER");
#endif
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
#ifdef VERBOSE
        Display_printf(display, 0, 0, "MPU6050: ACCEL CONFIGURED");
#endif
        return true;
    }
#ifdef VERBOSE
    Display_printf(display, 0, 0, "MPU6050: FAILED TO CONFIGURE ACCEL");
#endif
    return false;

}
//==================================================================================================================================

/*
 * Config the Gyro
 *
 * */
bool mpu6050_configGyro(mpu6050_dps_t range){
#ifdef VERBOSE
    Display_printf(display, 0, 0, "MPU6050: CONFIGURING GYRO");
#endif
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
#ifdef VERBOSE
        Display_printf(display, 0, 0, "MPU6050: GYRO CONFIGURED");
#endif
        return true;
    }
#ifdef VERBOSE
    Display_printf(display, 0, 0, "MPU6050: FAILED TO CONFIGURE GYRYO");
#endif
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
