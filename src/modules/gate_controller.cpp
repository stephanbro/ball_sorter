
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
    // TODO: An interrupt needs to trigger to close
    // this gate after a certain amount of time
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
  //TODO: This buffer should not be shifted every tick most likely
  shift_buffer(new_dest);
}

void GateController::shift_buffer(gates_t new_dest)
{
  // Add the newest number to the beginning of the buffer
  gate_buffer[GATE_SENSING] = new_dest;
  // Shift the buffer along
  memmove(&(gate_buffer[GATE1]), &(gate_buffer[GATE_SENSING]), sizeof(uint8_t)*(GATE_MAX-1));
}
