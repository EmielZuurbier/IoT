// Includes
#include <EIoTCloudRestApi.h>
#include <EIoTCloudRestApiConfig.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

// Define pins and leds
int pir = D0;
int ldr = A0;
int led = LED_BUILTIN;
int pirState = LOW;
int val = 0;
int light = 0;

// Set wifi credentials
// const char* ssid = "Zy_private_Wolken";
// const char* password = "YNKYYCJF37HF7";

const char* ssid = "OnePlus2";
const char* password = "137946825";
WiFiClient client;

// Create EIoT credentials
EIoTCloudRestApi eiotcloud;
#define AP_SSID "OnePlus2"
#define AP_PASSWORD "137946825"
#define INSTANCE_ID_A "57051a17c943a0661cf314c0/TdvbAOxZJW2F6EW2"
#define INSTANCE_ID_B "57051a17c943a0661cf314c0/0hWq4lTIkhK4gQZh"

// ThingSpeak credentials
unsigned long CHANNEL_ID  = 106349;
const char * API_KEY = "W1D4UB0DV3HHI5NP";
#define AUTHOR_ID "MiloSaurus"

// Setup
void setup() {
  Serial.begin(9600);
   
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected with ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  ThingSpeak.begin(client);
   //eiotcloud.begin();
}

// Loop
void loop() {

  light = analogRead(ldr);
  val = digitalRead(pir);
    if (val == HIGH) {
      if (pirState == LOW) {
        digitalWrite(led, HIGH);
        Serial.println("Motion detected!");
        ThingSpeak.writeField(CHANNEL_ID, 1, val, API_KEY);
        eiotcloud.sendParameter(INSTANCE_ID_A, 1);
        // LDR
        Serial.println(light);
        delay(1000);
        eiotcloud.sendParameter(INSTANCE_ID_B, light);
        ThingSpeak.writeField(CHANNEL_ID, 2, light, API_KEY);
        pirState = HIGH;
        // ThingSpeak only takes updates per 15 seconds
      }
    } else {
      if (pirState == HIGH) {
        digitalWrite(led, LOW);
        Serial.println("Motion ended");
        ThingSpeak.writeField(CHANNEL_ID, 1, val, API_KEY);
        eiotcloud.sendParameter(INSTANCE_ID_A, 0);
        // LDR
        delay(1000);
        Serial.println(light);
        eiotcloud.sendParameter(INSTANCE_ID_B, light);
        ThingSpeak.writeField(CHANNEL_ID, 2, light, API_KEY);
        pirState = LOW;
        // ThingSpeak only takes updates per 15 seconds
      }
    }
}
