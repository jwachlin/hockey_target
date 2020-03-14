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
 * @file HTESP32.h
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#ifndef HTESP32_h
#define HTESP32_h

#define HT_DEBUG

#ifdef HT_DEBUG
#define debug_print(x)			Serial.print((x))
#else
#define debug_print(x)
#endif

#ifdef HT_DEBUG
#define debug_println(x)			Serial.println((x))
#else
#define debug_println(x)
#endif

//Pins
#define GRN_PIN 								17
#define BLU_PIN 								16
#define RED_PIN 								4
#define BTN_PIN									12
#define DIP1_PIN								33
#define DIP2_PIN								27
#define DIP3_PIN								26
#define SCL_PIN									22
#define SDA_PIN									21
#define INT_PIN									25

/**
 * @brief Performs initialization 
 *
 * 	This function setups up GPIO, I2C
 */
void begin();

/**
 * @brief Manages task
 *
 */
void task();
 
uint32_t get_index(void);

#endif