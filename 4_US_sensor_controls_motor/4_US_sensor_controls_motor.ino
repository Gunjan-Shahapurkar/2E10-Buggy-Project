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

unsigned long current_t;
unsigned long start_t;
const unsigned long check = 1000;   // 1 second

void setup() {
  // put your setup code here, to run once:

  // Motors
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);

  // US sensors
  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);

  start_t = millis();   // initial start time

}

void loop() {
  // put your main code here, to run repeatedly:

// US sensors
  int distance;
  long duration;

  digitalWrite( US_TRIG, LOW );
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );

  
  current_t = millis();   // current time
  //if (current_t - start_t >= check){
    
    duration = pulseIn( US_ECHO, HIGH );
    distance = duration/58;
    start_t = current_t;
  //}
  
  Serial.print("Distance detected: ");
  Serial.print( distance );
  Serial.println(" cm");
  //delay(1000);


  // Control speed 
  analogWrite(ENA, 255);
  analogWrite(ENB, 255); 

  if (distance > 20 ){    // ON

    // Left 
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Right
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

  else if (distance <= 20 ) {   // OFF

    // Left
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    // Right
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

}
