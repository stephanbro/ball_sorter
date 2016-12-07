
#include "gate_controller.hpp"
#include <string.h>

GateController::GateController(void (*gate_toggle_in)(uint8_t, uint8_t)) :
  gate_toggle(gate_toggle_in)
{
  for (uint8_t i = 0; i < GATE_MAX; i++) {
    gate_buffer[i] = GATE_MAX;
  }
}

void GateController::tick(gates_t new_dest)
{
  bool gates_val[GATE_MAX] = {};
  // Check if we sensed something new
  if (new_dest != GATE_MAX) {
    gates_val[GATE_SENSING] = OPEN;
  }
  // Check each slot to update it's state
  for (uint8_t i = GATE1; i < GATE_MAX; i++) {
    if (gate_buffer[i] == i) {
      gates_val[i] = CLOSE;
      gate_buffer[i] = GATE_MAX;
    } else {
      gates_val[i] = OPEN;
    }
  }
  for (uint8_t i = 0; i < GATE_MAX; i++) {
    gate_toggle(i, gates_val[i]);
  }
  shift_buffer(new_dest);
}

void GateController::shift_buffer(gates_t new_dest)
{
  // Drop the last number off the buffer and shift
  memmove(&(gate_buffer[1]), &(gate_buffer[0]), sizeof(uint8_t)*3);
  // Add the newest number to the beginning of the buffer
  gate_buffer[0] = new_dest;
}
