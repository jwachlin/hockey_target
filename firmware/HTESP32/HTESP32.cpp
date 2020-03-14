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
 * @file HTESP32.cpp
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#include <Arduino.h>
#include <Wire.h>

#include <esp_wifi.h>

#include "HTESP32.h"
#include "sensor.h"
#include "network.h"
#include "feedback.h"
#include "games.h"

static uint32_t offset = 0;
static GAME_TYPE this_game = BEAT_THE_CLOCK;

/**
 * @brief Performs initialization 
 *
 * 	This function setups up GPIO, I2C
 */
void begin()
{
	pinMode(RED_PIN, OUTPUT);
	pinMode(GRN_PIN, OUTPUT);
	pinMode(BLU_PIN, OUTPUT);

	digitalWrite(RED_PIN, LOW);
	digitalWrite(GRN_PIN, LOW);
	digitalWrite(BLU_PIN, LOW);

	pinMode(BTN_PIN, INPUT_PULLUP);
	pinMode(DIP1_PIN, INPUT_PULLUP);
	pinMode(DIP2_PIN, INPUT_PULLUP);
	pinMode(DIP3_PIN, INPUT_PULLUP);

	pinMode(INT_PIN, INPUT_PULLDOWN);

	Wire.begin(SDA_PIN,SCL_PIN,100000);

	#ifdef HT_DEBUG
	Serial.begin(115200);
	#endif

	// Get index based on dipswitches
	offset = 0;
	uint8_t switches[] = {DIP1_PIN, DIP2_PIN, DIP3_PIN};
	for(int32_t i = 0; i < 3; i++)
	{
		if(!digitalRead(switches[i]))
		{
			offset |= (1 << i);
		}
	}
	debug_print("This index is ");
	debug_println(offset);

	init_sensor();
	init_network();
	init_feedback();
}

/**
 * @brief Manages task
 *
 */
void task()
{
	handle_game(this_game);
}

uint32_t get_index(void)
{
	return offset;
}