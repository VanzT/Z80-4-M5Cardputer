#ifndef BATTERY_H
#define BATTERY_H

#include <M5Unified.h>  // Include M5Unified library

int getBatteryPercentage() {
    // Get battery percentage from M5Unified library
    return M5.Power.getBatteryLevel();  // This returns the battery percentage (0-100%)
}

#endif  // BATTERY_H
