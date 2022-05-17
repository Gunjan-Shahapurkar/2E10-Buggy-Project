// Start/stop motors using IR sensors -> start when sensors detect dark, stop when sensors detect light
// Stop motors when Ultrasonic sensor senses obstacle 

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


void setup() {
  // put your setup code here, to run once:
  
  // IR sensors
  Serial.begin(9600);

  // IR Sensors
  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );

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

}

void loop() {
  // put your main code here, to run repeatedly:

  // US sensors
  int distance;
  long duration;
  int max_dis = 10;

  digitalWrite( US_TRIG, LOW );
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );
    
    
  duration = pulseIn( US_ECHO, HIGH );
  distance = duration/58;

  Serial.print("Distance detected: ");
  Serial.print( distance );
  Serial.println(" cm");
  

  // Control speed 
  analogWrite(ENA, 255);
  analogWrite(ENB, 255); 

  int eyeValueL = digitalRead( LEYE );
  int eyeValueR = digitalRead( REYE );

  
  // Left side
 
  if((eyeValueL == HIGH) && (distance > max_dis)){  // ON
    Serial.print("low  ");
    left_motor_on();
    
  }else if ((eyeValueL == LOW) || (distance < max_dis)){    // OFF
    Serial.print("high ");
    left_motor_off();
  }
  
  // Right side
    if((eyeValueR == HIGH) && (distance > max_dis)){    // ON
    Serial.println("low  ");
    right_motor_on();
    
  }else if ((eyeValueR == LOW) || (distance < max_dis)){    // OFF
    Serial.println("high ");
    right_motor_off();
    
  }
 
}


void left_motor_on() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void left_motor_off() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void right_motor_on() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right_motor_off() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
}
