/*
 * Code created by Alexandre Lavoie and Tristan Viera of team int kiwi.
 */

int kiwi = 0; // <3

// Light sensor variables.
bool left_light_sensor = false;
bool right_light_sensor = false;

// Distance
float distance = 0;

// Constant variables.
const float MAX_MOTOR_SPEED = 1;

void setup() {
  
}

void loop() {
  test_light_sensor();
  distance = get_distance();
  move();
}

/*
 * Tests for the light sensor in the line following problem.
 */

void test_light_sensor(){

}

/*
 * Gets distance to the object in front of the robot.
 */

float get_distance(){
  
}

/*
 * Moves the robot according to the given sensor input.
 */

void move(){
  if(!left_light_sensor&&!right_light_sensor){
      
  }else if(!left_light_sensor){
      
  }else{
        
  }  
}
