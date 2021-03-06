//pinout of the cnc shield v3
#define stephor   2
#define stepver   3
#define dirhor    5
#define dirver    6
#define enable    8

void setup() {
  pinMode(stephor,OUTPUT);
  pinMode(stepver,OUTPUT);
  pinMode(dirhor,OUTPUT);
  pinMode(dirver,OUTPUT);
  pinMode(enable,OUTPUT);
  digitalWrite(enable, LOW);
  //Serial.begin(9600);  
  digitalWrite(dirhor,HIGH);
  digitalWrite(dirver,HIGH);
}

void loop() {
  speedctrl(-1,stephor,dirhor);
  speedctrl(-1,stepver,dirver);
}

void speedctrl(int movespeed, int steppin, int dirpin){
  if(movespeed<0){
    movespeed=movespeed*(-1);
    digitalWrite(dirpin,LOW);
  }
  else if(movespeed>0)digitalWrite(dirpin,HIGH);
  else return;
   
    digitalWrite(steppin, HIGH); 
    digitalWrite(steppin, LOW); 
    delay(movespeed);  
}


void teststepper(int pin, int speedms,int steps){
    for(int i=1; i<=steps; i++){
    digitalWrite(pin, HIGH);
    delay(speedms/2);
    digitalWrite(pin, LOW);
    delay(speedms/2);
  }
}
void test2stepper(int pin1, int pin2, int speedms,int steps){
    for(int i=1; i<=steps; i++){
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    delay(speedms);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    delay(speedms);
  }
}
int readjoystick(int pin){
 if(analogRead(pin) > 562)return map(analogRead(pin), 562, 1023, 50, 5);
 else if(analogRead(pin) < 462)return map(analogRead(pin), 0, 462, -5, -50);
 else return 0;
}
