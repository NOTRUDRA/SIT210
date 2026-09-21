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

// These flags tell the main loop that an interrupt has happened.
volatile bool pirTriggered = false;
volatile bool buttonTriggered = false;

volatile unsigned long lastPirInterrupt = 0;
volatile unsigned long lastButtonInterrupt = 0;

bool led1Active = false;
bool led2Active = false;

unsigned long led1StartTime = 0;
unsigned long led2StartTime = 0;


// Runs when the PIR detects motion.
void pirInterrupt() {

  unsigned long currentTime = millis();

  // Ignore repeated PIR signals within the cooldown period.
  if (currentTime - lastPirInterrupt >= PIR_COOLDOWN) {
    pirTriggered = true;
    lastPirInterrupt = currentTime;
  }
}


// Runs when the button is pressed.
void buttonInterrupt() {

  unsigned long currentTime = millis();

  // Prevent one button press from being detected multiple times.
  if (currentTime - lastButtonInterrupt >= BUTTON_COOLDOWN) {
    buttonTriggered = true;
    lastButtonInterrupt = currentTime;
  }
}


// Turns both LEDs on and records when they were switched on.
void turnLightsOn() {

  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);

  led1Active = true;
  led2Active = true;

  led1StartTime = millis();
  led2StartTime = millis();
}


// Checks for motion and then checks the light level.
void checkMotion() {

  if (pirTriggered == true) {

    pirTriggered = false;

    Serial.println("PIR: Motion detected.");

    float lux = lightMeter.readLightLevel();

    Serial.print("Light level: ");
    Serial.print(lux);
    Serial.println(" lux");

    // Turn the lights on only when motion is detected in darkness.
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


// Checks whether the button was pressed.
void checkButton() {

  if (buttonTriggered == true) {

    buttonTriggered = false;

    Serial.println("Button pressed.");

    turnLightsOn();

    Serial.println("Lights turned ON by button.");
  }
}


// Turns each LED off after its specified time.
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

  // INPUT_PULLUP keeps the button HIGH until it is pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

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

  // PIR triggers the interrupt when its signal changes from LOW to HIGH.
  attachInterrupt(
    digitalPinToInterrupt(PIR_PIN),
    pirInterrupt,
    RISING
  );

  // Button triggers the interrupt when its signal changes from HIGH to LOW.
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonInterrupt,
    FALLING
  );

  Serial.println("System ready.");
}


void loop() {

  checkMotion();
  checkButton();
  checkTimers();

  // Gives the system a short pause between checks.
  delay(50);
}
