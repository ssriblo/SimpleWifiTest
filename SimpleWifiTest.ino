#include <Arduino.h>
#include <stdint.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SECRET_SSID_DEFAULT       ""
#define SECRET_PASSWORD_DEFAULT   "" 

wl_status_t wifiStatus;
String wifiTag[] = {"WL_IDLE_STATUS", "WL_NO_SSID_AVAIL", "WL_SCAN_COMPLETED", "WL_CONNECTED", "WL_CONNECT_FAILED", "WL_CONNECTION_LOST", "WL_DISCONNECTED"};

void setup(){
    Serial.begin(115200);
    delay(500);
    Serial.println("*********************** SETUP START ***********************");
    WiFi.begin(SECRET_SSID_DEFAULT, SECRET_PASSWORD_DEFAULT);
    // delay(3000); // Wait 3 Seconds, WL_DISCONNECTED is present until new connect!

    wifiStatus = WiFi.status();
    Serial.println(wifiTag[wifiStatus]);
    while(WiFi.status() != WL_CONNECTED ) {
        delay(500);
        wifiStatus = WiFi.status();
        Serial.print(wifiTag[wifiStatus]);
        Serial.print("\t");
    }
    Serial.println();
    Serial.println("\n*********************** SETUP END ***********************");
}

#define LOOP_DELAY_MS           10000 
#define RECONNECT_MS            60*1000
#define RECONNECT_LOOP_TICK     RECONNECT_MS/LOOP_DELAY_MS

void loop(){
    static int new_reconnect = 0;
    static int heartBeatNum = 0;

    if(WiFi.status() != WL_CONNECTED ) {
        if(heartBeatNum > new_reconnect){
            new_reconnect = heartBeatNum + RECONNECT_LOOP_TICK;
            Serial.println("**** reconnect *****");       
#define RECONNECT
#ifdef RECONNECT        
            WiFi.reconnect();
#else        
            WiFi.disconnect(); // works too, but some slowly
            WiFi.begin(SECRET_SSID_DEFAULT, SECRET_PASSWORD_DEFAULT);
#endif
        }
    }
    Serial.print(new_reconnect);       
    Serial.print("  ");       
    Serial.print(wifiTag[WiFi.status()]);       
    Serial.print("  HeartBeatNum=");
    Serial.println(heartBeatNum++);
    delay(LOOP_DELAY_MS);
}