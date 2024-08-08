#include "globals.h"
#include <M5Unified.h>
#pragma once

//*********************************************************************************************
//****                                 Telnet task                                         ****
//*********************************************************************************************
void TelnetTask(void *parameter) {
  server.begin();
  server.setNoDelay(true);

  // Ensure Serial is available before printing
//  while (!Serial) {
//    vTaskDelay(10); // Wait for Serial to become available   <-- why?
//  }

  const char *startMsg = "Telnet Task Started:\n\r";
  const char *useMsg = "\n\r***Use 'telnet ";
  const char *endMsg = "' to connect***\n\r";

  // Use Serial.write for all parts of the message
  vTaskDelay(1000);
  Serial.write(startMsg, strlen(startMsg));
  vTaskDelay(1);
  Serial.write(useMsg, strlen(useMsg));
  vTaskDelay(1);
  Serial.write(WiFi.localIP().toString().c_str());
  vTaskDelay(1);
  Serial.write(endMsg, strlen(endMsg));
  vTaskDelay(3000);
  telnet_t = true;

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setTextSize(2);
  M5.Display.println("Z80 for Cardputer");
  M5.Display.setCursor(30, 40);
  M5.Display.println("TELNET TO: ");
  M5.Display.setCursor(30, 60);
  M5.Display.setTextColor(TFT_YELLOW);
  M5.Display.println(WiFi.localIP().toString().c_str());

  for (;;) {

    if (server.hasClient()) {
      serverClient = server.available();
      //if (serverClient) {
        Serial.print("\n\rNew Telnet client @ ");
        Serial.println(serverClient.remoteIP());
        //vTaskDelay(100);
        serverClient.write(255);  // IAC
        serverClient.write(251);  // WILL
        serverClient.write(1);    // ECHO
        //vTaskDelay(100);
        serverClient.write(255);  // IAC
        serverClient.write(251);  // WILL
        serverClient.write(3);    // suppress go ahead
        //vTaskDelay(100);
        serverClient.write(255);  // IAC
        serverClient.write(252);  // WONT
        serverClient.write(34);   // LINEMODE
        //vTaskDelay(100);
        serverClient.write(27);   //Print "esc"
        serverClient.print("c");  //Send esc c to reset screen
        //vTaskDelay(100);
        for (int i = 0; i < 11; i++) {
          serverClient.println(banner[i]);
          //vTaskDelay(1);
        };
        while (serverClient.available()) serverClient.read();  //Get rid of any garbage received
        //vTaskDelay(500);
        RUN = false;  //Force Z80 reboot
      //}
    }
    vTaskDelay(100);
  }
}
