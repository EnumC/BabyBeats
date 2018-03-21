#include <max6675.h>

/*************************************************************
  Using:
  Blync
  MLX90614

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


/*#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <BlynkSimpleEsp8266.h>
*/ //Disabled for Main Processing Board
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "2211a13ba6ed4254a8b3b9a0127dc56c";
//WiFiManager wifiManager;

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

/*
   The format of our output.

   Set this to PROCESSING_VISUALIZER if you're going to run
    the Processing Visualizer Sketch.
    See https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer

   Set this to SERIAL_PLOTTER if you're going to run
    the Arduino IDE's Serial Plotter.
*/
const int OUTPUT_TYPE = SERIAL_PLOTTER;
const int PIN_INPUT = A0;
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle

PulseSensorPlayground pulseSensor;
SoftwareSerial serialCom(8, 9); // RX, TX


void setup()
{
  
  // Debug console
  Serial.begin(9600);
  while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
  Serial.println("Serial Live!!!!!!!!");
  serialCom.begin(2400);
  

  /*wifiManager.autoConnect("BabyBeatsPrototype");

  WiFi.printDiag(Serial);

  Blynk.config(auth); */

  Serial.println("Initializing MLX90614");
  mlx.begin(); //Temp Sensor
  pulseSensor.analogInput(PIN_INPUT);
  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);
  Serial.println("Initializing MAX6675");
  if (!pulseSensor.begin()) {
    Serial.println("ERROR: Pulse Sensor Failed To Initialize. Perhaps Interrupts are not supported?!");
  }
  else
    Serial.println("Initialization Complete!");
}

void loop()
{
  //Blynk.run();
  
  serialCom.print(mlx.readAmbientTempC()); 
  serialCom.print(" ");
  serialCom.print(mlx.readObjectTempC()); 
  serialCom.print(" ");
  //Serial.println("*C");
  double ambientF =  (mlx.readAmbientTempF()); 
  double objectF =  (mlx.readObjectTempF()); 
  //Serial.println("*F");
  //Blynk_Delay(20);
  delay(200);
   // write the latest sample to Serial.
 //pulseSensor.outputSample();
 if (pulseSensor.sawStartOfBeat()) {
   //pulseSensor.outputBeat();
  }
  if (thermocouple.readCelsius() >0) {
    serialCom.print(thermocouple.readCelsius());
  }
   serialCom.print(" ");
   //Serial.print("F = ");
   //Serial.print(thermocouple.readFahrenheit());
   serialCom.println();
}

/* Blynk_Delay(int milli)
{
  int end_time = millis() + milli;
  while (millis() < end_time)
  {
    if (Blynk.connected())
    {
      Blynk.run();
    }
    yield();
  }
}*/

