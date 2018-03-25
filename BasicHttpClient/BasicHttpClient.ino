/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <string>
#include "string"
//#include <WiFiClientSecure.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial



ESP8266WiFiMulti WiFiMulti;





bool signalWarning = false;

void setup() {


  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);
  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Puppet Guest", "argon4949");



}

void loop() {


  String badlist[2] = {"B6:A7:D8:G9:U0:Y1", "B5:A7:D2:G9:R1:L8"};
  int numberOfBadGuys = 2;
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) { //waits until its connected

    HTTPClient http; // connects to it with http

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin("https://safe-house-7baf3.firebaseio.com/MAC.json", "b8 4f 40 70 0c 63 90 e0 07 e8 7d bd b4 11 d0 4a ea 9c 90 f6"); //HTTPS
    //http.begin("http://jillustrations.com/DHCP.html"); //HTTP this is where the connection happens
    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();  // Getting http code and checks if it can land in the website

    // httpCode will be negative on error
    if (httpCode > 0) { // if it exists
      // HTTP header has been send and Server response header has been handled
      ////USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) { //if it's good
        String payload = http.getString(); //payload is what were getting back from the website

        USE_SERIAL.println("payload:%d \n " + payload);
        //TODO: parse payload
        //   String currentConnections[2] = payload.body();
        String currentConnections = payload;
        //USE_SERIAL.println(currentConnections[0]);


        //if anything in array is also in badlist. (double for loop)

        //if (signalWarning = false) {
        for (int i = 0; i < numberOfBadGuys; i++) {
          
            // if i in badlist
            //then change a boolean to true
            if (currentConnections.indexOf(badlist[i]) != -1) { //contains (currentConnections,(badList[i])

              
              
              USE_SERIAL.println("something cool happen");
              signalWarning = true;
            
              digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
              // but actually the LED is on; this is because
              // it is active low on the ESP-01)
              delay(1000);                      // Wait for a second
              digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
              delay(1000);
            }
          
        }
        //}






      }
    }
    else {
      //USE_SERIAL.printf(“%s”, http.errorToString(httpCode).c_str());
      //USE_SERIAL.printf("hello %s", http.errorToString(httpCode).c_str());

    }

    http.end();
  }

  delay(4000);
}

//char StrContains(char *str, char *sfind)
//{
//    char found = 0;
//    char index = 0;
//    char len;
//
//    len = strlen(str);
//    
//    if (strlen(sfind) > len) {
//        return 0;
//    }
//    while (index < len) {
//        if (str[index] == sfind[found]) {
//            found++;
//            if (strlen(sfind) == found) {
//                return 1;
//            }
//        }
//        else {
//            found = 0;
//        }
//        index++;
//    }
//
//    return 0;
//}


