#include <Wire.h>
#include <BH1750.h>

const int PIR_PIN = 2;
const int BUTTON_PIN = 11;

const int LED1_PIN = 6;
const int LED2_PIN = 7;

const float DARK_THRESHOLD = 50.0;

const unsigned long LED1_TIME = 30000;
const unsigned long LED2_TIME = 60000;

const unsigned long PIR_COOLDOWN = 2000;
const unsigned long BUTTON_COOLDOWN = 500;

BH1750 lightMeter;

volatile bool pirTriggered = false;
volatile bool buttonTriggered = false;

volatile unsigned long lastPirInterrupt = 0;
volatile unsigned long lastButtonInterrupt = 0;

bool led1Active = false;
bool led2Active = false;

unsigned long led1StartTime = 0;
unsigned long led2StartTime = 0;


void pirInterrupt() {

  unsigned long currentTime = millis();

  if (currentTime - lastPirInterrupt >= PIR_COOLDOWN) {
    pirTriggered = true;
    lastPirInterrupt = currentTime;
  }
}


void buttonInterrupt() {

  unsigned long currentTime = millis();

  if (currentTime - lastButtonInterrupt >= BUTTON_COOLDOWN) {
    buttonTriggered = true;
    lastButtonInterrupt = currentTime;
  }
}


void turnLightsOn() {

  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);

  led1Active = true;
  led2Active = true;

  led1StartTime = millis();
  led2StartTime = millis();
}


void checkMotion() {

  if (pirTriggered == true) {

    pirTriggered = false;

    Serial.println("PIR: Motion detected.");

    float lux = lightMeter.readLightLevel();

    Serial.print("Light level: ");
    Serial.print(lux);
    Serial.println(" lux");

    if (lux < DARK_THRESHOLD) {

      Serial.println("It is dark.");

      turnLightsOn();

      Serial.println("Lights turned ON by PIR.");

    } else {

      Serial.println("It is bright.");
      Serial.println("Lights not turned ON.");
    }
  }
}


void checkButton() {

  if (buttonTriggered == true) {

    buttonTriggered = false;

    Serial.println("Button pressed.");

    turnLightsOn();

    Serial.println("Lights turned ON by button.");
  }
}


void checkTimers() {

  if (led1Active &&
      millis() - led1StartTime >= LED1_TIME) {

    digitalWrite(LED1_PIN, LOW);
    led1Active = false;

    Serial.println("LED1 OFF after 30 seconds.");
  }


  if (led2Active &&
      millis() - led2StartTime >= LED2_TIME) {

    digitalWrite(LED2_PIN, LOW);
    led2Active = false;

    Serial.println("LED2 OFF after 60 seconds.");
  }
}


void setup() {

  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);       // button stays HIGH when released

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  Wire.begin();

  if (lightMeter.begin()) {
    Serial.println("BH1750 started.");
  } else {
    Serial.println("BH1750 not detected.");
  }

  attachInterrupt(
    digitalPinToInterrupt(PIR_PIN),
    pirInterrupt,
    RISING
  );                                      // PIR interrupt when signal rises

  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonInterrupt,
    FALLING
  );                                      // button interrupt when pressed

  Serial.println("System ready.");
}


void loop() {

  checkMotion();
  checkButton();
  checkTimers();

  delay(50);                              // small delay between checks
}
