#ifndef BATTERY_H
#define BATTERY_H

#include <M5Unified.h>  // Include M5Unified library

int getBatteryPercentage() {
    // Get battery percentage from M5Unified library
    return M5.Power.getBatteryLevel();  // This returns the battery percentage (0-100%)
}

void displayBatteryLevel() {
    int batteryLevel = getBatteryPercentage();  // Get the battery percentage

    // Clear the area where the battery level is displayed by drawing a filled rectangle
    M5.Display.fillRect(10, 115, 160, 20, BLACK);  // Clear the area (adjust width/height as needed)

    M5.Display.setCursor(10, 115);                 // Move the cursor to the desired position
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.printf("Battery: %d%%", batteryLevel);  // Print the new battery level
}

#endif  // BATTERY_H
