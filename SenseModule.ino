// SenseModule - Module of Digital Barrier Proximity Sensor based on Arduino and TOF
#include <Wire.h>
// Pololu Github - https://github.com/pololu/vl53l0x-arduino
#include <VL53L0X.h>

// --- define debug print to Serial
#define Debug

// --- define PINS
#define LED_PIN 13
#define VCC_PIN 4
#define OUT_PIN 5

// --- define Variables
int Distance_mm = 0;
int Barrier_mm = 1000;
int Distance_D = 0;
int Distance_AVG = 0;
int BuffSize = 10;
int BuffQueue[10];
char strDuff[20];

// --- define Objects
VL53L0X sensor;

int buffAdd(int item)
{
  //shift BuffQueue
  int i;
  for ( i = 1; i < BuffSize; i=i+1) 
  {
    //Serial.print(BuffSize-i); Serial.print("-");Serial.print(BuffQueue[BuffSize-i]); Serial.print("=");  
    //Serial.print(BuffSize-i-1); Serial.print("-");Serial.println(BuffQueue[BuffSize-i-1]);  
    BuffQueue[BuffSize-i] = BuffQueue[BuffSize-i-1];
  }
  //add new Item
  BuffQueue[0] = item;
  //calc AVG
  int avg = 0;
  for ( i = 0; i < BuffSize; i++) 
  {
    //Serial.print(i); Serial.print("-");Serial.println(BuffQueue[i]);  
    avg += BuffQueue[i];
  }
  
  return avg / BuffSize;
}

void setup()
{
  #ifdef Debug
    Serial.begin(115200);
    Serial.println("Start setup()");
  #endif 
  pinMode(LED_PIN, OUTPUT);  // LED PIN init
  digitalWrite( LED_PIN, LOW);
  pinMode(OUT_PIN, OUTPUT);  // OUT PIN init
  digitalWrite( OUT_PIN, LOW);
  pinMode(VCC_PIN, OUTPUT);  // VCC PIN init
  digitalWrite( VCC_PIN, HIGH);
  
  #ifdef Debug
    Serial.println("Start init Sensor");
  #endif 
  Wire.begin();

  // init VL53L0X Object
  sensor.init();
  sensor.setTimeout(500);

  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);

  // Start continuous back-to-back mode
  sensor.startContinuous();
  #ifdef Debug
    Serial.println("Finish setup()");
  #endif
}

void loop()
{
  Distance_mm = sensor.readRangeContinuousMillimeters();
  if (Distance_mm > 2000)
    { 
      Distance_mm = 2000;
    };
  Distance_AVG = buffAdd(Distance_mm);
  Distance_D = Distance_AVG < Barrier_mm;
  digitalWrite( OUT_PIN, Distance_D);
  digitalWrite( LED_PIN, Distance_D);

  #ifdef Debug
    sprintf(strDuff, "RAW %4dmm AVG %4dmm %1d D - %d", Distance_mm, Distance_AVG, Distance_D, millis());
    Serial.println(strDuff);  
  #endif   
}
