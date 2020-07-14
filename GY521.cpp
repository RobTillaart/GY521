//
//    FILE: GY521.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.1
// PURPOSE: Arduino library for I2C GY521 accelerometer-gyroscope sensor
//     URL: https://github.com/RobTillaart/GY521
//
// HISTORY:
// 0.1.0    2017-11-20 initial version
// 0.1.1    2020-07-09 refactor + initial release

#include "Wire.h"
#include "GY521.h"

// keep names in sync with BIG MPU6050 lib
#define GY521_GYRO_CONFIG     0x1B
#define GY521_ACCEL_CONFIG    0x1C

#define GY521_ACCEL_XOUT_H    0x3B
#define GY521_TEMP_OUT_H      0x41
#define GY521_GYRO_XOUT_H     0x43

#define GY521_PWR_MGMT_1      0x6B
#define GY521_WAKEUP          0x00

#define RAD2DEGREES           (180.0 / PI)


/////////////////////////////////////////////////////
//
// PUBLIC
//
GY521::GY521(uint8_t address)
{
  _address = 0x69;
  if (address == 0x68) _address = 0x68;
}

bool GY521::wakeup()
{
  Wire.beginTransmission(_address);
  Wire.write(GY521_PWR_MGMT_1);
  Wire.write(GY521_WAKEUP);
  return Wire.endTransmission() == 0;
}


int GY521::read()
{
  if (_throttle)
  {
    if ((millis() - _lastTime) < GY521_THROTTLE_TIME) return GY521_THROTTLED;
  }

  // Connected ?
  Wire.beginTransmission(_address);
  Wire.write(GY521_ACCEL_XOUT_H);
  if (Wire.endTransmission() != 0) return GY521_ERROR_READ;

  // Get the data
  Wire.requestFrom(_address, (uint8_t)14);
  // ACCELEROMETER
  _ax = ( ( ((int)Wire.read()) << 8) | Wire.read() );  // ACCEL_XOUT_H  ACCEL_XOUT_L
  _ay = ( ( ((int)Wire.read()) << 8) | Wire.read() );  // ACCEL_YOUT_H  ACCEL_YOUT_L
  _az = ( ( ((int)Wire.read()) << 8) | Wire.read() );  // ACCEL_ZOUT_H  ACCEL_ZOUT_L
  // TEMPERATURE
  _temperature = ( ((int)Wire.read()) << 8) | Wire.read();  // TEMP_OUT_H    TEMP_OUT_L
  // GYROSCOPE
  _gx = (( ((int)Wire.read()) << 8) | Wire.read());  // GYRO_XOUT_H   GYRO_XOUT_L
  _gy = (( ((int)Wire.read()) << 8) | Wire.read());  // GYRO_YOUT_H   GYRO_YOUT_L
  _gz = (( ((int)Wire.read()) << 8) | Wire.read());  // GYRO_ZOUT_H   GYRO_ZOUT_L

  // time interval
  uint32_t now = millis();
  float duration = (now - _lastTime) * 0.001;   // time in seconds.
  _lastTime = now;

  // Convert raw acceleration to g's
  _ax *= _raw2g;
  _ay *= _raw2g;
  _az *= _raw2g;

  // prepare for Pitch Roll Yaw
  _aax = atan(_ay / hypot(_ax, _az)) * RAD2DEGREES;
  _aay = atan(-1.0 * _ax / hypot(_ay, _az)) * RAD2DEGREES;
  _aaz = atan(_az / hypot(_ax, _ay)) * RAD2DEGREES;

  // Error correct the angles
  _aax += axe;
  _aay += aye;
  _aaz += aze;

  // Convert to Celsius
  _temperature = (_temperature + 12412.0) * 0.00294117647;  //  == /340.0  + 36.53;

  // Convert raw Gyro to degrees/seconds
  _gx *= _raw2dps;
  _gy *= _raw2dps;
  _gz *= _raw2dps;
  
  // Error correct raw gyro measurements.
  _gx += gxe;
  _gy += gye;
  _gz += gze;
  
  _gax += _gx * duration;
  _gay += _gy * duration;
  _gaz += _gz * duration;

  _yaw = _gaz;
  _pitch = 0.96 * _gay + 0.04 * _aay;
  _roll = 0.96 * _gax + 0.04 * _aax;

  return GY521_OK;
}

void GY521::setAccelSensitivity(uint8_t as)
{
  if (as > 3) as = 3;
  uint8_t val = getRegister(GY521_ACCEL_CONFIG);
  val &= 0xE7;
  val |= (as << 3);
  setRegister(GY521_ACCEL_CONFIG, val);
  // calculate conversion factor.
  _raw2g = 16384.0;
  for (uint8_t i = 0; i < _afs; i++) _raw2g *= 0.5;
  _raw2g = 1.0 / _raw2g;
}

void  GY521::setGyroSensitivity(uint8_t gs)
{
  _gfs = gs;
  if (_gfs > 3) _gfs = 3;
  uint8_t val = getRegister(GY521_GYRO_CONFIG);
  val &= 0xE7;
  val |= (_gfs << 3);
  setRegister(GY521_GYRO_CONFIG, val);
    _raw2dps = 131.0;
  for (uint8_t i = 0; i < _gfs; i++) _raw2dps *= 0.5;
  _raw2dps = 1.0 / _raw2dps;
}

uint8_t GY521::setRegister(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(value);
  // no need to do anything if not connected.
  if (Wire.endTransmission() != 0) return GY521_ERROR_WRITE;
  return GY521_OK;
}

uint8_t GY521::getRegister(uint8_t reg)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return GY521_ERROR_WRITE;
  Wire.requestFrom(_address, (uint8_t) 1);
  uint8_t val = Wire.read();
  return val;
}

// END OF FILE
