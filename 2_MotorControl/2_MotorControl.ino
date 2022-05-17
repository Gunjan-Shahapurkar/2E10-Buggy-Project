// Change buggy directions & speed

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

  
  // Control direction 

  // forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(1000);
  

  // backwards
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(1000);
  

  // left = backwards  right = forwards

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(1000);
  

  // right = backwards  left = forwards

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  delay(1000);
  

  // motor off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  delay(1000);
}
