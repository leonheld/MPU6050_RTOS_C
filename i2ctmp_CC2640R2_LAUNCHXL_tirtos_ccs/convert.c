/*
 * convert.c
 *
 *  Created on: Jul 11, 2020
 *      Author: Leon
 */


#include <string.h>
#include <mpu6050_convert.h>

const unsigned CheckIfSigned[33]=
{0x00000000,
0x00000001,0x00000002,0x00000004,0x00000008,
0x00000010,0x00000020,0x00000040,0x00000080,
0x00000100,0x00000200,0x00000400,0x00000800,
0x00001000,0x00002000,0x00004000,0x00008000,
0x00010000,0x00020000,0x00040000,0x00080000,
0x00100000,0x00200000,0x00400000,0x00800000,
0x01000000,0x02000000,0x04000000,0x08000000,
0x10000000,0x20000000,0x40000000,0x80000000};
const unsigned ConvertToSigned[32]=
{0xffffffff,
0xfffffffe,0xfffffffc,0xfffffff8,0xfffffff0,
0xffffffe0,0xffffffc0,0xffffff80,0xffffff00,
0xfffffe00,0xfffffc00,0xfffff800,0xfffff000,
0xffffe000,0xffffc000,0xffff8000,0xffff0000,
0xfffe0000,0xfffc0000,0xfff80000,0xfff00000,
0xffe00000,0xffc00000,0xff800000,0xff000000,
0xfe000000,0xfc000000,0xf8000000,0xf0000000,
0xe0000000,0xc0000000,0x80000000};
const unsigned digits2bits[33]=
{0x00000000,
0x00000001,0x00000003,0x00000007,0x0000000f,
0x0000001f,0x0000003f,0x0000007f,0x000000ff,
0x000001ff,0x000003ff,0x000007ff,0x00000fff,
0x00001fff,0x00003fff,0x00007fff,0x0000ffff,
0x0001ffff,0x0003ffff,0x0007ffff,0x000fffff,
0x001fffff,0x003fffff,0x007fffff,0x00ffffff,
0x01ffffff,0x03ffffff,0x07ffffff,0x0fffffff,
0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};


int convert_raw_to_integer (const unsigned source,
                            const unsigned digits,
                            BOOL * validation,
                            int LowRange,
                            int HighRange,
                            const int scale,
                            const int offset)
{
    int tempInt;
    unsigned temp;
    temp = source & CheckIfSigned[digits];
    if (temp != 0)
        temp = source | ConvertToSigned[digits-1];
    else
        temp= source;
    memcpy (&tempInt, &temp, sizeof (unsigned));
    tempInt=tempInt*scale + offset;
    if (tempInt >= LowRange && tempInt <= HighRange)
        *validation= TRUE;
    else
        *validation = FALSE;
    return tempInt;
}
