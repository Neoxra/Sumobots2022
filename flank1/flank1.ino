// A basic program that locates the enemy robot, evades them and tries to hit them from the side

// Step one, drive forward to change where we are so they take longer to find us.
// Step two search for the other robot.
// Step three move past the other robot.
// Step four find the robot again.
// Step five hit the openent. 

/////////////////////////////////////////////////////////////////////////////////////////////////////
// To be changed when robot is built

// Start Button
int button_pin = 7;

// Distance sensors
const int trig_pin_left = 3;
const int echo_pin_left = 4;
const int trig_pin_right = 5;
const int echo_pin_right = 6;

int scan_distance = 60;

// Line sensors
int top_left = A1;
int top_right = A2;
int bottom_left = A3;
int bottom_right = A4;

int white = 0;
int black = 1;

// Motor control
int motor1pin1 = 12;
int motor1pin2 = 11;

int motor2pin1 = 10;
int motor2pin2 = 9;

int motor1speed = 13;
int motor2speed = 8;

// See what state of the game we are in; initial start 0
int gamestate = 0;


void setup() {
  // Setup IO pins
  pinMode(button_pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  
  // Step 0 Start - Wait for button to be pressed and then wait 5 seconds before start; else idle blinker led
  while (gamestate == 0) {
    // Match start Sequence
    if (digitalRead(button_pin) == HIGH && gamestate == 0) {
      // intiate 5 second count down
      for (int i = 0; i <= 4; i++)  {
        blink(500);
      }

      gamestate = 1;
      Serial.println("gamestate: 1");
      blink(50);
    }
    else {
      for (int i = 0; i <= 2; i++) {
        blink(50);
      }
      delay(1000);  
    }
  }
 

  // Step 1 - move forward
  long int t1 = millis();
  long int t2 = millis();
  
  if (gamestate == 1){
    // Take current runtime
    t1 = millis(); 
    t2 = millis();

    while (t2 <= (t1 + 1000) && check_any_line() == 0) {
      
      analogWrite(motor1speed, 255); 
      analogWrite(motor2speed, 255); 
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      
      t2 = millis();
    }
    
    analogWrite(motor1speed, 0); 
    analogWrite(motor2speed, 0); 
    gamestate = 2;
    Serial.println("gamestate: 2");
    blink(50);
    
  }

  // Step 2 - turn left and find enemy
  if (gamestate == 2) {
  
      analogWrite(motor1speed, 125); 
      analogWrite(motor2speed, 0); 
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);

    // Check to see if either sensors see something within scan_distance (mm)
    if ((evaluate_distance(trig_pin_left, echo_pin_left) < scan_distance || evaluate_distance(trig_pin_right, echo_pin_right) < scan_distance) && check_any_line() == 0) {
      
      analogWrite(motor1speed, 0); 
      analogWrite(motor2speed, 0);
      gamestate = 3;
      Serial.println("gamestate: 3");
      blink(50);
      
    }
      
  }

  // Step 3 - Turn right until we can no longer see the oponent and then drive forward a bit
  if (gamestate == 3) {

      analogWrite(motor1speed, 0); 
      analogWrite(motor2speed, 125); 
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);

      // Check if the oponent is no longer directly in front of us or that we aren't over the line
      if (evaluate_distance(trig_pin_left, echo_pin_left) > scan_distance && evaluate_distance(trig_pin_right, echo_pin_right) > scan_distance && check_any_line() == 0) {
        t1 = millis();  
        t2 = millis();

        while (t2 <= (t1 + 1000) && check_any_line() == 0) {
          
          analogWrite(motor1speed, 255); 
          analogWrite(motor2speed, 255); 
          digitalWrite(motor1pin1, HIGH);
          digitalWrite(motor1pin2, LOW);
          digitalWrite(motor2pin1, HIGH);
          digitalWrite(motor2pin2, LOW);
          
          t2 = millis();
        }
        
        analogWrite(motor1speed, 0); 
        analogWrite(motor2speed, 0); 
        gamestate = 4;
        Serial.println("gamestate: 4");
        blink(50);
    }
  }

  // Search for enemy again (can mix with gamestaate 2 later if program works)
  if (gamestate == 4) {

    analogWrite(motor1speed, 255); 
    analogWrite(motor2speed, 0); 
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);

    // Check to see if either sensors see something within scan_distance (mm)
    if (evaluate_distance(trig_pin_left, echo_pin_left) < scan_distance || evaluate_distance(trig_pin_right, echo_pin_right) < scan_distance && check_any_line() == 0) {
      
      analogWrite(motor1speed, 0); 
      analogWrite(motor2speed, 0);
      gamestate = 5;
      Serial.println("gamestate: 5");
      blink(50);
    }
    
  }  
  // alligning self charging enemy .
  if (gamestate == 5) {
    
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    
    if (check_any_line() == 0) {
      
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
    else{
      // turn around and move away from edge, restart search for enemy.
      gamestate = 1;
      Serial.println("gamestate: 1 (hit line)");
      blink(50);
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
  unsigned int duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  unsigned int distance = duration * 0.034 / 2;
  /*
  // Prints the distance on the Serial Monitor
  if (trigPin == trig_pin_left) {
    Serial.print("Distance Left: ");
    Serial.println(distance);
  }
  if (trigPin == trig_pin_right){
    Serial.print("Distance Right: ");
    Serial.println(distance);
  }*/

  return distance;
  
}

// not working atm...
int check_any_line() {
  // Check if any line sensor has seen the line
  /*
  if (digitalRead(top_left) == white) {
    return 1;
  }
  if (digitalRead(top_right) == white) {
    return 2;
  }
  if (digitalRead(bottom_left) == white) {
    return 3;
  }
  if (digitalRead(bottom_right) == white) {
    return 4;
  } 

  */

  return 0;
}

int blink(int delayLED) {
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(delayLED);                       
  digitalWrite(LED_BUILTIN, LOW); 
  delay(delayLED);
}
