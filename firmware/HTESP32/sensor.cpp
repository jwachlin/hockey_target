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
 * @file sensor.cpp
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#include <Arduino.h>
#include <Wire.h>

#include "HTESP32.h"
#include "sensor.h"

bool sensor_int_recv = false;

void IRAM_ATTR handle_sensor_int()
{
    debug_println("Impact interrupt!");
    sensor_int_recv = true;
}

static void read_registers(uint8_t reg, uint8_t * buffer, uint32_t len)
{
    Wire.beginTransmission(LIS3DH_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(LIS3DH_ADDRESS, len);
    while(Wire.available())
    {
        *(buffer++) = Wire.read();
    }
}

static void write_registers(uint8_t reg, uint8_t * buffer, uint32_t len)
{
    Wire.beginTransmission(LIS3DH_ADDRESS);
    Wire.write(reg);
    for(uint32_t i = 0; i < len; i++)
    {
        Wire.write(buffer[i]);
    }
    Wire.endTransmission();
}

void init_sensor(void)
{
    delay(10);
    uint8_t me = 0;
    read_registers(REG_WHO_AM_I, &me, 1);
    if(me != WHO_AM_I_VAL)
    {
        debug_println("Couldn't talk to accelerometer");
        return;
    }
    debug_println("Accelerometer recognized");

    // Set up sensor
    uint8_t data = (0x07 << 4) | (0x07 << 0);   // 400Hz, all axes enabled
    write_registers(REG_CTRL_REG1, &data, 1);

    data = (0x01 << 6); // IA1 interrupt on INT1
    write_registers(REG_CTRL_REG3, &data, 1);

    data = (0x03 << 4) | (0x01 << 3); // 16g range, high resolution
    write_registers(REG_CTRL_REG4, &data, 1);

    data = (0x01 << 5) | (0x01 << 3) | (0x01 << 1); // Or combination of high events on x,y,z
    write_registers(REG_INT1_CFG, &data, 1);

    data = 7; // 1 LSb = 186 mg @ FS = ±16 g
    write_registers(REG_INT1_THS, &data, 1);

    data = 20; // 1 LSb = 1/ODR
    write_registers(REG_INT1_DUR, &data, 1);

    // Set up interrupt
    reset_int_status();
    attachInterrupt(INT_PIN, handle_sensor_int, RISING);
}

void read_sensor(sensor_data_t * data)
{
    uint8_t buffer[6];
    read_registers(REG_OUT_X_L | 0x80, buffer, 6);
    memcpy(&data->x, &buffer[0], 2);
    memcpy(&data->y, &buffer[2], 2);
    memcpy(&data->z, &buffer[4], 2);
}

void reset_int_status(void)
{
    sensor_int_recv = false;
}

bool get_int_status(void)
{
    return sensor_int_recv;
}