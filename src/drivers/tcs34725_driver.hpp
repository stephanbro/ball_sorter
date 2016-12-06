
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

    virtual bool init(void) override;
    virtual void enable(void) override;
    virtual void disable(void) override;
    virtual color_t get_color(void) override;

    static const uint8_t rgb_color[COLOR_MAX][3];
    static const char* rgb_string[COLOR_MAX+1];

  private:
    // Registers
    enum : uint8_t {
      TCS34725_RA_ENABLE   = 0x00, // Enables states and interrupts
      TCS34725_RA_ATIME    = 0x01, // RGBC time
      TCS34725_RA_WTIME    = 0x03, // Wait time

      TCS34725_RA_CONFIG   = 0x0D, // Configuration
      TCS34725_RA_CONTROL  = 0x0F, // Control

      TCS34725_RA_WIA      = 0x12, // Who Am I/ID

      TCS34725_RA_CDATAL   = 0x14, // Clear channel data low
      TCS34725_RA_CDATAH   = 0x15, // Clear channel data high
      TCS34725_RA_RDATAL   = 0x16, // Red channel data low
      TCS34725_RA_RDATAH   = 0x17, // Red channel data high
      TCS34725_RA_GDATAL   = 0x18, // Green channel data low
      TCS34725_RA_GDATAH   = 0x19, // Green channel data high
      TCS34725_RA_BDATAL   = 0x1A, // Blue channel data low
      TCS34725_RA_BDATAH   = 0x1B, // Blue channel data high

      // These items aren't registers, but they can fit in this enum
      TCS34725_ADDRESS     = 0x29,
      TCS34725_WIA_VAL     = 0x44, // Who Am I/ID value
      TCS34725_COMMAND_BIT = 0x80,
    };

    // Enable register values
    enum : uint8_t {
      TCS34725_ENABLE_AIEN = 0x10, // RGBC Interrupt enable
      TCS34725_ENABLE_WEN  = 0x08, // Wait enable - Writing 1 actives the wait timer
      TCS34725_ENABLE_AEN  = 0x02, // RGBC Enable - Writing 1 actives the ADC, 0 disables it
      TCS34725_ENABLE_PON  = 0x01, // Power on- Writing 1 activates the internal oscillator, 0 disables it
    };

    void update_color_values(void);

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
    // Magnitude of rgb
    uint16_t m_rgb_max;
};

#endif /* _TCS34725_DRIVER_HPP_ */

