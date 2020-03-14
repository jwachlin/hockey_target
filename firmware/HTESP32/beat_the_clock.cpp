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
 * @file beat_the_clock.cpp
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include <string.h>

#include "HTESP32.h"
#include "network.h"
#include "games.h"
#include "sensor.h"
#include "feedback.h"
#include "beat_the_clock.h"

#define NUM_DEVICES         2       // Including self TODO detect who is available on the network
#define TIMEOUT_MS          5000

volatile static bool msg_recv = false;
volatile static esp_now_msg_t last_msg_recv;
volatile static uint32_t time_to_end_sense = 0;
volatile static uint32_t time_to_begin_sense = 0;

static int32_t wait_for_impact(void)
{
    if(millis() > time_to_end_sense)
    {
        //Timeout
        set_color(0,0,0); // turn off leds
        return -2;
    }
    else
    {
        // Map time to led range, start full on and decrease
        // TODO nonlinear map for intensity
        uint32_t total_time = time_to_end_sense - time_to_begin_sense;
        uint32_t time_so_far = millis() - time_to_begin_sense;
        uint8_t led = map(time_so_far, 0, total_time, 255, 0);
        set_color(led,0,0); 
    }
    

    if(get_int_status())
    {
        reset_int_status();
        int32_t time_to_impact = millis() - time_to_begin_sense;
        time_to_end_sense = millis() -10; // ensure it times out always until new command
        set_color(0, 255, 0);
        delay(100); // TODO make this not delay
        set_color(0,0,0); // turn off leds
        return time_to_impact;
    }
    return -1;
}

void handle_beat_the_clock(void)
{
    static uint32_t last_tx = 0;
    if(get_index() == 0)
    {
        // Index 0 is master, there should only be one on the network
        if(millis() > last_tx + TIMEOUT_MS)
        {
            // Pick random value within the available NUM_DEVICES
            uint32_t device_to_turn_on = random(NUM_DEVICES);
            debug_print("Device chosen: ");
            debug_println(device_to_turn_on);

            if(device_to_turn_on == get_index())
            {
                // If self, watch for impact
                time_to_begin_sense = millis();
                time_to_end_sense = time_to_begin_sense + (TIMEOUT_MS - 1000); // 1 second less
                reset_int_status();
            }
            else
            {
                // Send command 
                esp_now_msg_t new_msg;
                new_msg.game = BEAT_THE_CLOCK;
                new_msg.msg_source = get_index();
                new_msg.msg_destination = device_to_turn_on;
                new_msg.payload_len = 4;
                uint32_t timeout = TIMEOUT_MS - 1000; // 1 second less
                memcpy(&new_msg.payload[0], &timeout, 4);
                send_msg(&new_msg);
            }
            msg_recv = false;
            last_tx = millis();
        }
        else
        {
            // Dont need to send a new message yet, but handle responses if present
            if(msg_recv && last_msg_recv.game == BEAT_THE_CLOCK && last_msg_recv.payload_len == 4)
            {
                uint32_t time_to_impact;
                memcpy(&time_to_impact, (uint8_t *) &last_msg_recv.payload[0], 4);
                debug_print("Remote Impact at: ");
                debug_print(time_to_impact);
                debug_print("ms on device ");
                debug_println(last_msg_recv.msg_source);
                msg_recv = false;
            }
        }
        
    }
    else
    {
        // Otherwise you are not master 
        if(msg_recv && last_msg_recv.game == BEAT_THE_CLOCK && last_msg_recv.payload_len == 4)
        {
            uint32_t timeout = 0;
            memcpy(&timeout, (uint8_t *) &last_msg_recv.payload[0], 4);
            time_to_begin_sense = millis();
            time_to_end_sense = time_to_begin_sense + timeout;
            reset_int_status();
            debug_print("Looking for impact with ");
            debug_print(timeout);
            debug_println("ms timeout");
            msg_recv = false;
        }
    }

    // Regardless, handle if you should be looking for impact
    int32_t impact_fb = wait_for_impact();
    if( impact_fb > 0)
    {
        debug_print("Local Impact at: ");
        debug_print(impact_fb);
        debug_println("ms");
        // Send response if not master
        if(get_index() != 0)
        {
            esp_now_msg_t new_msg;
            new_msg.game = BEAT_THE_CLOCK;
            new_msg.msg_source = get_index();
            new_msg.msg_destination = 0;
            new_msg.payload_len = 4;
            uint32_t time_to_impact = (uint32_t) impact_fb;
            memcpy(&new_msg.payload[0], &time_to_impact, 4);
            send_msg(&new_msg);
        }
    }
}

void handle_beat_the_clock_msg(esp_now_msg_t * msg)
{
    // This is within an ISR!!!
    // BE CAREFUL TO EXIT QUICKLY
    msg_recv = true;
    memcpy((uint8_t *) &last_msg_recv, msg, sizeof(esp_now_msg_t));
}