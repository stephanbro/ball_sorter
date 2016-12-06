
#ifndef _TCS34725_DRIVER_HPP_
#define _TCS34725_DRIVER_HPP_

#include "color_iface.hpp"

// Sensor class
class TCS34725 : public colorIface
{
  public:
    // Constructor
    TCS34725(uint8_t (*read8_in)(uint8_t, uint8_t),
      uint16_t (*read16_in)(uint8_t, uint8_t),
      void (*write8_in)(uint8_t, uint8_t, uint8_t));
    ~TCS34725(); // Destructor

    virtual void init(void);

    virtual void enable(void);

    virtual void disable(void);

    void update_color_values(void);

    virtual color_t get_color(void);

    static const uint8_t rgb_color[COLOR_MAX][3];
    static const char* rgb_string[COLOR_MAX+1];

  private:
    uint8_t  read8(uint8_t reg);
    uint16_t read16(uint8_t reg);

    void     write8(uint8_t reg, uint8_t val);

    // Is driver inited?
    bool     m_is_inited;
    // Last color values
    uint8_t  m_red_last;
    uint8_t  m_green_last;
    uint8_t  m_blue_last;
    uint16_t m_clear_last;
    uint16_t m_rgb_max;
};

#endif /* _TCS34725_DRIVER_HPP_ */

