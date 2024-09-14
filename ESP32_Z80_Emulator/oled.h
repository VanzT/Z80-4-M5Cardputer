#pragma once

#include "globals.h"
#include <M5Unified.h>
#include "battery.h"

void displayBatteryLevel() {
    int batteryLevel = getBatteryPercentage();  // Get the battery percentage

    // Clear the area where the battery level is displayed by drawing a filled rectangle
    M5.Display.fillRect(10, 115, 160, 20, BLACK);  // Clear the area (adjust width/height as needed)

    M5.Display.setCursor(10, 115);                 // Move the cursor to the desired position
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(2);
    M5.Display.printf("Battery: %d%%", batteryLevel);  // Print the new battery level
}

void displayApModeInfo() {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setTextSize(2);
  M5.Display.println("Z80 for Cardputer");
  M5.Display.setCursor(10, 50);
  M5.Display.setTextColor(TFT_GREEN);
  M5.Display.println("SSID: Z80-AP");
  M5.Display.setCursor(10, 70);
  M5.Display.println("Pass: Z80-password");
}

void displayTelnetInfo() {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setTextSize(2);
  M5.Display.println("Z80 for Cardputer");
  M5.Display.setCursor(10, 50);
  M5.Display.println("TELNET TO: ");
  M5.Display.setCursor(10, 70);
  if (ipString.length() > 12) {
    M5.Display.setTextSize(2);  // Decrease font size if IP length is greater than 12
  } else {
    M5.Display.setTextSize(3);  // Default font size for IP address
  }
  if (APMode) {
    M5.Display.setTextColor(TFT_GREEN);
  } else {
    M5.Display.setTextColor(TFT_YELLOW);
  }
  M5.Display.println(ipString);
  displayBatteryLevel();
}

void blankScreen() {
  M5.Display.fillScreen(TFT_BLACK);
}