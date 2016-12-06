
#ifndef _COLOR_IFACE_HPP_
#define _COLOR_IFACE_HPP_

#include <stdint.h>

class colorIface
{
  public:
    colorIface(uint8_t (*read8_in)(uint8_t, uint8_t),
        uint16_t (*read16_in)(uint8_t, uint8_t),
        void (*write8_in)(uint8_t, uint8_t, uint8_t)) :
      read8_cb(read8_in),
      read16_cb(read16_in),
      write8_cb(write8_in)
    { }

    virtual ~colorIface() {}

    typedef enum COLOR {
      BLACK, WHITE, RED, LIME, BLUE, YELLOW,
      CYAN, MAGENTA, SILVER, GRAY, MAROON, OLIIVE,
      GREEN, PURPLE, TEAL, NAVY, ORANGE,
      COLOR_MAX
    } color_t;

    static const uint8_t rgb_color[COLOR_MAX][3];
    static const char* rgb_string[COLOR_MAX+1];

    virtual bool init() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual color_t get_color() = 0;

  protected:
    uint8_t  (*read8_cb)(uint8_t, uint8_t);
    uint16_t (*read16_cb)(uint8_t, uint8_t);
    void     (*write8_cb)(uint8_t, uint8_t, uint8_t);
};

#endif /* _COLOR_IFACE_HPP_ */

