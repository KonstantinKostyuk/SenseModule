// SenseModule - Module of Digital Barrier Proximity Sensor based on Arduino and TOF
#include <Wire.h>
// Pololu Github - https://github.com/pololu/vl53l0x-arduino
#include <VL53L0X.h>

// --- define PINS
#define LED_PIN 13
#define OUT_PIN 11

// --- define Variables
int Distance_mm = 0;
int Barrier_mm = 1000;
int Distance_D = 0;

// --- define Objects
VL53L0X sensor;


void setup()
{
  pinMode(LED_PIN, OUTPUT);  // LED PIN init
  digitalWrite( LED_PIN, LOW);
  pinMode(OUT_PIN, OUTPUT);  // OUT PIN init
  digitalWrite( OUT_PIN, LOW);
  
  Serial.begin(115200);
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
}

void loop()
{
  Distance_mm = sensor.readRangeSingleMillimeters();
  Distance_D = Distance_mm < Barrier_mm;
  digitalWrite( OUT_PIN, Distance_D);
  digitalWrite( LED_PIN, Distance_D);
  
  if (sensor.timeoutOccurred()) 
    { Serial.print("Sensor TIMEOUT"); }
    else
    { Serial.print(millis());
      Serial.print(" - ");
      Serial.print(Distance_mm);
      Serial.print(" mm - ");
      Serial.print(Distance_D);
      Serial.print(" D");
      Serial.println();
    }
}
