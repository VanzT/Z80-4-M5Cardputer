#pragma once

#include <WiFi.h>
#include <ArduinoOTA.h>
#include "FS.h"
#include "SD.h"
#include "disk.h"

// Function to read Wi-Fi credentials from creds.txt
bool readCredentials(String &ssid, String &password) {
  // Open the creds.txt file
  File file = SD.open("/z80/creds.txt", FILE_READ);
  if (!file) {
    Serial.println("Failed to open creds.txt on SD card");
    return false;
  }

  // Read the file line by line
  String line;
  while (file.available()) {
    line = file.readStringUntil('\n');
    line.trim();  // Remove any leading/trailing whitespace

    // Parse SSID and PASSWORD
    if (line.startsWith("mySSID =")) {
      ssid = line.substring(line.indexOf('=') + 1);
      ssid.trim();
    } else if (line.startsWith("myPASSWORD =")) {
      password = line.substring(line.indexOf('=') + 1);
      password.trim();
    }
  }

  file.close();

  if (ssid.length() > 0 && password.length() > 0) {
    return true;
  } else {
    Serial.println("Invalid credentials format in creds.txt");
    return false;
  }
}

// OTA Task
void OTAtask(void *parameter) {
  String ssid, password;

  // Mount SD card
  // sdSPI.begin(SCK, MISO, MOSI, SS); //should already be mounted in setup()
  if (!SD.begin(SS, sdSPI)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  // Read Wi-Fi credentials from the SD card
  if (!readCredentials(ssid, password)) {
    Serial.println("Using default credentials");
    ssid = "defaultSSID";
    password = "defaultPASSWORD";
  }

  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid.c_str(), password.c_str());
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE); // required to set hostname properly
  WiFi.setHostname(hostName);
  ArduinoOTA.setHostname(hostName);

  // Set a timeout for Wi-Fi connection (e.g., 10 seconds)
  unsigned long startAttemptTime = millis();
  bool connected = false;

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nWiFi Connected - IP address: ");
    Serial.println(WiFi.localIP());

    ArduinoOTA
        .onStart([]() {
          String type;
          if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
          else  // U_SPIFFS
            type = "filesystem";

          Serial.println("\nStart updating " + type);
        })
        .onEnd([]() {
          Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
          Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
          Serial.printf("Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
          else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
          else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
          else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
          else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
        });

    ArduinoOTA.begin();
    Serial.println("\nOTA update Service Started\n");
    ota_t = true;
    connected = true;
  } 

  if (!connected) {
    // If Wi-Fi connection fails, set up an access point
    Serial.println("\nConnection Failed! Setting up Access Point...");
    WiFi.softAP("Z80-AP", "Z80-password");  // Customize AP credentials as needed
    IPAddress IP = WiFi.softAPIP();
    Serial.print("Access Point IP address: ");
    Serial.println(IP);
    ArduinoOTA.begin();
    ota_t = true;
  }

  // OTA task loop
  while (1) {
    ArduinoOTA.handle();
    vTaskDelay(10);
  }
}
