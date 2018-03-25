/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <string>
#include "string"

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;


// constants won't change. They're used here to set pin numbers:
const int buttonPin = 12;             // the number of the pushbutton pin
const int buzzPin =  4;
//const int ledPin =  13;             // the number of the LED pin
const String badlist[2] = {"B6:A7:D8:G9:U0:Y1", "B5:A7:D2:G9:R1:L8"};
const int numberOfBadGuys = 2;

// variables will change:
bool snooze = false;
bool signalWarning = false;


void setup() {
  pinMode(buzzPin, OUTPUT);           // initialize the buzzer pin as an output
  pinMode(buttonPin, INPUT);          // initialize the pushbutton pin as an input
  pinMode(LED_BUILTIN, OUTPUT);       // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);
  USE_SERIAL.begin(115200);

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

  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) { //waits until its connected
    HTTPClient http; // connects to it with http
    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    // this is where the connection happens
    http.begin("https://safe-house-7baf3.firebaseio.com/MAC.json", "b8 4f 40 70 0c 63 90 e0 07 e8 7d bd b4 11 d0 4a ea 9c 90 f6"); //HTTPS
    //http.begin("http://jillustrations.com/DHCP.html"); //HTTP
    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();                          // Getting http code and checks if it can land in the website

    // httpCode will be negative on error
    if (httpCode > 0) {                                 // if it exists
      // HTTP header has been send and Server response header has been handled
      //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {                    //if it's good
        String currentConnections = http.getString();    //payload is what were getting back from the website
        USE_SERIAL.println("payload:%d \n " + currentConnections);

        //if (signalWarning = false) {

        // Check if anything in array is also in badlist
        signalWarning = false;
        for (int i = 0; i < numberOfBadGuys; i++) {
          // if any values of badlist are found in currentConnection,
          // then change a boolean to true
          if (currentConnections.indexOf(badlist[i]) != -1) {
            USE_SERIAL.println("found badguy in currentConnections");
            signalWarning = true;
          }
        }

        // if badguy detected (signalWarning == true), AND snooze has not been set,
        if (signalWarning && !snooze) {
          USE_SERIAL.println("badguy found AND snooze NOT set");
          blinkLight("on");
          buzzer("on");
        } else if ( !signalWarning){
          USE_SERIAL.println("NO badguy found");
          blinkLight("off");
          buzzer("off");
        }

        // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
        if (digitalRead(buttonPin) == HIGH) {             // if snooze is pressed
          USE_SERIAL.println("**** Button was pressed ***");
          buzzer("off");                                  // turn off buzzer, turn off light
          blinkLight("off");
          snooze = true;
        }
      }
    } else {
      //USE_SERIAL.printf(“%s”, http.errorToString(httpCode).c_str());
      //USE_SERIAL.printf("hello %s", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  delay(4000);                        // how often we query the url
}

void blinkLight (String action) {
  if (action == "on") {
//    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    // but actually the LED is on; this is because
//    // it is active low on the ESP-01)
//    delay(1000);                      // Wait for a second
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    //delay(1000);
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  }
}

void buzzer (String action) {
  if (action == "on") {
    analogWrite(4, 512);
  } else {
    analogWrite(4, 0);
  }
}
