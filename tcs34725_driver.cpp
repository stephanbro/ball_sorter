#include <stdint.h>
#include <math.h>
#include "tcs34725_driver.hpp"

#define TCS34725_ADDRESS     0x29

#define TCS34725_COMMAND_BIT 0x80

#define TCS34725_RA_ENABLE   0x00 // Enables states and interrupts
#define TCS34725_ENABLE_AIEN 0x10 // RGBC Interrupt enable
#define TCS34725_ENABLE_WEN  0x08 // Wait enable - Writing 1 actives the wait timer
#define TCS34725_ENABLE_AEN  0x02 // RGBC Enable - Writing 1 actives the ADC, 0 disables it
#define TCS34725_ENABLE_PON  0x01 // Power on- Writing 1 activates the internal oscillator, 0 disables it

#define TCS34725_RA_ATIME    0x01 // RGBC time
#define TCS34725_RA_WTIME    0x03 // Wait time

#define TCS34725_RA_CONFIG   0x0D // Configuration
#define TCS34725_RA_CONTROL  0x0F // Control

#define TCS34725_RA_WIA      0x12 // Who Am I/ID
#define TCS34725_WIA_VAL     0x44 // Who Am I/ID value

#define TCS34725_RA_CDATAL   0x14 // Clear channel data low
#define TCS34725_RA_CDATAH   0x15 // Clear channel data high
#define TCS34725_RA_RDATAL   0x16 // Red channel data low
#define TCS34725_RA_RDATAH   0x17 // Red channel data high
#define TCS34725_RA_GDATAL   0x18 // Green channel data low
#define TCS34725_RA_GDATAH   0x19 // Green channel data high
#define TCS34725_RA_BDATAL   0x1A // Blue channel data low
#define TCS34725_RA_BDATAH   0x1B // Blue channel data high


TCS34725::TCS34725() :
  m_is_inited(false),
  m_red_last(0),
  m_green_last(0),
  m_blue_last(0),
  m_clear_last(0),
  m_rgb_max(0)
{ }

TCS34725::~TCS34725() { }

void TCS34725::init(void)
{
  if (m_is_inited == true) {
    return;
  }
  uint8_t val = read8(TCS34725_RA_WIA);
  m_is_inited = (val == 0x44);
  enable();
}

void TCS34725::enable(void)
{
  if (m_is_inited == false) {
    return;
  }
  write8(TCS34725_RA_ENABLE, TCS34725_ENABLE_PON);
  //delay(3);
  write8(TCS34725_RA_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

void TCS34725::disable(void)
{
  if (m_is_inited == false) {
    return;
  }
  uint8_t val = read8(TCS34725_RA_ENABLE);
  write8(TCS34725_RA_ENABLE, val & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

void TCS34725::update_color_values(void)
{
  if (m_is_inited == false) {
    return;
  }

  m_clear_last = read16(TCS34725_RA_CDATAL);
  uint16_t red = read16(TCS34725_RA_RDATAL);
  uint16_t blu = read16(TCS34725_RA_BDATAL);
  uint16_t grn = read16(TCS34725_RA_GDATAL);

  // Set color max
  m_rgb_max = red;
  if (m_rgb_max < blu) {
    m_rgb_max = blu;
  }
  if (m_rgb_max < grn) {
    m_rgb_max = grn;
  }

  m_red_last   = (uint8_t)(((float)red / (float)m_rgb_max)*255);
  m_blue_last  = (uint8_t)(((float)blu / (float)m_rgb_max)*255);
  m_green_last = (uint8_t)(((float)grn / (float)m_rgb_max)*255);
}

TCS34725::color_t TCS34725::get_color(void)
{
  if (m_clear_last < 2000) {
    return COLOR_MAX;
  }

  color_t ret = COLOR_MAX;
  uint16_t d  = 65535;
  for (uint8_t i = 0; i < COLOR_MAX; i++) {
    uint16_t new_d = pow((m_red_last   - colorIface::rgb_color[i][0])*0.30f, 2) +
                     pow((m_green_last - colorIface::rgb_color[i][1])*0.59f, 2) +
                     pow((m_blue_last  - colorIface::rgb_color[i][2])*0.11f, 2);
    if (d > new_d) {
      d = new_d;
      ret = (color_t)i;
    }
  }
  return ret;
}

uint8_t  TCS34725::read8(uint8_t reg)
{
  return read8_cb(TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg);
}
uint16_t TCS34725::read16(uint8_t reg)
{
  return read16_cb(TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg);
}

void     TCS34725::write8(uint8_t reg, uint8_t val)
{
  write8_cb(TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg, val);
}

