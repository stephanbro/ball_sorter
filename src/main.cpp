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

// Some #defines
#define COLOR_SENSOR_INTERVAL     10  // in ms
#define GATE_CONTROLLER_INTERVAL  1   // in ms
#define GATE_SENSING_OUTPUT_PIN   4
#define GATE1_OUTPUT_PIN          5
#define GATE2_OUTPUT_PIN          6

// These are pointers because there is no obvious
// way to pass constructor arguments in arduino
static colorIface* g_color_sensor;
static GateController* g_gate_controller;

// Convert from color to gate number, GATE_MAX means not handled
static const GateController::gates_t color_to_gate[colorIface::COLOR_MAX+1] = {
  GateController::GATE_MAX, // Black
  GateController::GATE_MAX, // White
  GateController::GATE_MAX, // Red
  GateController::GATE3,    // Lime
  GateController::GATE_MAX, // Blue
  GateController::GATE1,    // Yellow
  GateController::GATE_MAX, // Cyan
  GateController::GATE_MAX, // Magenta
  GateController::GATE1,    // Silver (looks like Yellow for some reason)
  GateController::GATE_MAX, // Gray
  GateController::GATE_MAX, // Maroon
  GateController::GATE_MAX, // Olive
  GateController::GATE_MAX, // Green
  GateController::GATE_MAX, // Purple
  GateController::GATE_MAX, // Teal
  GateController::GATE_MAX, // Navy
  GateController::GATE2,    // Orange
  GateController::GATE_MAX, // COLOR_MAX
};

static void     i2c_read(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data);
static void     i2c_write(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data);
static uint8_t  i2c_read8( uint8_t address, uint8_t reg);
static uint16_t i2c_read16(uint8_t address, uint8_t reg);
static void     i2c_write8( uint8_t address, uint8_t reg, uint8_t data);
static void     init_failed(void);
static void     gate_out(uint8_t pin, uint8_t output);

// I2C handling code
// TODO: Move to a HAL?
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

uint8_t i2c_read8( uint8_t address, uint8_t reg)
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

void i2c_write8( uint8_t address, uint8_t reg, uint8_t data)
{
  i2c_write(address, reg, 1, (uint8_t*)&data);
}

void init_failed(void)
{
  while (true) {
    Serial.println("Init failed!");
    delay(1000);
  }
}

void gate_out(uint8_t pin, uint8_t output)
{
  digitalWrite(pin+4, output);
}

void setup()
{
  // Setup I2C
  Wire.begin();

  // Setup debug console over usb
  Serial.begin(115200);

  g_color_sensor = new TCS34725(i2c_read8, i2c_read16, i2c_write8);
  if (g_color_sensor == nullptr || g_color_sensor->init() == false) {
      init_failed();
  } else {
    g_color_sensor->enable();
  }

  pinMode(GATE_SENSING_OUTPUT_PIN,  OUTPUT);
  pinMode(GATE1_OUTPUT_PIN,         OUTPUT);
  pinMode(GATE2_OUTPUT_PIN,         OUTPUT);

  g_gate_controller = new GateController(gate_out);
}

void loop()
{
  unsigned long time = millis();
  static unsigned long color_sensor_last_call = time;
  static unsigned long gate_controller_last_call = time;

  colorIface::color_t color = colorIface::COLOR_MAX;

  // Color sensor
  if (time < color_sensor_last_call || // Handle rollover simply
      (time - color_sensor_last_call) > COLOR_SENSOR_INTERVAL) {
    color_sensor_last_call = time;
    color = g_color_sensor->get_color();
    Serial.println(colorIface::rgb_string[color]);
  }

  // Gate controller
  if (time < gate_controller_last_call || // Handle rollover simply
      (time - gate_controller_last_call) > GATE_CONTROLLER_INTERVAL) {
    gate_controller_last_call = time;
    g_gate_controller->tick(color_to_gate[color]);
  }
}

