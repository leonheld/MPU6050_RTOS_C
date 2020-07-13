/*
 * i2ctmp.h
 *
 *  Created on: Jul 12, 2020
 *      Author: Leon
 */

#ifndef I2CTMP_H_
#define I2CTMP_H_

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

extern I2C_Handle      i2c;
extern I2C_Params      i2cParams;
extern I2C_Transaction i2cTransaction;

extern Display_Handle display;


#endif /* I2CTMP_H_ */
