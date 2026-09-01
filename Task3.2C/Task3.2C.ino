#include <WiFiNINA.h>
#include <PubSubClient.h>


//variables instantiated
const char* ssid = "Airtel_Only Work";
const char* password = "Revenge#2231";

const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;

const char* myName = "Rudra";

const int PorchPin = 4;
const int HallwayPin = 5;
const int TrigPin = 9;
const int EchoPin = 10;


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


long getDistance()
{
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(TrigPin, HIGH);       //send ultrasonic signal
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  long duration = pulseIn(EchoPin, HIGH);    //measure the time taken for the echo

  long distance = duration * 0.034 / 2;      //convert time into distance

  return distance;
}


void bothLightsOn()
{
  digitalWrite(PorchPin, HIGH);       //turn both lights on by making output high
  digitalWrite(HallwayPin, HIGH);
}


void bothLightsOff()
{
  digitalWrite(PorchPin, LOW);        //turn both lights off by making output low
  digitalWrite(HallwayPin, LOW);
}


void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message received on ");
  Serial.print(topic);
  Serial.print(": ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();

  if (strcmp(topic, "ES/Wave") == 0)
  {
    bothLightsOn();                   //turn both lights on when wave message is received
  }

  if (strcmp(topic, "ES/Pat") == 0)
  {
    bothLightsOff();                  //turn both lights off when pat message is received
  }
}


void reconnectMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.print("Connecting to MQTT...");

    String clientID = "ArduinoClient-";
    clientID += String(random(0xffff), HEX);

    if (mqttClient.connect(clientID.c_str()))
    {
      Serial.println("connected");

      mqttClient.subscribe("ES/Wave");       //subscribe to wave topic
      mqttClient.subscribe("ES/Pat");        //subscribe to pat topic

      Serial.println("Subscribed to ES/Wave");
      Serial.println("Subscribed to ES/Pat");
    }
    else
    {
      Serial.print("Failed, rc=");
      Serial.println(mqttClient.state());

      delay(5000);
    }
  }
}


void setup()
{
  Serial.begin(9600);

  pinMode(PorchPin, OUTPUT);           //arduino pins which are connected to lights set to output
  pinMode(HallwayPin, OUTPUT);

  pinMode(TrigPin, OUTPUT);            //trigger pin set to output
  pinMode(EchoPin, INPUT);             //echo pin set to input

  digitalWrite(PorchPin, LOW);         //initially lights are off as set to low
  digitalWrite(HallwayPin, LOW);

  //connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.begin(ssid, password) != WL_CONNECTED)
  {
    Serial.print(".");
    delay(3000);
  }

  Serial.println();
  Serial.println("Wi-Fi connected");

  mqttClient.setServer(mqttServer, mqttPort);    //set MQTT broker and port
  mqttClient.setCallback(mqttCallback);           //set function for received messages
}


void loop()
{
  if (!mqttClient.connected())             //check if MQTT is connected
  {
    reconnectMQTT();
  }

  mqttClient.loop();                       //keep checking for MQTT messages

  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  if (distance >= 10 && distance <= 30)       //if hand is detected between 10 and 30 cm
  {
    Serial.println("WAVE DETECTED");

    mqttClient.publish("ES/Wave", myName);    //publish name to wave topic

    delay(2000);
  }


  if (distance > 0 && distance < 10)           //if hand is detected closer than 10 cm
  {
    Serial.println("PAT DETECTED");

    mqttClient.publish("ES/Pat", myName);     //publish name to pat topic

    delay(2000);
  }

  delay(100);
}
