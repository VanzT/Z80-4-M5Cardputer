#include "globals.h"
#pragma once

bool ledOn = false;  // Flag to track LED state
int ledToggleCounter = 0;  // Counter to control LED toggling
int toggleInterval = 10;  // Adjust this value to control blinking speed

// Variables for biorhythm-like LED control
int redValue = 0, greenValue = 0, blueValue = 0;
int redIncrement = 1, greenIncrement = 2, blueIncrement = 3;

// Function to update the LED colors in a psudeo biorhythm pattern
void updateLedColor() {
  redValue += redIncrement;
  greenValue += greenIncrement;
  blueValue += blueIncrement;

  // Handle wraparound for 0-255 range
  if (redValue > 255 || redValue < 0) redIncrement = -redIncrement;
  if (greenValue > 255 || greenValue < 0) greenIncrement = -greenIncrement;
  if (blueValue > 255 || blueValue < 0) blueIncrement = -blueIncrement;

  leds[0] = CRGB(redValue, greenValue, blueValue);
  FastLED.show();
}

// Serial input and output buffer task
void serialTask(void *parameter) {
  char c;
  Serial.write("\n\rSerial I/O Task Started\n\r");
  vTaskDelay(1);
  serial_t = true;

  for (;;) {
    // Check for chars to be sent
    while (txOutPtr != txInPtr) {
      Serial.write(txBuf[txOutPtr]);  // Send char to console

      if (serverClient.connected() && telnetReady) {
        if (useLED) {
          ledToggleCounter++;
          // Toggle LED every 'toggleInterval' characters
          if (ledToggleCounter >= toggleInterval) {
            if (ledOn) {
              leds[0] = CRGB::Black;  // Turn LED off
              FastLED.show();
            } else {
              updateLedColor();  // Turn LED on
            }
            ledOn = !ledOn;  // Toggle the LED state
            ledToggleCounter = 0;  // Reset the counter
          }
          serverClient.write(txBuf[txOutPtr]);  // Send via Telnet if client connected
        } else {
          serverClient.write(txBuf[txOutPtr]);  // Send via Telnet if client connected
        }
      }
      txOutPtr++;  // Increment Output buffer pointer
      if (txOutPtr == sizeof(txBuf)) txOutPtr = 0;  // Wrap around circular buffer
      vTaskDelay(1);
    }

    // Ensure the LED is turned off when no more chars are being sent
    if (useLED && ledOn) {
      leds[0] = CRGB::Black;  // Turn LED off
      FastLED.show();
      ledOn = false;  // Reset the flag
    }

    // Check for Received chars from Serial
    while (Serial.available()) {
      rxBuf[rxInPtr] = Serial.read();
      rxInPtr++;
      if (rxInPtr == sizeof(rxBuf)) rxInPtr = 0;
      vTaskDelay(1);
    }

    // Check for Received chars from Telnet
    while (serverClient.available()) {
      c = serverClient.read();
      if (c == '\r' && serverClient.peek() == '\n') serverClient.read();
      if (c == 127) c = 8;
      rxBuf[rxInPtr] = c;
      rxInPtr++;
      if (rxInPtr == sizeof(rxBuf)) rxInPtr = 0;
    }

    // Handle virtual UART register
    if (rxOutPtr != rxInPtr && bitRead(pIn[UART_LSR], 0) == 0) {
      pIn[UART_PORT] = rxBuf[rxOutPtr];
      rxOutPtr++;
      if (rxOutPtr == sizeof(rxBuf)) rxOutPtr = 0;
      bitWrite(pIn[UART_LSR], 0, 1);
    }

    vTaskDelay(1);
  }
}

// Print string to output buffer, will send to serial and telnet if connected
void outString(char buf[]) {
  int i = 0;
  while (buf[i] > 0) {
    txBuf[txInPtr] = buf[i];
    txInPtr++;
    i++;
    if (txInPtr == sizeof(txBuf)) txInPtr = 0;
  }
}
