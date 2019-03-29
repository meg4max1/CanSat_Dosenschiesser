import processing.serial.*;

Serial myPort;

Table table;
String filename;

String inputString;
String[] sensorLabels = {"Temperature:", "Humidity:", "Pressure:", "Acceleration:", "Rssi:"};
String[] sensorUnits =  {"°C", "%RH", "mbar", "m/s²", " "};
int[] sensorMultipliers = { 10, 10, 1000, 10, 1};
String[] csvString = new String[5];
float[] sensorData = new float[5];
boolean portOpen = false;

int saveCounter = 0;

void setup()
{
  size(1280, 720);
  surface.setResizable(true);

  myPort = new Serial(this, "COM8", 9600);

  filename = "log_"+year()+"."+month()+"."+day()+"_"+hour()+":"+minute()+":"+second()+".csv;

  table =new Table();
  table.addColumn("Time");
  for (int i=0; i<sensorLabels.length; i++) {
    table.addColumn(sensorLabels[i]);
  }
}


void draw()
{
  if ( myPort.available() > 0) {
    inputString = myPort.readStringUntil('\n');
    if (inputString != null) {
      csvString = split(inputString, ",");
      for (int i=0; i<csvString.length; i++) {
        sensorData[i] = float(csvString[i]);
      }
      myPort.clear();
      table.addRow();
      saveCounter++;
      background(10);
      fill(20);
      rect(32, 32, 362, 68+sensorLabels.length*40, 8);
      fill(200);
      textSize(36);
      text("Raw Data Inputs", 55, 78);
      textSize(24);
      fill(255);
      for (int i=0; i<sensorData.length; i++) {
        text(sensorLabels[i], 44, 124+40*i);
        text(sensorUnits[i], 325, 124+40*i);
        text((sensorData[i]/sensorMultipliers[i]), 200, 124+40*i);
        table.setFloat(table.getRowCount(), sensorLabels[i], sensorData[i]);
      }
      if (saveCounter>20) {
        saveTable(table, filename);
        text("saving", 600, 600);
        saveCounter = 0;
      }
    } 
    }
  }
