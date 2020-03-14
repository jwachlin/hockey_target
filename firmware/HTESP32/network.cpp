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
 * @file network.cpp
 * @author Jacob Wachlin
 * @date 13 February 2020
 * @brief ESP32 based hockey target
 */

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>

#include <esp_wifi.h>
#include <esp_now.h>
#include <stdint.h>
#include <string.h>

#include "HTESP32.h"
#include "network.h"
#include "games.h"

#define WIFI_CHANNEL                        (1)

static uint8_t broadcast_mac[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static void handle_error(esp_err_t err)
{
  switch (err)
  {
    case ESP_ERR_ESPNOW_NOT_INIT:
      debug_println("Not init");
      break;

    case ESP_ERR_ESPNOW_ARG:
      debug_println("Argument invalid");
      break;

    case ESP_ERR_ESPNOW_INTERNAL:
      debug_println("Internal error");
      break;

    case ESP_ERR_ESPNOW_NO_MEM:
      debug_println("Out of memory");
      break;

    case ESP_ERR_ESPNOW_NOT_FOUND:
      debug_println("Peer is not found");
      break;

    case ESP_ERR_ESPNOW_IF:
      debug_println("Current WiFi interface doesn't match that of peer");
      break;

    default:
      break;
  }
}

static void msg_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    debug_println("Message received");
  if (len == sizeof(esp_now_msg_t))
  {
    esp_now_msg_t msg;
    memcpy(&msg, data, len);
    debug_println("Message correct length");

    
    if(msg.msg_destination == get_index())
    {
        game_handle_msg(&msg);
        debug_println("This msg is to me");
    }
  }
}

static void msg_send_cb(const uint8_t* mac, esp_now_send_status_t sendStatus)
{

  switch (sendStatus)
  {
    case ESP_NOW_SEND_SUCCESS:
      debug_println("Send success");
      break;

    case ESP_NOW_SEND_FAIL:
      debug_println("Send Failure");
      break;

    default:
      break;
  }
}

static void network_setup(void)
{
  //Puts ESP in STATION MODE
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0)
  {
    return;
  }

  esp_now_peer_info_t peer_info;
  peer_info.channel = WIFI_CHANNEL;
  memcpy(peer_info.peer_addr, broadcast_mac, 6);
  peer_info.ifidx = ESP_IF_WIFI_STA;
  peer_info.encrypt = false;
  esp_err_t status = esp_now_add_peer(&peer_info);
  if (ESP_OK != status)
  {
    debug_println("Could not add peer");
    handle_error(status);
  }

  // Set up callback
  status = esp_now_register_recv_cb(msg_recv_cb);
  if (ESP_OK != status)
  {
    debug_println("Could not register callback");
    handle_error(status);
  }

  status = esp_now_register_send_cb(msg_send_cb);
  if (ESP_OK != status)
  {
    debug_println("Could not register send callback");
    handle_error(status);
  }
}

void init_network(void)
{
    network_setup();
}

void send_msg(esp_now_msg_t * msg)
{
  // Pack
  uint16_t packet_size = sizeof(esp_now_msg_t);
  uint8_t msg_data[packet_size];
  memcpy(&msg_data[0], msg, sizeof(esp_now_msg_t));

  esp_err_t status = esp_now_send(broadcast_mac, msg_data, packet_size);
  if (ESP_OK != status)
  {
    debug_println("Error sending message");
    handle_error(status);
  }
}