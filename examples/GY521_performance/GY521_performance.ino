//
//    FILE: GY521_performance.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: minimal demo
//    DATE: 2021-06-13

//#define SWIRE
#include "GY521.h"

GY521 sensor(0x68);

uint32_t counter = 0;


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);

#ifdef SWIRE
  sensor.begin(A3,A4);
#else
  Wire.begin();
#endif

  delay(100);
  while (sensor.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
}

void loop()
{
  uint32_t start = micros();
  int16_t x = sensor.read();
  uint32_t duration = micros() - start;
  
  Serial.print(x);
  Serial.print('\t');
  Serial.print(duration);
  Serial.println();
  
  delay(1000);
}

// -- END OF FILE --
