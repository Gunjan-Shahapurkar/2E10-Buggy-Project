#include <WiFiNINA.h>
#include <PID_v1.h>


char ssid[] = "2E10_TC01";    
char pass[] = "LindaDoyle";    

WiFiServer server(5200); // Created server at that port 


// IR sensors
const int LEYE = 9;
const int REYE = 2;


// Motors
// direction control

// left
const int IN1 = 5;  
const int IN2 = 6;
// right
const int IN3 = 16;
const int IN4 = 3;

// speed control
const int ENA = 17;
const int ENB = 19;


// Ultrasonic sensors
const int US_TRIG = 7; 
const int US_ECHO = 8;

long duration;
long distance;
long max_dis = 10;  

unsigned long current_t;    // Current time since the program began
const unsigned long period = 500;     // Polls US sensors every half a second

// Client command
char c;
// Filter variable
char temp;

// Send data
char distance_char;
char speed_char;

// PID
double Setpoint, Input, Output;
double Kp = 35, Ki = 0, Kd = 1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void setup() {
  
  Serial.begin(9600);  // Speed at which info is transferred in a communication channel

  // IR Sensors
  pinMode( LEYE, INPUT );   // pinMode: configures specific pin for INPUT/OUTPUT
  pinMode( REYE, INPUT );

  // Motors - at a low impedence state & provide power (OUTPUT)
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);

  // US sensors
  pinMode(US_TRIG, OUTPUT);   // Triggers sensors
  pinMode(US_ECHO, INPUT);    // Sends a US signal out

  // PID
  Input = distance;
  Setpoint = max_dis;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255,255);
  
  // Networking
  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);
  server.begin();

}

void loop() {

  current_t = millis();
  
  // US sensors
  if (current_t % period == 0) {   // Polling US Sensors every half a second
    distance = object_distance(); 
  }

 
 int eyeValueL = digitalRead( LEYE );
 int eyeValueR = digitalRead( REYE );


 WiFiClient client = server.available();
 if (client.connected()) { 
  Serial.println("Client connected");
  if (client.available()){
    temp = client.read();
  }
 }

 // Filters out characters associated with the buttons
 if (temp == 'f') {
  c = temp;
 }
 else if (temp == 's') {
    c = temp;
  }

  // PID
  Input = distance;
  myPID.Compute();


  // Forward Button
  if (c == 'f'){    
    if((eyeValueL == HIGH) && (Output < 0) && (distance > 0)){    // LEFT ON - detects black & moves forward
      left_motor_on(abs(Output));
    }
    
    if((eyeValueL == HIGH) && (Output > 0)){    // LEFT ON - detects black & moves back
      left_motor_back(Output);
    }
    
    if ((eyeValueL == LOW)){    // LEFT OFF
      left_motor_off();
    }
    
    if((eyeValueR == HIGH) && (Output < 0) && (distance > 0)){    // RIGHT ON - detects black & moves forward
      right_motor_on(abs(Output));
    }

    if((eyeValueR == HIGH) && (Output > 0)){    // RIGHT ON - detects black & moves back
      right_motor_back(Output);
    }
    
    if ((eyeValueR == LOW)){    // RIGHT OFF
      right_motor_off();
    }
}

 // Stop Button 
 if (c == 's') {   
   left_motor_off();
   right_motor_off(); 
 }

  if (current_t % period == 0) {
    //Reports distance and speed
    if (distance > 255) {
      distance_char = char(255);
      server.write(distance_char);
    }
    else {
      distance_char = char(distance);
      server.write(distance_char);
    }
    speed_char = char(abs(Output));
    server.write(speed_char);
    
    // print in Serial Monitor for debugging
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.print("Speed: ");
    Serial.println(Output);
    Serial.println(long(distance_char));
    Serial.println(double(speed_char));
  }

}

// Functions
 
 // Ultrasonic sensor function
 long object_distance() {
  digitalWrite( US_TRIG, LOW );   // digitalWrite: sets voltage on pin
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );    // HIGH: 3.3V
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );   // LOW: 0V

  duration = pulseIn( US_ECHO, HIGH );
  distance = duration/58;
  return distance;
 }
  
 // Functions for Motor control
 void left_motor_on(double speed_m) {
  analogWrite(ENA, speed_m);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
 }

 void right_motor_on(double speed_m) {
  analogWrite(ENB, speed_m);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
 }

 void left_motor_off(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
 }

void right_motor_off() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void left_motor_back(double speed_m){
  analogWrite(ENA, speed_m);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void right_motor_back(double speed_m){
  analogWrite(ENB, speed_m);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
