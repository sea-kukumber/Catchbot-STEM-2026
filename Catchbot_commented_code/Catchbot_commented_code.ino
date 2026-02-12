/*
catchBOT code in C++
Code by M. Steele
February 2026
*/
//Libraries needed
#include <math.h>         //Download math librabry. We are just using this for the value of pi
#include <AccelStepper.h> //Download the stepper library

//Time
unsigned long startTime; //Unsigned longs are variables with the ability to hold up to 32 bits of storage. Unsigned longs won't store negative numbers.
unsigned long t1;  
unsigned long t2;
float deltaTime; //Floats are variables with the ability to hold up to 32 bits of storage. Floats can store positive, negative numbers and decimal places.

//Speed-o-meter
int sensor1 = 6;    //Define first IR sensor's pin as pin 6.
int sensor2 = 5;    //Define second IR sensor's pin as pin 5.

bool trip = false;    //Define the trip variable as false (a bool is a variable that is either true or false).

//Define pin connections & motor's steps per revolution
#define STEP_PIN 2
#define DIR_PIN 3
AccelStepper stepper(STEP_PIN, DIR_PIN);

//Stepper motor
float r = 0.02;  //In meters. This is the radius of the stepper's axel.
float rotations;
float x;      //In meters. This will be the position where the marble will land.

void setup() {
  //The two IR sensors are our inputs.
  pinMode(sensor2, INPUT);  
  pinMode(sensor1, INPUT);

//This is the required setup for the stepper.
  stepper.setMaxSpeed(4000); //Define the MaxSpeed variable and set it to 4000
  stepper.setAcceleration(500); //Define the Acceleration variable and set it to 500

  Serial.begin(9600);     //Start serial moniter
}

void loop() {

  //Read the state of the IR sensors.
  int state2 = digitalRead(sensor2);
  int state1 = digitalRead(sensor1);

    //This is the code for the speed-o-meter, specifically to measure the change in time
  if ((state1 == LOW) & !trip) {    //If the first beam has been triggered and the trip variable is false.
    Serial.println("point 1 hit");  //Print "point 1 hit" on the serial moniter
    t1 = millis();    //Measure the time when the beam was triggered in milliseconds
    trip = true;    //Make the bool true. This allows the second beam to be triggered
  } else if ((state2 == LOW) & trip) {  //If the second beam has been triggered and the trip variable is true
    Serial.println("point 2 hit");    //Print "point 2 hit" on the serial moniter
    t2 = millis();    //Measure the time at which the second beam was triggered
    deltaTime = (t2 - t1) / 1000.000;   //This is to measure the change in time between when both sensors have been triggered. Since millis records in milliseconds, by diving everything by 1000, it converts the time to seconds.
 
  //Calculate the speed. The distance between the two sensors is 38 cm or 0.38 meters.
  //Another thing to note is printing all these values on the Serial moniter will slow the whole process down. It is recommened to get ride of these if time is of the essence.
   Serial.println(deltaTime);
    Serial.print(0.038/deltaTime, 2); //This will print the speed on the serial moniter. The formula for speed is distance/time
    Serial.println("m/sec");    //The unit for speed. This is probably the most practical unit for this scale of movement.
    Serial.print((0.038/deltaTime)*3.6, 2);    
    Serial.println(" km/h");    //The calibration device used here was recording outputs in km/h, but this will not be very practical for the scale of this project.

  //Move the catcher
    x = 0.53 - ((0.038 / deltaTime) * sqrt(2 * 0.73 / 9.8));  // (Length of track) - (speed)*root(2*height/gravity)   This is the formula for calculating distance travelled by the marble
//In the line above, we are calculating the distance the robot needs to travel. Our track has a length of 53 cm or 0.53 meters.
    Serial.print(x);    //Print the position where the marble will land.
    Serial.println(" m");   //This is calculated in meters.

    rotations = x / (2 * M_PI * r);   //This is the amount of rotations required by the motor to move to the correct distance.

    //To control the amount of movement needed by the stepper and at what speed
    for (int i = 0; i <= 407*rotations; i++) {
      stepper.moveTo (100);   //This is telling the stepper motor to move 100 steps.
      stepper.setSpeed(stepper.maxSpeed());     //Set the stepper speed to the max speed. This was established in the void setup.
      stepper.run();    //Let's the stepper move.
       
      stepper.moveTo (-100);
      stepper.setSpeed(stepper.maxSpeed());
      stepper.run();
  }
    trip = false;   //Reset the trip variable to false. This allows repeated tests one after another
  }
}

