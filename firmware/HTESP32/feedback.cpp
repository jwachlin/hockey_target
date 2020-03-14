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
 * @file feedback.cpp
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#include <Arduino.h>
#include <Wire.h>

#include <esp_wifi.h>

#include "HTESP32.h"
#include "feedback.h"

void set_color(uint8_t red, uint8_t grn, uint8_t blu)
{
    ledcWrite(0, red);
    ledcWrite(1, grn);
    ledcWrite(2, blu);
}

static void red_on(void)
{
    ledcWrite(0, 255);
}

static void green_on(void)
{
    ledcWrite(1, 255);
}

static void blue_on(void)
{
    ledcWrite(2, 255);
}

void init_feedback(void)
{
    ledcSetup(0, 5000, 8);
    ledcSetup(1, 5000, 8);
    ledcSetup(2, 5000, 8);

    ledcAttachPin(RED_PIN, 0);
    ledcAttachPin(GRN_PIN, 1);
    ledcAttachPin(BLU_PIN, 2);
}