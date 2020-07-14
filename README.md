# GY521

Arduino library for I2C GY521 accelerometer-gyroscope sensor a.k.a. MCU-6050

## Description

Experimental library for GY521 a.k.a. MCU-6050

Library is work in progress, in fact extracted and extended from an old project.
It needs to be tested a lot more.

It has two examples
- calibration example to determine the offsets needed
- example to read values.

## Calibration (short version for now)

1. load and run calibration example  
   it shows a header containing 6 numbers and 10 lines og 8 numbers
1. wait until the middle 6 of the longer lines stabilize (should be all 0)
1. copy the 6 numbers above the axe aye aze as these are the numbers needed.


## Future

- test test and test ...(ESP too)
- pitch roll yaw example
- better documentation
- look for math optimizations  (atan/hypot)
- calibrate function in the lib ?
- ESP wire support (other pins)
- ...

## documents

- check details - MPU-6000-Register-Map1.pdf

## Operation

See examples, use with care 

