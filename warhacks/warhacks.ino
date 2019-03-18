/*
 * Code created by Alexandre Lavoie, Tristan Viera, Justin De La Fosse, and Ryan Baker of team int kiwi at Warhacks 2019.
 */

#include <Servo.h> // Needed for the servos.

int kiwi = 0; // N.P - <3

// Constants for calibration.
const int MAX_SPEED = 15; // Max motor speeds.
const float LEFT_MOTOR_CONSTANT = 1; // Speed constant of left motor.
const float RIGHT_MOTOR_CONSTANT = -1; // Speed constant of right motor.
const float LEFT_MOTOR_ZERO = 87; // Zero constant of left motor.
const float RIGHT_MOTOR_ZERO = 87; // Zero constant of right motor.
const float DELAY = 10; // Delay on every loop to prevent overflowing the sensors.
const float LIGHT_THRESHOLD = 200; // Threshold for light sensors. 0-1023 : ~<50  White, ~>200  Black.

// Pin inputs.
const int lMotorPin = 8;
const int rMotorPin = 9;
const int lLightSensor = A0;
const int rLightSensor = A1;

// Servos.
Servo left_wheel, right_wheel;

// Light Sensors.
bool left_light_sensor = false;
bool right_light_sensor = false;

void setup() {
  pinMode(lLightSensor, INPUT);
  pinMode(rLightSensor, INPUT);
  
  left_wheel.attach(lMotorPin);
  right_wheel.attach(rMotorPin);
}

void loop() {
  test_light_sensors();
  follow_line_two_sensors();
  delay(DELAY);
}

/*
 * Tests for light on left and right light sensor greater than light threshold.
 */

void test_light_sensors(){
    left_light_sensor = analogRead(lLightSensor)>LIGHT_THRESHOLD; 
    right_light_sensor = analogRead(rLightSensor)>LIGHT_THRESHOLD;
}

/*
 * Follows a line according to the given left and right sensor.
 */

void follow_line_two_sensors(){
  if(left_light_sensor){ // If senses black line on left sensor.
    left_wheel.write(LEFT_MOTOR_ZERO);
    right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
  }else if(right_light_sensor){ // If senses black line on right sensor.
    left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
    right_wheel.write(RIGHT_MOTOR_ZERO);
  }else{
    left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
    right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
  }
}
