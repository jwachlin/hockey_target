/*
MIT License

Copyright (c) 2020 by Jacob Wachlin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * @file sensor.h
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#ifndef SENSOR_h
#define SENSOR_h

#define LIS3DH_ADDRESS                  0x19

#define REG_STATUS_REG_AUX              0x07
#define REG_WHO_AM_I                    0x0F
#define WHO_AM_I_VAL                            0x33
#define REG_CTRL_REG0                   0x1E
#define REG_TEMP_CFG_REG                0x1F
#define REG_CTRL_REG1                   0x20
#define REG_CTRL_REG2                   0x21
#define REG_CTRL_REG3                   0x22
#define REG_CTRL_REG4                   0x23
#define REG_CTRL_REG5                   0x24
#define REG_CTRL_REG6                   0x25
#define REG_REFERENCE                   0x26
#define REG_STATUS_REG                  0x27
#define REG_OUT_X_L                     0x28
#define REG_OUT_X_H                     0x29
#define REG_OUT_Y_L                     0x2A
#define REG_OUT_Y_H                     0x2B
#define REG_OUT_Z_L                     0x2C
#define REG_OUT_Z_H                     0x2D
#define REG_INT1_CFG                    0x30
#define REG_INT1_SRC                    0x31
#define REG_INT1_THS                    0x32
#define REG_INT1_DUR                    0x33

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} sensor_data_t;

void init_sensor(void);
void read_sensor(sensor_data_t * data);
void reset_int_status(void);
bool get_int_status(void);

#endif