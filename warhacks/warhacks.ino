/*
 * Code created by Alexandre Lavoie, Tristan Viera, Justin De La Fosse, and Ryan Baker of team int kiwi.
 */

#include <Servo.h>

const int FIRST_LINE = 1;
int kiwi = 0;

// Constants
const float MIN_DISTANCE = 10;
const int MAX_SPEED = 10;
const int MAX_ANGLE_TIME = 50;
const float TURNING_CONSTANT = 0.25;
const float LEFT_MOTOR_CONSTANT = 1;
const float RIGHT_MOTOR_CONSTANT = -0.9;
const float LEFT_MOTOR_ZERO = 90;
const float RIGHT_MOTOR_ZERO = 85;

// Pin
const int triggerPin = 11;
const int echoPin = 10;
const int lMotorPin = 8;
const int rMotorPin = 9;
const int lLightSensor = A1;
const int rLightSensor = A0;

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
bool is_turning = false;
bool is_at_line = false;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lLightSensor, INPUT);
  pinMode(rLightSensor, INPUT);
  
  left_wheel.attach(lMotorPin);
  right_wheel.attach(rMotorPin);
  Serial.begin(9600);
}

void loop() {
  Serial.print("Distance: ");
  Serial.println(distance_to_object);
  Serial.print("Left Light Sensor: ");
  Serial.println(left_light_sensor);
  Serial.print("Right Light Sensor: ");
  Serial.println(right_light_sensor);
  Serial.print("Current Line: ");
  Serial.println(current_line);
  Serial.print("Changing Line: ");
  Serial.println(is_changing_line);
  Serial.print("At End: ");
  Serial.println(is_at_end);
  Serial.print("Is Turning: ");
  Serial.println(is_turning);
  test_sensors();
  move();
  delay(100);
  left_wheel.write(LEFT_MOTOR_ZERO);
  right_wheel.write(RIGHT_MOTOR_ZERO);
  delay(1);
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
    left_light_sensor = (analogRead(lLightSensor)>300);
    right_light_sensor = (analogRead(rLightSensor)>300);
}

void move_forward_two_sensors(){
  if(!left_light_sensor&&!right_light_sensor){
    left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
    right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
  }else if(left_light_sensor){
    left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED*TURNING_CONSTANT);
    right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
  }else if(right_light_sensor){
    left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
    right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED*TURNING_CONSTANT);
  }else if(left_light_sensor&&right_light_sensor){
    is_going_left = FIRST_LINE<current_line;
    is_at_end = true;
    is_turning = true;
    is_at_line = false;
  }
}

void move_forward_one_sensor(){
  if(is_going_left){
    if(right_light_sensor){
      left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
      right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED*TURNING_CONSTANT);
    }else{
      left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
      right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
    }
  }else{
     if(left_light_sensor){
      left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED*TURNING_CONSTANT);
      right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
    }else{
      left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
      right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
    }
  }
}

void turning(){
    if(is_going_left){
      right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
    }else{
      left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
    }
    delay(MAX_ANGLE_TIME);
    is_turning = false;
}

void line_change(){
  if(is_turning){
    turning();
  }else if(is_at_line){
    if(is_going_left){
      left_wheel.write(LEFT_MOTOR_ZERO+LEFT_MOTOR_CONSTANT*MAX_SPEED);
      if(left_light_sensor) is_changing_line = false;
    }else{
      right_wheel.write(RIGHT_MOTOR_ZERO+RIGHT_MOTOR_CONSTANT*MAX_SPEED);
      if(right_light_sensor) is_changing_line = false;
    }
  }else if(left_light_sensor&&right_light_sensor){
    is_at_line = true;
  }
}

void end(){
  if(is_turning){
    turning();
  }else{
    move_forward_one_sensor();
    if((is_going_left&&left_light_sensor)|(!is_going_left&&right_light_sensor)){
       if(current_line==FIRST_LINE){
         // WINNER!
       }else{
         current_line -= (is_going_left * 2) - 1;
       }
    }
  }
}

void set_line_change(){
  if(current_line==1&&is_going_left){
    is_going_left=false;
  }else if(current_line == 4 && !is_going_left){
    is_going_left = true;
  }
  current_line -= (is_going_left * 2) - 1;
  is_changing_line = true;
  is_turning = true;
}

void test_sensors(){
  test_light_sensors();
  distance_to_object = get_distance();
}

void move(){
  if(is_changing_line){
    line_change();
  }else if(is_at_end){
    end();
  }else{
    if(distance_to_object<=MIN_DISTANCE){
      set_line_change();
    }else{
      move_forward_two_sensors();
    }
  }
}
