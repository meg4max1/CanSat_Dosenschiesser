import processing.serial.*;

Serial recvSerial;

String recvPort = "COM8";

Table table;
String filename;

String inputString;
int[] displayOrder = {0, 9, 1, 10,
                      2, 11, 3, 4, 
                      5, 6, 7, 14, 
                      15, 16, 17, 12, 
                      13, 8};
String[] sensorLabels = {"SatTemp:", "SatHumidity:", "SatPressure:", "SatAccel:", 
                         "SatGpsNr:", "SatLat:", "SatLon:", "SatGPSAlt:", 
                         "PacketRssi:", "BaseTemp:", "BaseHumidity:", "BasePressure:", 
                         "TrackAltAngle:", "TrackAziAngle", "BaseGpsNr:", "BaseLat:", 
                         "BaseLon:", "BaseGPSAlt:"};
String[] sensorUnits =  {"°C", "%RH", "mbar", "m/s²", 
                         " ", "deg", "deg", "m", 
                         " ", "°C", "%RH", "mbar", 
                         "°",  "°", " ", "deg", 
                         "deg", "m" };
int[] sensorMultipliers = { 10, 10, 1000, 1000, 
                            1, 100000, 100000, 10, 
                            1, 10, 10, 1000, 
                            1, 10, 1, 100000, 
                            100000, 10};
String[] csvString = new String[sensorLabels.length];
float[] sensorData = new float[sensorLabels.length];

String[] calcLabels = {"deltaTemp:", "deltaHumidity:", "deltaPressure:"};
String[] calcUnits =  {"°C", "%RH", "mbar"};
float[] calcData = new float[calcLabels.length];

int saveCounter = 0;
int saveInterval = 60;

int lastGUIupdate = 0;
int refreshGUIinterval = 500;

int columnDist= 402;

void setup()
{
  size(1280, 720, P3D);
  surface.setResizable(true);



  filename = "log_"+year()+"_"+month()+"_"+day()+"_"+hour()+"_"+minute()+"_"+second()+".csv";

  table =new Table();
  table.addColumn("Time");
  for (int i=0; i<sensorLabels.length; i++) {
    table.addColumn(sensorLabels[i]);
  }
  background(10);
  recvSerial = new Serial(this, recvPort, 9600);
}


void draw()
{
  if ( recvSerial.available() > 0) {
    inputString = recvSerial.readStringUntil('\n');
    if (inputString != null) {
      csvString = split(inputString, ",");
      for (int i=0; i<csvString.length; i++) {
        sensorData[i] = float(csvString[i]);
      }
      recvSerial.clear();
      calculate();
      table.addRow();
      table.setInt(table.getRowCount()-1, "Time", hour()*3600+minute()*60+second());
      for (int i=0; i<sensorData.length; i++) {
        table.setFloat(table.getRowCount()-1, sensorLabels[i], sensorData[i]/sensorMultipliers[i]);
      }
      saveCounter++;
      if (saveCounter>saveInterval) {
        saveTable(table, filename);
        text("saving", 600, 600);
        saveCounter = 0;
      }
    }
  }
  
  
  if (millis() - lastGUIupdate > refreshGUIinterval) {
    drawGUI();
  }
  
  
  delay(1);
}



void calculate() {
  calcData[0]=sensorData[0]/sensorMultipliers[0]-sensorData[9]/sensorMultipliers[9];
  calcData[1]=sensorData[1]/sensorMultipliers[1]-sensorData[10]/sensorMultipliers[10];
  calcData[2]=sensorData[2]/sensorMultipliers[2]-sensorData[11]/sensorMultipliers[11];
}

void drawGUI() {

  fill(20);
  rect(32, 32, 362, 68+sensorLabels.length*40, 8);
  fill(200);
  textSize(36);
  text("Raw Data Inputs", 55, 78);
  textSize(24);
  fill(255);
  for (int i=0; i<sensorData.length; i++) {
    text(sensorLabels[displayOrder[i]], 44, 124+40*i);
    text(sensorUnits[displayOrder[i]], 325, 124+40*i);
    text((sensorData[displayOrder[i]]/sensorMultipliers[displayOrder[i]]), 210, 124+40*i);
  }
  fill(20);
  rect(32+columnDist, 32, 362, 68+sensorLabels.length*40, 8);
  fill(200);
  textSize(36);
  text("Calculated", 55+columnDist, 78);
  textSize(24);
  fill(255);
  for (int i=0; i<calcLabels.length; i++) {
    text(calcLabels[i], 44+columnDist, 124+40*i);
    text(calcUnits[i], 325+columnDist, 124+40*i);
    text(calcData[i], 210+columnDist, 124+40*i);
  }
}  
