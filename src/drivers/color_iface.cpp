#include "color_iface.hpp"

const uint8_t colorIface::rgb_color[colorIface::COLOR_MAX][3] = {
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

const char* colorIface::rgb_string[COLOR_MAX+1] = {
  "BLACK", "WHITE", "RED", "LIME", "BLUE", "YELLOW",
  "CYAN", "MAGENTA", "SILVER", "GRAY", "MAROON", "OLIVE",
  "GREEN", "PURPLE", "TEAL", "NAVY", "ORANGE",
  "NO_COLOR"
};



