//pin definitions of the cnc shield v3
#define stephor   2
#define stepver1  3
#define stepver2  4
#define dirhor    5
#define dirver1   6
#define dirver2   7
#define enable    8

#define MOVE_SPEED_FAST 1 

String bufString;
int serialVals[4] = {0,0,0,0};
bool newData = 0;


void setup() {
  pinMode(stephor,OUTPUT);
  pinMode(stepver1,OUTPUT);
  pinMode(stepver2,OUTPUT);
  pinMode(dirhor,OUTPUT);
  pinMode(dirver1,OUTPUT);
  pinMode(dirver2,OUTPUT);
  pinMode(enable,OUTPUT);
  digitalWrite(enable, LOW);

  digitalWrite(dirhor,HIGH);
  digitalWrite(dirver1,HIGH);
  digitalWrite(dirver2,HIGH);

  Serial.begin(9600);
}

void loop() {
  serialRead();
  if(newData){
    movefast(serialVals[0],stephor,dirhor);
    movefast(serialVals[1],stepver1,dirver1);
    newData=0;
  }
  speedctrl(serialVals[2],stephor,dirhor);
  speedctrl(serialVals[3],stepver1,dirver1);
}

void serialRead(){
 if(Serial.available()){
  for(int i=0;i<=3;i++){
    bufString = Serial.readStringUntil(','); 
    serialVals[i]=bufString.toInt();
  }
  while (Serial.available() > 0) {
    Serial.read();
  } 
  newData=1;
 }
}

void movefast(int steps, int steppin, int dirpin){
  if(steps=0){
    return 0;
  }
  else if(steps<0){
    steps=steps*(-1);
    digitalWrite(dirpin,LOW);
  }
  else{
   digitalWrite(dirpin,HIGH); 
  }
  
  for(int i=1; i<=steps; i++){ 
    digitalWrite(steppin, HIGH); 
    digitalWrite(steppin, LOW); 
    delay(MOVE_SPEED_FAST);  
  }
  return 0;
}
void speedctrl(int movespeed, int steppin, int dirpin){
  if (movespeed=0){
    return 0;
  }
  else if(movespeed<0){
    movespeed=movespeed*(-1);
    digitalWrite(dirpin,LOW);
  }
  else{
    digitalWrite(dirpin,HIGH);
  }
   
    digitalWrite(steppin, HIGH);
    delay(movespeed);  
    digitalWrite(steppin, LOW); 
    delay(movespeed);  


    return 0;
} 
