
#include "gate_controller.hpp"
#include <string.h>

GateController::GateController(void (*gate_toggle_in)(uint8_t, uint8_t)) :
  gate_toggle(gate_toggle_in)
{
  m_gates[GATE_SENSING].gate_timer[CLOSE] = 750; // Time from close -> open
  m_gates[GATE_SENSING].gate_timer[OPEN]  = 50;  // Time from  open -> close
  m_gates[GATE_SENSING].last_output = CLOSE;
  m_gates[GATE_SENSING].wait_count = 0;

  m_gates[GATE1].gate_timer[CLOSE] = 100;
  m_gates[GATE1].gate_timer[OPEN] = 400;
  m_gates[GATE1].last_output = CLOSE;
  m_gates[GATE1].wait_count = 0;

  m_gates[GATE2].gate_timer[CLOSE] = 100;
  m_gates[GATE2].gate_timer[OPEN] = 500;
  m_gates[GATE2].last_output = CLOSE;
  m_gates[GATE2].wait_count = 0;

  m_gates[GATE3].gate_timer[CLOSE] = 250;
  m_gates[GATE3].gate_timer[OPEN] = 100;
  m_gates[GATE3].last_output = CLOSE;
  m_gates[GATE3].wait_count = 0;

  m_gates[GATE4].gate_timer[CLOSE] = 250;
  m_gates[GATE4].gate_timer[OPEN] = 250;
  m_gates[GATE4].last_output = CLOSE;
  m_gates[GATE4].wait_count = 0;

  // Initialize outputs
  for (unsigned i = 0; i < GATE_MAX; i++) {
    gate_toggle(i, m_gates[i].last_output);
  }
}

void GateController::tick(gates_t new_dest)
{
  // Iterate in reverse order to prevent a destination
  // from moving to the end in a single tick
  for (unsigned i = GATE_MAX-1; i >= GATE_SENSING && i < GATE_MAX; i--) {
    gate_info_s* gate = &(m_gates[i]);
    // Skip this gate if the tick count hasn't been met
    if (++(gate->wait_count) < gate->gate_timer[gate->last_output]) {
      continue;
    }
    gate->wait_count = 0;
    // Special handling for the sensing gate
    if (i == GATE_SENSING) {
      gate_sensing_handler(new_dest);
      continue;
    }

    if (i == 0) {
      // 0 should be handled above,
      // but check it here to prevent a crash from bad code
      continue;
    }

    // Get the destination from the previous gate's exit queue
    gates_t dest = (gates_t)m_gates[i-1].exit_queue.pop();
    gate_state_t output = OPEN;

    // Close the gate if the destination is this gate
    // or the gate is at rest
    if (dest == i || dest == RingFIFO::EMPTY) {
      output = CLOSE;
    // Add the destination to this gate's exit queue
    } else {
      gate->exit_queue.push(dest);
    }
    gate_toggle(i, output);
    gate->last_output = output;
  }
}

void GateController::gate_sensing_handler(gates_t new_dest)
{
  gate_state_t output = CLOSE;

  if (new_dest < GATE_MAX) {
    // Check if the sensing gate was just closed
    if (m_gates[GATE_SENSING].last_output == CLOSE) {
      output = OPEN;
      m_gates[GATE_SENSING].exit_queue.push(new_dest);
    }
  }

  gate_toggle(GATE_SENSING, output);
  m_gates[GATE_SENSING].last_output = output;
}

void GateController::set_ticks_between_gate(gates_t gate, gate_state_t state, uint16_t ticks)
{
  if (gate >= GATE_MAX || state >= STATES) {
    return;
  }

  m_gates[gate].gate_timer[state] = ticks;
}

