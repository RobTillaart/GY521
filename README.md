
[![Arduino CI](https://github.com/RobTillaart/GY521/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/GY521/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/GY521/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/GY521/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/GY521/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/GY521/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/GY521.svg?maxAge=3600)](https://github.com/RobTillaart/GY521/releases)

# GY521

Arduino library for I2C GY521 accelerometer-gyroscope sensor a.k.a. MCU-6050


## Description

Experimental library for GY521 a.k.a. MCU-6050

Library is work in progress, in fact it is extracted and extended from an old project.
It needs to be tested a lot more.


#### Examples

- calibration example to determine the offsets needed
- example to read values.
- test sketch to test get / set values.
- example to get pitch roll yaw.
- performance sketch.


## Breakout board

From left to right

|  pin | pinName | description     |
|:----:|:--------|:----------------|
|  0   |  VCC    | +5V             |
|  1   |  GND    | ground          |
|  2   |  SCL    | I2C clock       |
|  3   |  SDA    | I2C data        |
|  4   |  XDA    | auxiliary data  | see datasheet
|  5   |  XCL    | auxiliary clock | see datasheet
|  6   |  AD0    | address         | 
|  7   |  INT    | interrupt       |


#### Address

AD0 connected to GND => 0x68 
AD0 connected to VCC => 0x69


## Calibration (short version for now)

1. load and run calibration example  
   it shows a header containing 6 numbers and 10 lines of 8 numbers
1. wait until the middle 6 of the longer lines stabilize (should all be 0)
1. copy the 6 numbers above the axe aye aze as these are the numbers needed.


## Interface


### Constructor

- **GY521(uint8_t address = 0x69, , TwoWire \*wire = &Wire)** Constructor with default address. 
0x68 is also a valid address. The wire argument is optional to select Wire1 Wire2 etc. on some boards.
- **bool begin(uint8_t sda, uint8_t scl)** begin for ESP32 et al. Returns true if address can be found on I2C bus.
- **bool begin()** Returns true if address can be found on I2C bus.
- **bool isConnected()** returns true if device can be found on I2C bus.
- **void reset()** set all internal values to 0 and throttle time to 10 ms.
- **bool wakeUp()**


### Throttle

- **void setThrottle(bool throttle = true)** throttle to force "delay" between reads.
- **bool getThrottle()** returns true if throttle mode is set.
- **void setThrottleTime(uint16_t ti )** throttle time in milliseconds, max = 65535 = 1++ minute
- **uint16_t getThrottleTime()** returns throttle time set.


### READ

#### Set before read

- **bool setAccelSensitivity(uint8_t as)** as = 0, 1, 2, 3 ==> 2g 4g 8g 16g
- **uint8_t getAccelSensitivity()** returns 0, 1, 2, 3
- **bool setGyroSensitivity(uint8_t gs)** gs = 0,1,2,3  ==>  250, 500, 1000, 2000 degrees/second
- **uint8_t  getGyroSensitivity()** returns 0, 1, 2, 3  


#### Actual read

- **int16_t read()** returns ...
- **uint32_t lastTime()** last time sensor is actually read. In milliseconds.


#### Call after read

Note that multiple calls will return the same value. One must explicitly call **read()** to get new values. 

- **float getAccelX()** idem
- **float getAccelY()** idem
- **float getAccelZ()** idem
- **float getAngleX()** idem
- **float getAngleY()** idem
- **float getAngleZ()** idem
- **float getTemperature()** idem
- **float getGyroX()** idem
- **float getGyroY()** idem
- **float getGyroZ()** idem
- **float getPitch()** idem
- **float getRoll()** idem
- **float getYaw()** idem


### Register access

Read the register PDF for the specific value and meaning of registers.

- **uint8_t setRegister(uint8_t reg, uint8_t value)**
- **uint8_t getRegister(uint8_t reg)**


## Future

**Should**
- test test and test ...(ESP too)
- improve documentation
- look for maths optimizations  (atan, hypot, performance)


**Could**
- calibrate function in the lib ?  (think not as lib might grow?)
- calibrate sketch could print code snippet to include...
- option to read only Accel?
- option to read only Gyro?

## documents

- check details - MPU-6000-Register-Map1.pdf


## Operation

See examples, use with care 

