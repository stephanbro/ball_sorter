
// Standard includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Arduino includes
#include <Wire.h>

#include "tcs34725_driver.hpp"

static TCS34725 g_tcs34725;

void i2c_read(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data)
{
  // Set register to read from
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  // Read bytes
  Wire.requestFrom(address, len);
  for (uint8_t i = 0; i < len; i++) {
    *(data+i) = Wire.read();
  }
}

void i2c_write(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data)
{
  // Set register to write to
  Wire.beginTransmission(address);
  Wire.write(reg);
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(*(data+i));
  }
  Wire.endTransmission();
}

uint8_t  i2c_read8( uint8_t address, uint8_t reg)
{
  uint8_t data = 0;
  i2c_read(address, reg, 1, &data);
  return data;
}

uint16_t i2c_read16(uint8_t address, uint8_t reg)
{
  uint8_t data[2] = {};
  i2c_read(address, reg, 2, (uint8_t*)&data);
  return (uint16_t)(data[1] << 8) | (data[0]);
}

void    i2c_write8( uint8_t address, uint8_t reg, uint8_t data)
{
  i2c_write(address, reg, 1, (uint8_t*)&data);
}

void setup()
{
  // Setup I2C
  Wire.begin();

  // Setup debug console over usb
  Serial.begin(115200);
  g_tcs34725.i2c_read8_cb  = i2c_read8;
  g_tcs34725.i2c_read16_cb = i2c_read16;
  g_tcs34725.i2c_write8_cb = i2c_write8;
  g_tcs34725.init();
  g_tcs34725.enable();

}

void loop()
{
  g_tcs34725.update_color_values();
  Serial.println(TCS34725::rgb_string[g_tcs34725.get_color()]);
  delay(1000);
}

