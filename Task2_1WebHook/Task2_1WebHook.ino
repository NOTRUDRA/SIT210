#include <WiFiNINA.h>
#include "ThingSpeak.h"
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include "secrets.h"

// Pull credentials from secrets.h
char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASS;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// assigning sensor info
DHT dht(3, DHT22); 
BH1750 lightMeter;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Wire.begin();
  lightMeter.begin();
  ThingSpeak.begin(client); 
}

float getTemp() {  //function for getting temperature
  return dht.readTemperature(); 
}

float getLight() {   //function for getting light
  return lightMeter.readLightLevel(); 
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {   //check if wifi is not connected
    Serial.print("Connecting to WiFi");
    while(WiFi.status() != WL_CONNECTED) {   //loop while not connected
      WiFi.begin(ssid, pass);    //restablish connection
      delay(5000);     
    } 
    Serial.println(" Connected.");
  }

  float temp = getTemp();  //takes value from above function
  float light = getLight();  //

  //if dht not working
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return; //exit code
  }

  //set fields for thingspeak
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, light);
  

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  //send request across to the thingSpeak server and stores error code
  if(x == 200){   //request successful
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code: " + String(x));
  }
  
  delay(30000);  //30 second delay
}


