// NOT AS EFFECTIVE

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


void setup() {
  // put your setup code here, to run once:
  
  // IR sensors
  Serial.begin(9600);

  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );

  // Motors
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


  int eyeValueL = digitalRead( LEYE );
  int eyeValueR = digitalRead( REYE );

  
  // Left side
 
  if( eyeValueL == LOW ){  // HIGH SPEED
    Serial.print("low  ");

    analogWrite(ENA, 80);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    
  }else{    // LOW SPEED
    Serial.print("high ");

    analogWrite(ENB, 200);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

  // Right side
  
    if( eyeValueR == LOW ){    // HIGH SPEED
    Serial.println("low  ");

    analogWrite(ENB, 80);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    
  }else{    // LOW SPEED
    Serial.println("high ");

    analogWrite(ENA, 200);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); 
  }
  
}
