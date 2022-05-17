#include <WiFiNINA.h>


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

// Client command
char c;
// Client greeting
char temp;

// PID
double default_speed = 175;
double setpoint = 10;
double Kp = 50;
double dis_difference;
double error;


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

  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);
  server.begin();

}

void loop() {

  // US sensors
  long duration;
  long distance;
  long max_dis = 20;

  digitalWrite( US_TRIG, LOW );   // digitalWrite: sets voltage on pin
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );    // HIGH: 3.3V
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );   // LOW: 0V

  duration = pulseIn( US_ECHO, HIGH );
  distance = duration/58;
  dis_difference = setpoint - distance;

  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Difference: ");
  Serial.println(dis_difference);

  // Control speed 
//  analogWrite(ENA, 100);
//  analogWrite(ENB, 100); 

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
  
  
  // Forward Button
  if (c == 'f'){    
    if((eyeValueL == HIGH) && (dis_difference < 0)){    // LEFT FORWARD - detects black
      left_motor_on(default_speed + (error*Kp));
    }

    if ((eyeValueL == HIGH) && (dis_difference > 0)){   // LEFT BACKWARDS
      left_motor_back(default_speed + (error*Kp));
    }
    
    if ((eyeValueL == LOW)){    // LEFT OFF - detects white
      left_motor_off();
    }
    
    if((eyeValueR == HIGH) && (dis_difference < 0)){    // RIGHT FORWARD - detects black
      right_motor_on(default_speed + (error*Kp));
    }

    if((eyeValueR == HIGH) && (dis_difference > 0)){    // RIGHT BACKWARDS
      right_motor_back(default_speed + (error*Kp));
    }
    
    if ((eyeValueR == LOW)){    // RIGHT OFF - detects white
      right_motor_off();
    }
 
}

 // Stop Button 
 if (c == 's') {   
   left_motor_off();
   right_motor_off(); 
 }

 
 // Reports obstacles
 
 // dis_difference < 0; forward
 if ((dis_difference >= -10) && (dis_difference <= -7)) {   // Object distance greater than setpoint;  distance: 17-20cm 
  error = 1.5;  
  client.write("O");
  }
 else if ((dis_difference > -7) && (dis_difference <= -4)) {  // Object distance greater than setpoint;   distance: 14-16cm 
  error = 1;
  client.write("O");
  }
 else if ((dis_difference > -4) && (dis_difference <= -1)) {  // Object distance greater than setpoint;   distance: 11-13cm 
  error = 0.5; 
  client.write("O");
  } 

 // dis_difference > 0; backwards
 else if ((dis_difference <= 9) && (dis_difference >= 7)){    // Object distance less than setpoint;   distance: 7-9cm
  error = 1.5;   
  client.write("O");
 }
 else if ((dis_difference < 7) && (dis_difference >= 4)){    // Object distance less than setpoint;   distance: 4-6cm
  error = 1;   
  client.write("O");
 }
 else if ((dis_difference < 4) && (dis_difference >= 1)){    // Object distance less than setpoint;   distance: 1-3cm
  error = 0.5;   
  client.write("O");
 }

 // Object is not within range
 else if ((distance > max_dis)){
  error = 0;
  client.write("C");
 }
 
}


 // Functions for Motor control
 void left_motor_on(double speed) {
  analogWrite(ENA, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
 }

 void right_motor_on(double speed) {
  analogWrite(ENB, speed);
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

void left_motor_back(double speed){
  analogWrite(ENA, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void right_motor_back(double speed){
  analogWrite(ENB, speed);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
