#include <stdint.h>
#include <math.h>
#include "tcs34725_driver.hpp"

#define TCS34725_ADDRESS     0x29

#define TCS34725_COMMAND_BIT 0x80

TCS34725::TCS34725(uint8_t (*read8_in)(uint8_t, uint8_t),
    uint16_t (*read16_in)(uint8_t, uint8_t),
    void (*write8_in)(uint8_t, uint8_t, uint8_t)) :
  colorIface(read8_in, read16_in, write8_in),
  m_is_inited(false),
  m_red_last(0),
  m_green_last(0),
  m_blue_last(0),
  m_clear_last(0),
  m_rgb_max(0)
{ }

TCS34725::~TCS34725() { }

bool TCS34725::init(void)
{
  if (m_is_inited == true) {
    return true;
  }
  uint8_t val = read8(TCS34725_RA_WIA);
  m_is_inited = (val == 0x44);
  return m_is_inited;
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

  // Get color magnitude
  m_rgb_max = sqrt(pow(red, 2) + pow(blu, 2) + pow(grn, 2));

  // Normalize rgb values
  m_red_last   = (uint8_t)(((float)red / (float)m_rgb_max)*255);
  m_blue_last  = (uint8_t)(((float)blu / (float)m_rgb_max)*255);
  m_green_last = (uint8_t)(((float)grn / (float)m_rgb_max)*255);
}

TCS34725::color_t TCS34725::get_color(void)
{
  update_color_values();
  // TODO: Make this configurable
  if (m_clear_last < 5000) {
    return COLOR_MAX;
  }

  color_t ret = COLOR_MAX;
  uint16_t d  = UINT16_MAX;
  // Iterate through each color to find which one is the closest
  // to what is currently being seen. Adjustment multipliers are
  // for more closley matching how the human eye sees them.
  // http://stackoverflow.com/questions/1847092/given-an-rgb-value-what-would-be-the-best-way-to-find-the-closest-match-in-the-d

  for (uint8_t i = 0; i < COLOR_MAX; i++) {
    uint16_t new_d = pow((m_red_last   - colorIface::rgb_color[i][0])*0.30f, 2)+
                     pow((m_green_last - colorIface::rgb_color[i][1])*0.59f, 2)+
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

