/*
 * Code created by Alexandre Lavoie, Tristan Viera, Justin De La Fosse, and Ryan Baker of team int kiwi.
 */

#include <Servo.h>

int kiwi = 0; // <3

// Constants
const int FIRST_LINE = 1;
const int MIN_DISTANCE = 1;

// Pin
const int triggerPin = 2;
const int echoPin = 3;
const int lMotorPin = 8;
const int rMotorPin = 12;
const int lLightSensor = 4;
const int rLightSensor = 5;

// Servos
Servo left_wheel, right_wheel;

// Light sensor variables.
bool left_light_sensor = false;
bool right_light_sensor = false;

// Distance sensor variables.
long distance_to_object = 0;
int current_line = FIRST_LINE;

// State variables.
bool is_at_end = false;
bool is_going_left = true;
bool is_changing_line = false;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lLightSensor, INPUT);
  pinMode(rLightSensor, INPUT);
  
  left_wheel.attach(lMotorPin);
  right_wheel.attach(rMotorPin);
}

void loop() {
  test_sensors();
  move();
}

long get_distance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(4);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(triggerPin, LOW);
  
  return (340.0 * (1.0 / 1000000.0) * (100.0) * (pulseIn(echoPin, HIGH) / 2.0));
}

void test_light_sensors(){
    left_light_sensor = digitalRead(lLightSensor);
    right_light_sensor = digitalRead(rLightSensor);
}


void move_forward(){
  if(!left_light_sensor&&!right_light_sensor){
    left_wheel.write(0);
    right_wheel.write(0);
  }else if(!left_light_sensor){
    left_wheel.write(0);
    right_wheel.write(0);
  }else if(!right_light_sensor){
    left_wheel.write(0);
    right_wheel.write(0);
  }else if(left_light_sensor&&right_light_sensor){
    is_at_end = true;
  }
}

void change_line(){
  if(current_line==1&&is_going_left){
    is_going_left=false;
  }else if(current_line == 4 && is_going_right){
    
  }
}

/*
 * Tests for the light sensor in the line following problem.
 */

void test_sensors(){
  test_light_sensors();
  distance_to_object = get_distance();
}

/*
 * Moves the robot according to the given sensor input.
 */

void move(){
  if(is_at_end){
    
  }else{
    if(distance_to_object<=MIN_DISTANCE){
      change_line();
    }else{
      move_forward();
    }
  }
}
