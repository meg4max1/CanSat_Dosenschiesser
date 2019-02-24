//pin definitions of the cnc shield v3
#define stephor   2
#define stepver1  3
#define stepver2  4
#define dirhor    5
#define dirver1   6
#define dirver2   7
#define enable    8

String bufString;
int serialVals[4] = {0,0,0,0};
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
  for(int i=0;i<=3;i++){
  Serial.print(serialVals[i]);
  Serial.print("/");
  } 
  Serial.println(); 
  delay(2000);
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
 }
}
