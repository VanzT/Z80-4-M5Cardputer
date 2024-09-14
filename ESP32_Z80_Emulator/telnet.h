#include "globals.h"
#include <M5Unified.h>
#include "oled.h"
#pragma once

//*********************************************************************************************
//****                                 Telnet task                                         ****
//*********************************************************************************************
void TelnetTask(void *parameter) {

  server.begin();
  server.setNoDelay(true);
  const char *startMsg = "Telnet Task Started:\n\r";
  const char *useMsg = "\n\r***Use 'telnet ";
  const char *endMsg = "' to connect***\n\r";

  // Get the local IP address (client mode)
  IPAddress localIP = WiFi.localIP();
  // Get the SoftAP IP address (access point mode)
  IPAddress apIP = WiFi.softAPIP();

  // Convert IP addresses to strings for display purposes
  ipString = localIP.toString();  //global
  String apString = apIP.toString(); //local

  // Check if either the local IP (client mode) or SoftAP IP (access point mode) is valid
  bool isValidIP = (localIP != IPAddress(0, 0, 0, 0) || apIP != IPAddress(0, 0, 0, 0)) && (ipString.length() > 0 || apString.length() > 0);
  bool APMode = (localIP == IPAddress(0, 0, 0, 0) && apIP != IPAddress(0, 0, 0, 0));
  if (APMode) {
    ipString = apString;  //so we display the AP assigned address on the screen instead of local WiFi addr.
  }

  // Use Serial.write for all parts of the message
  vTaskDelay(1000);
  Serial.write(startMsg, strlen(startMsg));
  vTaskDelay(1);
  Serial.write(useMsg, strlen(useMsg));
  vTaskDelay(1);
  if (isValidIP) {
    Serial.write(ipString.c_str());
  } else {
    Serial.write("not available");
  }
  vTaskDelay(1);
  Serial.write(endMsg, strlen(endMsg));
  vTaskDelay(1000);
  telnet_t = true;

  if (APMode) {
    displayApModeInfo();
    vTaskDelay(30000);
  }

  if (isValidIP) {
    displayTelnetInfo();
  } else {
    M5.Display.println("not available");
  }

  for (;;) {
    if (server.hasClient()) {
      telnetReady = false;
      serverClient = server.available();
        vTaskDelay(500); //might not be needed
        while (serverClient.available()) serverClient.read();  //Get rid of any garbage received
        vTaskDelay(500);  //might not be needed
        Serial.print("\n\rNew Telnet client @ ");
        Serial.println(serverClient.remoteIP());
        vTaskDelay(100);
        serverClient.write(255);  // IAC
        serverClient.write(251);  // WILL
        serverClient.write(1);    // ECHO
        vTaskDelay(100);
        serverClient.write(255);  // IAC
        serverClient.write(251);  // WILL
        serverClient.write(3);    // suppress go ahead
        vTaskDelay(100);
        serverClient.write(255);  // IAC
        serverClient.write(252);  // WONT
        serverClient.write(34);   // LINEMODE
        telnetReady = true;
        serverClient.write(27);   //Print "esc"
        serverClient.print("c");  //Send esc c to reset screen
        vTaskDelay(100);
        for (int i = 0; i < 11; i++) {
          serverClient.println(banner[i]);
          vTaskDelay(1);
        };
        RUN = false;  //Force Z80 reboot
      //}
    }
    vTaskDelay(100);
  }
}
