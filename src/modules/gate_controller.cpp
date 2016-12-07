
#include "gate_controller.hpp"
#include <string.h>

GateController::GateController(void (*gate_toggle_in)(uint8_t, uint8_t)) :
  gate_toggle(gate_toggle_in)
{
  for (uint8_t i = 0; i < SLOT_MAX; i++) {
    gate_buffer[i] = SLOT_MAX;
  }
}

void GateController::tick(slot_t new_dest)
{
  // Check each slot to update it's state
  bool gates_val[SLOT_MAX] = {};
  for (uint8_t i = 0; i < SLOT_MAX; i++) {
    if (gate_buffer[i] == i) {
      gates_val[i] = true;
      gate_buffer[i] = SLOT_MAX;
    }
  }
  for (uint8_t i = 0; i < SLOT_MAX; i++) {
    gate_toggle(i, gates_val[i]);
  }
  shift_buffer(new_dest);
}

void GateController::shift_buffer(slot_t new_dest)
{
  // Drop the last number off the buffer and shift
  memmove(&(gate_buffer[1]), &(gate_buffer[0]), sizeof(uint8_t)*3);
  // Add the newest number to the beginning of the buffer
  gate_buffer[0] = new_dest;
}
