
const int LEYE = 9;
const int REYE = 2;

int lastEyeValueL = LOW;
int lastEyeValueR = LOW;

void setup() {
  Serial.begin(9600);

  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );
}

void loop() {

int eyeValueL = digitalRead( LEYE );
int eyeValueR = digitalRead( REYE );
  
 if (eyeValueL != lastEyeValueL){
  if( eyeValueL == HIGH ){
    Serial.print("low  ");
  }else{
    Serial.print("high ");
  }
  lastEyeValueL = eyeValueL;
 }

if (eyeValueR != lastEyeValueR){
  if( eyeValueR == HIGH ){
    Serial.println("low  ");
  }else{
    Serial.println("high ");
  }
  lastEyeValueR = eyeValueR;
}
  delay(1000);
}
