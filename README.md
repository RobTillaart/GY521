# GY521

Arduino library for I2C GY521 accelerometer-gyroscope sensor a.k.a. MCU-6050

## Description

Experimental library for GY521 a.k.a. MCU-6050

Library is work in progress, in fact extracted and extended from an old project.
It needs to be tested a lot more.

It has three examples
- calibration example to determine the offsets needed
- example to read values.
- test sketch to test get / set values.

## Calibration (short version for now)

1. load and run calibration example  
   it shows a header containing 6 numbers and 10 lines of 8 numbers
1. wait until the middle 6 of the longer lines stabilize (should all be 0)
1. copy the 6 numbers above the axe aye aze as these are the numbers needed.


## Future

**Should**
- test test and test ...(ESP too)
- improve documentation
- look for math optimizations  (atan, hypot, performance)

**Could**
- calibrate function in the lib ?  (think not as lib might grow?)
- calibrate sketch could print code snippet to include...


## documents

- check details - MPU-6000-Register-Map1.pdf

## Operation

See examples, use with care 

