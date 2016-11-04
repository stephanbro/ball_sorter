
// Standard includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Arduino includes
#include <Wire.h>

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


uint8_t  i2c_read8( uint8_t address, uint8_t reg);
uint16_t i2c_read16(uint8_t address, uint8_t reg);
void     i2c_write8(uint8_t address, uint8_t reg, uint8_t val);
void     i2c_read(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data);

// Sensor class
class TCS34725
{
  public:
    TCS34725() :
      m_is_inited(false),
      m_clear_last(0),
      m_red_last(0),
      m_blue_last(0),
      m_green_last(0)
    { }

    ~TCS34725() { }

    void init(void)
    {
      if (m_is_inited == true) {
        return;
      }
      uint8_t val = read8(TCS34725_RA_WIA);
      m_is_inited = (val == 0x44);
    }

    void enable(void)
    {
      if (m_is_inited == false) {
        return;
      }
      write8(TCS34725_RA_ENABLE, TCS34725_ENABLE_PON);
      delay(3);
      write8(TCS34725_RA_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
    }

    void disable(void)
    {
      if (m_is_inited == false) {
        return;
      }
      uint8_t val = read8(TCS34725_RA_ENABLE);
      write8(TCS34725_RA_ENABLE, val & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
    }

    void update_color_values(void)
    {
      if (m_is_inited == false) {
        Serial.println("Not inited");
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

    typedef enum COLOR {
      BLACK, WHITE, RED, LIME, BLUE, YELLOW,
      CYAN, MAGENTA, SILVER, GRAY, MAROON, OLIIVE,
      GREEN, PURPLE, TEAL, NAVY, ORANGE,
      COLOR_MAX
    } color_t;

    color_t get_color(void)
    {
      Serial.print("C: ");
      Serial.print(m_clear_last);
      Serial.print(" R: ");
      Serial.print(m_red_last);
      Serial.print(" G: ");
      Serial.print(m_green_last);
      Serial.print(" B: ");
      Serial.print(m_blue_last);
      Serial.println("");

      if (m_clear_last < 2000) {
        return COLOR_MAX;
      }

      color_t ret = COLOR_MAX;
      uint16_t d  = 65535;
      for (uint8_t i = 0; i < COLOR_MAX; i++) {
        uint16_t new_d = pow((m_red_last   - rgb_color[i][0])*0.30f, 2) +
                         pow((m_green_last - rgb_color[i][1])*0.59f, 2) +
                         pow((m_blue_last  - rgb_color[i][2])*0.11f, 2);
        if (d > new_d) {
          d = new_d;
          ret = (color_t)i;
        }
      }
      return ret;
    }

    static const uint8_t rgb_color[COLOR_MAX][3];
    static const char* rgb_string[COLOR_MAX+1];

  private:
    uint8_t  read8(uint8_t reg)
    {
      return i2c_read8(TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg);
    }
    uint16_t read16(uint8_t reg)
    {
      return i2c_read16(TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg);
    }

    void     write8(uint8_t reg, uint8_t val)
    {
      i2c_write8(TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg, val);
    }

    // Is driver inited?
    bool     m_is_inited;
    // Last color values
    uint8_t  m_red_last;
    uint8_t  m_green_last;
    uint8_t  m_blue_last;
    uint16_t m_clear_last;
    uint16_t m_rgb_max;
};

static TCS34725 g_tcs34725;

const uint8_t TCS34725::rgb_color[COLOR_MAX][3] = {
  {  0,   0,   0}, // BLACK
  {255, 255, 255}, // WHITE
  {255,   0,   0}, // RED
  {  0, 255,   0}, // LIME
  {  0,   0, 255}, // BLUE
  {255, 255,   0}, // YELLOW
  {  0, 255, 255}, // CYAN
  {255,   0, 255}, // MAGENTA
  {192, 192, 192}, // SILVER
  {128, 128, 128}, // GRAY
  {128,   0,   0}, // MAROON
  {128, 128,   0}, // OLIVE
  {  0, 128,   0}, // GREEN
  {128,   0, 128}, // PURPLEz
  {  0, 128, 128}, // TEAL
  {  0,   0, 128}, // NAVY
  {255,  69,   0}, // RED ORANGE
};

const char* TCS34725::rgb_string[COLOR_MAX+1] = {
  "BLACK", "WHITE", "RED", "LIME", "BLUE", "YELLOW",
  "CYAN", "MAGENTA", "SILVER", "GRAY", "MAROON", "OLIVE",
  "GREEN", "PURPLE", "TEAL", "NAVY", "ORANGE",
  "NO_COLOR"
};

void setup()
{
  // Setup I2C
  Wire.begin();

  // Setup debug console over usb
  Serial.begin(115200);

  g_tcs34725.init();
  g_tcs34725.enable();

}

void loop()
{
  g_tcs34725.update_color_values();
  Serial.println(TCS34725::rgb_string[g_tcs34725.get_color()]);
  delay(1000);
}

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


