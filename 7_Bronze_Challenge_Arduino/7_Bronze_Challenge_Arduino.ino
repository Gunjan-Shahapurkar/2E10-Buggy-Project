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
  long max_dis = 10;

  digitalWrite( US_TRIG, LOW );   // digitalWrite: sets voltage on pin
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );    // HIGH: 3.3V
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );   // LOW: 0V

  duration = pulseIn( US_ECHO, HIGH );
  distance = duration/58;

  // Control speed 
  analogWrite(ENA, 255);
  analogWrite(ENB, 255); 

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
    if((eyeValueL == HIGH) && (distance > max_dis)){    // LEFT ON - detects black
      left_motor_on();
    }
    
    if ((eyeValueL == LOW) || ((distance < max_dis) && (distance > 0))){    // LEFT OFF
      left_motor_off();
    }
    
    if((eyeValueR == HIGH) && (distance > max_dis)){    // RIGHT ON
      right_motor_on();
    }
    
    if ((eyeValueR == LOW) || ((distance < max_dis) && (distance > 0))){    // RIGHT OFF
      right_motor_off();
    }
 
}

 // Stop Button 
 if (c == 's') {   
   left_motor_off();
   right_motor_off(); 
 }

 // Reports obstacles
 if ((distance < max_dis)) {   
  client.write("O");
  }
  else if ((distance > max_dis)){
    client.write("C");
  }
 
}


 
 // Functions for Motor control
 void left_motor_on() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
 }

 void right_motor_on() {
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
