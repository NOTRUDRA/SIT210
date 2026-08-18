
#System Description-

My system is designed to help Linda enter her house safely when it is dark. When she comes home and presses the push button, both the porch light and hallway light turn on. The porch light stays on for 30 seconds and then turns off. After that, the hallway light stays on for another 30 seconds and then turns off. This gives Linda enough light to find her keys and enter the house safely.


#Items used-

Arduino Nano 33 IoT
Porch LED
Hallway LED
Push Button
Resistors
Breadboard
Jumper Wires



#Code description-

The code first sets the pins for the porch light, hallway light and push button. 
In the setup() function, the light pins are set as output and the button pin is set as input. Both lights are also set to LOW at the start, so they are OFF.
The bothLightsOn() function is used to turn both the porch and hallway lights ON. 
The lightingAlgorithm() function controls the main lighting process. It turns both lights ON, waits for 30 seconds and turns the porch light OFF. It then waits for another 30 seconds and turns the hallway light OFF.
The loop() function keeps checking if the button is pressed. When the button is pressed, it calls the lightingAlgorithm() function and starts the lighting process.
