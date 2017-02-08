#include <Arduino.h>

// Standard includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Arduino includes
#include <Wire.h>

// Drivers and Modules
#include "drivers/color_iface.hpp"
#include "drivers/tcs34725_driver.hpp"
#include "modules/gate_controller.hpp"

// These are pointers because there is no obvious
// way to pass constructor arguments in arduino
static colorIface* g_color_sensor;
static GateController* g_gate_controller;

// Convert from color to gate number, GATE_MAX means not handled
static const GateController::gates_t color_to_gate[colorIface::COLOR_MAX] = {
  GateController::GATE_MAX, // Black
  GateController::GATE_MAX, // White
  GateController::GATE_MAX, // Red
  GateController::GATE3,    // Lime
  GateController::GATE_MAX, // Blue
  GateController::GATE1,    // Yellow
  GateController::GATE_MAX, // Cyan
  GateController::GATE_MAX, // Magenta
  GateController::GATE1, // Silver
  GateController::GATE_MAX, // Gray
  GateController::GATE_MAX, // Maroon
  GateController::GATE_MAX, // Olive
  GateController::GATE_MAX, // Green
  GateController::GATE_MAX, // Purple
  GateController::GATE_MAX, // Teal
  GateController::GATE_MAX, // Navy
  GateController::GATE2,    // Orange
};

static void i2c_read(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data);
static void i2c_write(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data);
static uint8_t  i2c_read8( uint8_t address, uint8_t reg);
static uint16_t i2c_read16(uint8_t address, uint8_t reg);
static void    i2c_write8( uint8_t address, uint8_t reg, uint8_t data);
static void init_failed(void);
static void gate_out(uint8_t pin, uint8_t output);

// I2C handling code
// TODO: Move to a HAL?
static void i2c_read(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data)
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

static void i2c_write(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data)
{
  // Set register to write to
  Wire.beginTransmission(address);
  Wire.write(reg);
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(*(data+i));
  }
  Wire.endTransmission();
}

static uint8_t  i2c_read8( uint8_t address, uint8_t reg)
{
  uint8_t data = 0;
  i2c_read(address, reg, 1, &data);
  return data;
}

static uint16_t i2c_read16(uint8_t address, uint8_t reg)
{
  uint8_t data[2] = {};
  i2c_read(address, reg, 2, (uint8_t*)&data);
  return (uint16_t)(data[1] << 8) | (data[0]);
}

static void    i2c_write8( uint8_t address, uint8_t reg, uint8_t data)
{
  i2c_write(address, reg, 1, (uint8_t*)&data);
}

static void init_failed(void)
{
  while (true) {
    Serial.println("Init failed!");
    delay(1000);
  }
}

static void gate_out(uint8_t pin, uint8_t output)
{
  digitalWrite(pin+4, output);
}

void setup()
{
  // Setup I2C
  Wire.begin();

  // Setup debug console over usb
  Serial.begin(115200);

  // Setup color sensor
  g_color_sensor = new TCS34725(i2c_read8, i2c_read16, i2c_write8);
  if (g_color_sensor == nullptr || g_color_sensor->init() == false) {
      init_failed();
  } else {
    g_color_sensor->enable();
  }

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  g_gate_controller = new GateController(gate_out);
}

void loop()
{
  colorIface::color_t color = g_color_sensor->get_color();
  Serial.println(colorIface::rgb_string[color]);
  g_gate_controller->tick(color_to_gate[color]);
  delay(50);
}

