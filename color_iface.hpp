#include <stdint.h>

class colorIface
{
  public:
    typedef enum COLOR {
      BLACK, WHITE, RED, LIME, BLUE, YELLOW,
      CYAN, MAGENTA, SILVER, GRAY, MAROON, OLIIVE,
      GREEN, PURPLE, TEAL, NAVY, ORANGE,
      COLOR_MAX
    } color_t;

    static const uint8_t rgb_color[COLOR_MAX][3];
    static const char* rgb_string[COLOR_MAX+1];

    virtual void init();
    virtual void enable();
    virtual void disable();
    virtual color_t get_color();

};