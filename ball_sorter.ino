
// Standard includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Arduino includes
#include <Wire.h>

#include "drivers/color_iface.hpp"
#include "drivers/tcs34725_driver.hpp"
#include "modules/gate_controller.hpp"

// These are pointers because there is no obvious
// access to constructors in arduino
static colorIface* g_color_sensor;
static GateController* g_gate_controller;

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

void init_failed(void)
{
  while (true) {
    Serial.println("Init failed!\n");
    delay(1000);
  }
}

void setup()
{
  // Setup I2C
  Wire.begin();

  // Setup debug console over usb
  Serial.begin(115200);

  // Setup color sensor
  g_color_sensor = new TCS34725(i2c_read8, i2c_read16, i2c_write8);
  if (g_color_sensor->init() == false) {
    init_failed();
  }
  g_color_sensor->enable();

  g_gate_controller = new GateController(digitalWrite);

}

void loop()
{
  const char* color = colorIface::rgb_string[g_color_sensor->get_color()];
  Serial.println(color);
  g_gate_controller->tick(GateController::SLOT1);
  delay(200);
}

