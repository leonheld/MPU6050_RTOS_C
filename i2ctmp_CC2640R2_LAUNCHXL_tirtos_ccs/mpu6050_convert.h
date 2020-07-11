/*
 * mpu6050_convert.h
 *
 *  Created on: Jul 11, 2020
 *      Author: Leon
 */

#ifndef MPU6050_CONVERT_H_
#define MPU6050_CONVERT_H_

#include <stdlib.h>
#include <math.h>

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


    /****************example of usage ***********************************
    *                                                                   *
    *int convert_raw_to_integer(const unsigned source,                  *
    *                           -- The raw data                         *
    *                          const unsigned digits,                   *
    *                           -- Num of bits of the element           *
    *                          BOOL * validation,                       *
    *                           -- the validation value - TRUE/FALSE    *
    *                          int HighRange,                           *
    *                           -- highest Eng value possible           *
    *                          int LowRange,                            *
    *                           -- lowest Eng value possible            *
    *                          const int scale,                         *
    *                          const int offset);                       *
    *                                                                   *
    *********************************************************************/

    /*conversion of Raw to different types*/

    float convert_raw_to_float(const unsigned source,
        const unsigned digits,
        BOOL* validation,
        float LowRange,
        float HighRange,
        const float scale,
        const float offset);

    float convert_raw_to_floatsn(const unsigned source,
        const unsigned digits,
        BOOL* validation,
        float LowRange,
        float HighRange,
        const float scale,
        const float offset);


#endif /* MPU6050_CONVERT_H_ */
