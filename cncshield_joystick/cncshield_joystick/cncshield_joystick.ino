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
  speedctrl(readjoystick(5),stephor,dirhor);
  speedctrl(readjoystick(4),stepver,dirver);
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

int readjoystick(int pin){
 if(analogRead(pin) > 562)return map(analogRead(pin), 562, 1023, 20, 1);
 else if(analogRead(pin) < 462)return map(analogRead(pin), 0, 462, -1, -20);
 else return 0;
}
