/*
    note: need add library Adafruit_BMP280 from library manage
    Github: https://github.com/adafruit/Adafruit_BMP280_Library
*/

#include <Adafruit_BMP280.h>
#include <M5StickC.h>
#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "Adafruit_Sensor.h"
#include "DHT12.h"
#include "bmm150.h"
#include "bmm150_defs.h"
WiFiMulti wifiMulti;
DHT12 dht12;
BMM150 bmm = BMM150();
bmm150_mag_data value_offset;
Adafruit_BMP280 bme;
int LedPin = 10;

const char* ID = "";    //wifi ID
const char* Password = "";    //wifi password
String StateURL = String("http://xuchenzheng098.atwebpages.com/device_feedback/html_php_test/setLED.php");
String DataURL = String("http://xuchenzheng098.atwebpages.com/device_feedback/html_php_test/action_page.php?Temp=");

//-------------------------------------------------------------------
void calibrate(uint32_t timeout) {
    int16_t value_x_min = 0;
    int16_t value_x_max = 0;
    int16_t value_y_min = 0;
    int16_t value_y_max = 0;
    int16_t value_z_min = 0;
    int16_t value_z_max = 0;
    uint32_t timeStart  = 0;

    bmm.read_mag_data();
    value_x_min = bmm.raw_mag_data.raw_datax;
    value_x_max = bmm.raw_mag_data.raw_datax;
    value_y_min = bmm.raw_mag_data.raw_datay;
    value_y_max = bmm.raw_mag_data.raw_datay;
    value_z_min = bmm.raw_mag_data.raw_dataz;
    value_z_max = bmm.raw_mag_data.raw_dataz;
    delay(100);

    timeStart = millis();

    while ((millis() - timeStart) < timeout) {
        bmm.read_mag_data();

        /* Update x-Axis max/min value */
        if (value_x_min > bmm.raw_mag_data.raw_datax) {
            value_x_min = bmm.raw_mag_data.raw_datax;

        } else if (value_x_max < bmm.raw_mag_data.raw_datax) {
            value_x_max = bmm.raw_mag_data.raw_datax;
        }

        /* Update y-Axis max/min value */
        if (value_y_min > bmm.raw_mag_data.raw_datay) {
            value_y_min = bmm.raw_mag_data.raw_datay;

        } else if (value_y_max < bmm.raw_mag_data.raw_datay) {
            value_y_max = bmm.raw_mag_data.raw_datay;
        }

        /* Update z-Axis max/min value */
        if (value_z_min > bmm.raw_mag_data.raw_dataz) {
            value_z_min = bmm.raw_mag_data.raw_dataz;

        } else if (value_z_max < bmm.raw_mag_data.raw_dataz) {
            value_z_max = bmm.raw_mag_data.raw_dataz;
        }

        Serial.print(".");
        delay(1);
    }

    value_offset.x = value_x_min + (value_x_max - value_x_min) / 2;
    value_offset.y = value_y_min + (value_y_max - value_y_min) / 2;
    value_offset.z = value_z_min + (value_z_max - value_z_min) / 2;
}
void setup() {

    wifiMulti.addAP(ID, Password);
    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, HIGH);

    //------------------------------------------------------------
    M5.begin();
    Wire.begin(0, 26);
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 2);
    M5.Lcd.println("ENV TEST");
    pinMode(M5_BUTTON_HOME, INPUT);

    if (bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
        Serial.println("Chip ID can not read!");
        while (1)
            ;
    } else {
        Serial.println("Initialize done!");
    }
    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
        while (1)
            ;
    }
    calibrate(10);
    Serial.print("\n\rCalibrate done..");
}
uint8_t setup_flag = 1;
void loop() {
    // put your main code here, to run repeatedly:
    float tmp = dht12.readTemperature();
    float hum = dht12.readHumidity();
    M5.Lcd.setCursor(0, 20, 2);
    M5.Lcd.printf("Temp: %2.1f Humi: %2.0f%%", tmp, hum);

    bmm150_mag_data value;
    bmm.read_mag_data();

    value.x = bmm.raw_mag_data.raw_datax - value_offset.x;
    value.y = bmm.raw_mag_data.raw_datay - value_offset.y;
    value.z = bmm.raw_mag_data.raw_dataz - value_offset.z;

    float xyHeading = atan2(value.x, value.y);
    float zxHeading = atan2(value.z, value.x);
    float heading   = xyHeading;

    if (heading < 0) heading += 2 * PI;
    if (heading > 2 * PI) heading -= 2 * PI;
    float headingDegrees   = heading * 180 / M_PI;
    float xyHeadingDegrees = xyHeading * 180 / M_PI;
    float zxHeadingDegrees = zxHeading * 180 / M_PI;

    Serial.print("Heading: ");
    Serial.println(headingDegrees);
    Serial.print("xyHeadingDegrees: ");
    Serial.println(xyHeadingDegrees);
    Serial.print("zxHeadingDegrees: ");
    Serial.println(zxHeadingDegrees);
    M5.Lcd.setCursor(0, 40, 2);
    M5.Lcd.printf("headingDegrees: %2.1f", headingDegrees);

    float pressure = bme.readPressure();
    M5.Lcd.setCursor(0, 60, 2);
    M5.Lcd.printf("pressure: %2.1f", pressure);
    delay(100);

    if (!setup_flag) {
        setup_flag = 1;

        if (bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
            Serial.println("Chip ID can not read!");
            while (1)
                ;
        } else {
            Serial.println("Initialize done!");
        }
        if (!bme.begin(0x76)) {
            Serial.println(
                "Could not find a valid BMP280 sensor, check wiring!");
            while (1)
                ;
        }
        calibrate(10);
        Serial.print("\n\rCalibrate done..");
    }

    if (digitalRead(M5_BUTTON_HOME) == LOW) {
        setup_flag = 0;
        while (digitalRead(M5_BUTTON_HOME) == LOW)
            ;
    }

    //------------------------------------------------------------------------------
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        String url = DataURL + String(tmp) + String("&Humi=") + String(hum) + String("&headingDegrees=") + String(headingDegrees) + String("&pressure=") + String(pressure);
        http.begin(url); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        
        long int t1 = millis();
        long int t2 = millis();
        while((t2-t1)<3000){
          http.begin(StateURL);
    
          int httpCodeLed = http.GET();
          if(httpCodeLed == HTTP_CODE_OK) {
            String LedState = http.getString();
            Serial.println(LedState);
            if(LedState == "On"){
              digitalWrite(LedPin, LOW);
            }
           else if(LedState == "Off"){
             digitalWrite(LedPin, HIGH);
            }
           t2 = millis();
         }
          http.end();
       }        
    }
}
