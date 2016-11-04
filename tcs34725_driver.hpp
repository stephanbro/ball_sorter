// Sensor class
class TCS34725
{
  public:
    TCS34725();  // Constructor
    ~TCS34725(); // Destructor

    void init(void);

    void enable(void);

    void disable(void);

    void update_color_values(void);

    typedef enum COLOR {
      BLACK, WHITE, RED, LIME, BLUE, YELLOW,
      CYAN, MAGENTA, SILVER, GRAY, MAROON, OLIIVE,
      GREEN, PURPLE, TEAL, NAVY, ORANGE,
      COLOR_MAX
    } color_t;

    color_t get_color(void);

    static const uint8_t rgb_color[COLOR_MAX][3];
    static const char* rgb_string[COLOR_MAX+1];

    uint8_t  (*read8_cb)(uint8_t, uint8_t);
    uint16_t (*read16_cb)(uint8_t, uint8_t);
    void     (*write8_cb)(uint8_t, uint8_t, uint8_t);

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

