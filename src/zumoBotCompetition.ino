#include <Wire.h>
#include <ZumoShield.h>


#define LED 13


#define QTR_THRESHOLD  1500 // microseconds


#define REVERSE_SPEED     400 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  500 // ms
#define TURN_DURATION     400 // ms


ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12


#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];


ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


//Ultrasonic sensor
const int trigPin = 0;
const int echoPin = 1;
const int chargeDistance = 15; //in cm
int duration;
int distance;




void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);


  // play audible countdown
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}


void setup()
{
  Serial.begin(9600);


  pinMode(LED, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  waitForButtonAndCountDown();
}


void loop()
{
  if (button.isPressed())
  {
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();
  }



//Ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  // motors.setSpeeds(TURN_SPEED, TURN_SPEED); //Parameters: left wheels, rifght wheels

  sensors.read(sensor_values);

  if (distance <= 30 && distance >0){
      motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[0] < QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION+ random(100, 251));
    motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[5] < QTR_THRESHOLD)
  {
      // if rightmost sensor detects line, reverse and turn to the left
      motors.setSpeeds(REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(-TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION + random(200, 450));
      motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
    }
    else
    {
      // otherwise, go straight
      // motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
        motors.setSpeeds(TURN_SPEED, TURN_SPEED); //Parameters: left wheels, rifght wheels

   
    }
  }


  // motors.setSpeeds(FORWARD_SPEED, -FORWARD_SPEED);   backwards
 // motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);   forward



