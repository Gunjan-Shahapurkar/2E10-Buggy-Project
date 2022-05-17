// Start/stop motors using IR sensors -> start when sensors detect dark, stop when sensors detect light

// IR sensors
const int LEYE = 9;
const int REYE = 2;

int lastEyeValueL = LOW;
int lastEyeValueR = LOW;


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

  // Control speed 
  analogWrite(ENA, 255);
  analogWrite(ENB, 255); 

  int eyeValueL = digitalRead( LEYE );
  int eyeValueR = digitalRead( REYE );

  
  // Left side
 
 if (eyeValueL != lastEyeValueL){
  if( eyeValueL == HIGH ){  // ON
    Serial.print("low  ");

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    
  }else{    // OFF
    Serial.print("high ");

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  lastEyeValueL = eyeValueL;
 }

  // Right side
  
  if (eyeValueR != lastEyeValueR){
    if( eyeValueR == HIGH ){    // ON
    Serial.println("low  ");

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    
  }else{    // OFF
    Serial.println("high ");

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW); 
  }
  
  lastEyeValueR = eyeValueR;
}

}
