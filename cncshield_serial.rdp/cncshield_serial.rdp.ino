//pin definitions of the cnc shield v3
#define stephor  2
#define stepver  3
#define dirhor   5
#define dirver   6
#define enable   8

#define MOVE_SPEED_FAST 1

String bufString;
int serialVals[2] = {0, 0};
bool newData = 0;

int horCounter = 0;
int verCounter = 0;

void setup() {
  pinMode(stephor, OUTPUT);
  pinMode(stepver, OUTPUT);
  pinMode(dirhor, OUTPUT);
  pinMode(dirver, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(enable, LOW);
  
  digitalWrite(dirhor, HIGH);
  digitalWrite(dirver, HIGH);

  Serial.begin(9600);
}

void loop() {
  serialRead();
  if (newData) {
    speedctrl(serialVals[0],stephor,dirhor);
    speedctrl(serialVals[1],stepver,dirver);
    newData=0;
  }
  }

void serialRead() {
  if (Serial.available()) {
    for (int i = 0; i <= 1; i++) {
      bufString = Serial.readStringUntil(',');
      serialVals[i] = bufString.toInt();
    }
    while (Serial.available() > 0) {
      Serial.read();
    }
    newData = 1;
  }
}

void speedctrl(int steps, int steppin, int dirpin){
  if(steps<0){
    steps=steps*(-1);
    digitalWrite(dirpin,LOW);
  }
  else if(steps>0)digitalWrite(dirpin,HIGH);
  else return;
  for(int i=1;i<steps;i++){ 
    digitalWrite(steppin, HIGH); 
    digitalWrite(steppin, LOW); 
    delay(1);
  }    
}
