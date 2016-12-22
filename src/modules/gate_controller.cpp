
#include "gate_controller.hpp"
#include <string.h>

GateController::GateController(void (*gate_toggle_in)(uint8_t, uint8_t)) :
  gate_toggle(gate_toggle_in),
  m_ticks_between_gate{20, 20, 20, 20, 20},
  m_tick_count{},
  m_close_gate_sensing(true)
{
}

void GateController::tick(gates_t new_dest)
{
  // Iterate in reverse order to prevent a destination
  // from moving to the end in a single tick
  for (int16_t i = GATE_MAX-1; i >= 0; i--) {
    // Skip this gate if the tick count hasn't been met
    if (++(m_tick_count[i]) < m_ticks_between_gate[i]) {
      continue;
    }
    m_tick_count[i] = 0;
    // Special handling for the sensing gate
    if (i == GATE_SENSING) {
      gate_sensing_handler(new_dest);
      continue;
    }
    // Get the destination from the previous gate's queue
    gates_t dest = (gates_t)m_gate_queue[i-1].pop();
    gate_state_t output = OPEN;
    // Close the gate if the front of the queue has this gate
    // as it's destination or the queue is empty
    if (dest == i || dest == RingFIFO::EMPTY) {
      output = CLOSE;
    // Push the destination on the next queue
    } else {
      m_gate_queue[i].push(dest);
    }
    gate_toggle(i, output);
  }
}

void GateController::gate_sensing_handler(gates_t new_dest)
{
  // Is there a signal from last tick to close the gate?
  if (m_close_gate_sensing == true) {
    m_close_gate_sensing = false;
    gate_toggle(GATE_SENSING, CLOSE);
  }
  // Is there something new?
  else if (new_dest < GATE_MAX) {
    m_close_gate_sensing = true;
    m_gate_queue[GATE_SENSING].push(new_dest);
    gate_toggle(GATE_SENSING, OPEN);
  }
}

void GateController::set_ticks_between_gate(gates_t gate, uint8_t ticks)
{
  if (gate >= GATE_MAX) {
    return;
  }
  m_ticks_between_gate[gate] = ticks;
}

