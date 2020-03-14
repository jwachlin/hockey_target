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
 * @file network.h
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#ifndef NETWORK_h
#define NETWORK_h

typedef enum
{
    BEAT_THE_CLOCK = 0,
    KING_OF_THE_HILL = 1
} GAME_TYPE;

// Note: 250 byte limit
typedef struct __attribute__((packed)) esp_now_msg_t
{
  GAME_TYPE game;
  uint32_t msg_source;
  uint32_t msg_destination;
  uint8_t payload_len;
  uint8_t payload[32];
} esp_now_msg_t;

void init_network(void);
void send_msg(esp_now_msg_t * msg);

#endif