//variables instantiated
const int PorchPin = 11;
const int HallwayPin = 10;
const int ButtonPin = 9;


void setup()
{
  pinMode(PorchPin, OUTPUT);       //arduino pins which are connected to lights set to output
  pinMode(HallwayPin, OUTPUT);   

    pinMode(ButtonPin, INPUT_PULLUP);  //button pin set to input

  digitalWrite(PorchPin, LOW);   //initially lights are off as set to low
  digitalWrite(HallwayPin, LOW);
}


void bothLightsOn()
{
  digitalWrite(PorchPin, HIGH);     //turn both lights on by making output high
  digitalWrite(HallwayPin, HIGH);
}


void lightingAlgorithm()   //This is how lighting algorithm works
{
  bothLightsOn(); //calls the function

  delay(30000);  //delay for porch light
  digitalWrite(PorchPin, LOW);  //set it low

  delay(30000);  //further delay for hallway light
  digitalWrite(HallwayPin, LOW);  //set this one also low
}



void loop()     //keep checking if button is pressed
{
  if (digitalRead(ButtonPin) == LOW)    //if button is pressed
  {
    lightingAlgorithm();    //calls that function
  }
}