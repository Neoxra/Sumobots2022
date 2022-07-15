// A basic program that locates the enemy robot, evades them and tries to hit them from the side

// Step one, drive forward to change where we are so they take longer to find us.
// Step two search for the other robot.
// Step three move past the other robot.
// Step four find the robot again.
// Step five hit the openent. 

/////////////////////////////////////////////////////////////////////////////////////////////////////
// To be changed when robot is built
// Distance sensor 1
const int trig_pin_left = 9;
const int echo_pin_left = 10;

long duration_left;
int distance_left;

// Distance sensor 2
const int trig_pin_right = 11;
const int echo_pin_right = 12;

long duration_right;
int distance_right;

// Line sensors
int top_left = 1;
int top_right = 2;
int bottom_left = 3;
int bottom_right = 4;

int white = 0;
int black = 1;

// Motor control
int motor1pin1 = 46;
int motor1pin2 = 48;

int motor2pin1 = 50;
int motor2pin2 = 52;

int motor1speed = 12;
int motor2speed = 13;

// See what state of the game we are in
int gamestate = 0;
void setup() {
  // Setup IO pins
  pinMode(trig_pin_left, OUTPUT); 
  pinMode(echo_pin_left, INPUT); 
  
  pinMode(trig_pin_right, OUTPUT); 
  pinMode(echo_pin_right, INPUT); 

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(motor1speed, OUTPUT); 
  pinMode(motor2speed, OUTPUT);

  // Starts the serial communication for debugging
  Serial.begin(9600); 

}

void loop() {
  // Wait for button to be pressed and then wait 5 seconds before start

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // Step 1 move
  long int t1 = millis();
  long int t2 = millis();
  if (gamestate == 1){
    // Take current runtime
    t1 = millis();
  
    t2 = millis();
    while (t2 <= (t1 + 1000) && check_any_line == 0) {
      
      analogWrite(motor1speed, 255); 
      analogWrite(motor2speed, 255); 
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, HIGH);
      
      t2 = millis();
    }
    
    analogWrite(motor1speed, 0); 
    analogWrite(motor2speed, 0); 
    gamestate = 2;
    
  }

  // Step 2 turn left and find
  if (gamestate == 2) {

      analogWrite(motor1speed, 255); 
      analogWrite(motor2speed, 255); 
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);

    // Check to see if either sensors see something within 80mm
    if ((evaluate_distance(trig_pin_left, echo_pin_left) < 80 || evaluate_distance(trig_pin_right, echo_pin_right) < 80) && check_any_line == 0) {
      
      gamestate = 3;

      analogWrite(motor1speed, 0); 
      analogWrite(motor2speed, 0);
      
    }
      
  }

  // Trun right until we can no longer see the oponent and then drive forward a bit
  if (gamestate == 3) {

      analogWrite(motor1speed, 255); 
      analogWrite(motor2speed, 255); 
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);

      // Check if the oponent is no longer directly in front of us or that we aren't over the line
      if ((evaluate_distance(trig_pin_left, echo_pin_left) > 80 && evaluate_distance(trig_pin_right, echo_pin_right) > 80) && check_any_line == 0) {
      
      t1 = millis();
  
      t2 = millis();
      while (t2 <= (t1 + 1000) && ) {
        
        analogWrite(motor1speed, 255); 
        analogWrite(motor2speed, 255); 
        digitalWrite(motor1pin1, HIGH);
        digitalWrite(motor1pin2, HIGH);
        
        t2 = millis();
      }
      
      analogWrite(motor1speed, 0); 
      analogWrite(motor2speed, 0); 
      gamestate = 4;
    }
  }

  if (gamestate == 4) {

    analogWrite(motor1speed, 255); 
    analogWrite(motor2speed, 255); 
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    // Check to see if either sensors see something within 80mm
    if ((evaluate_distance(trig_pin_left, echo_pin_left) < 80 || evaluate_distance(trig_pin_right, echo_pin_right) < 80) && check_any_line == 0) {
      
      gamestate = 5;

      analogWrite(motor1speed, 0); 
      analogWrite(motor2speed, 0);
      
    }
    
  }  

  if (gamestate == 5) {
    
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, HIGH);
    
    while (check_any_line == 0) {
      
      if (evaluate_distance(trig_pin_left, echo_pin_left) > evaluate_distance(trig_pin_right, echo_pin_right)) {
        analogWrite(motor1speed, 200); 
        analogWrite(motor2speed, 255); 
      }
      
      if (evaluate_distance(trig_pin_left, echo_pin_left) < evaluate_distance(trig_pin_right, echo_pin_right)) {
        analogWrite(motor1speed, 255); 
        analogWrite(motor2speed, 200); 
      }
      
      if (evaluate_distance(trig_pin_left, echo_pin_left) == evaluate_distance(trig_pin_right, echo_pin_right)) {
        analogWrite(motor1speed, 255); 
        analogWrite(motor2speed, 255); 
      }
      
    }
    
  }
}

int evaluate_distance(int trigPin, int echoPin) {
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;
  
}

int check_any_line() {
  // Check if any line sensor has seen the line
  if (digitalRead(top_left) == white) {
    return 1
  }
  if (digitalRead(top_left) == white) {
    return 2
  }
  if (digitalRead(top_left) == white) {
    return 3
  }
  if (digitalRead(top_left) == white) {
    return 4
  }

  return 0;
}
