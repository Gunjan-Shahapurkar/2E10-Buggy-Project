#include <WiFiNINA.h>
#include <PID_v1.h>
#include <avr/dtostrf.h>    // convert double to char

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
long max_dis = 15;  

unsigned long current_t;
const unsigned long period = 500;

// Client command
char c;
// Client greeting
char temp;

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

//  Serial.print("Distance: ");
//  Serial.println(distance);
//  Serial.print("Speed: ");
//  Serial.println(Output);
//  
  // Forward Button
  if (c == 'f'){    
    if((eyeValueL == LOW) && (eyeValueR == HIGH) && (Output < 0)){    // TURN LEFT FORWARDS detects white
      motor_left(abs(Output));
    }

    if((eyeValueL == LOW) && (eyeValueR == HIGH) && (Output > 0)){    // TURN LEFT BACKWARDS detects white
      motor_right(Output);
      //motor_left(Output);
    }
    
    if ((eyeValueR == LOW) && (eyeValueL == HIGH) && (Output < 0)){    // TURN RIGHT FORWARD
      motor_right(abs(Output));
    }

    if ((eyeValueR == LOW) && (eyeValueL == HIGH) && (Output > 0)){    // TURN RIGHT BACKWARDS
      motor_left(Output);
      //motor_right(Output);
    }
    
    if((eyeValueL == HIGH) && (eyeValueR == HIGH) && (Output < 0)){    // GO FORWARDS WHEN BOTH DETECTORS ON WHITE
      motor_forward(abs(Output));
    }

    if((eyeValueL == HIGH) && (eyeValueR == HIGH) && (Output > 0)){    // GO BACKWARDS ON WHITE WHEN OBJECT IS IN CLOSE RANGE
      motor_backwards(Output);
    }
    
    if ((eyeValueR == LOW) && (eyeValueL == LOW)){    // STOPS WHEN BOTH SENSORS ARE ON WHITE
      motor_off();
    }

}

 // Stop Button 
 if (c == 's') {   
  motor_off(); 
 }

 // Reports obstacles
 if ((distance < max_dis)) {   
  client.write("O");
  }
  else if ((distance > max_dis)){
    client.write("C");
  }
  char distance_s[8]; // Buffer big enough for 7-character float
  dtostrf(distance, 6, 2, distance_s);
  client.write(distance_s);
  Serial.println(distance_s);
  
}


 
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
void motor_forward(double speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void motor_backwards(double speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void motor_left(double speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void motor_right(double speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void motor_off() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
