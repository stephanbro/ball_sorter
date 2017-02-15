
#ifndef _GATE_CONTROLLER_HPP_
#define _GATE_CONTROLLER_HPP_

#include <stdint.h>
#include <stdbool.h>

#include "../util/RingFIFO.hpp"

class GateController
{
  public:
    GateController(void (*gate_toggle_in)(uint8_t, uint8_t));
    virtual ~GateController() {}

    enum gates_t : unsigned {
      GATE_SENSING = 0,
      GATE1        = 1,
      GATE2        = 2,
      GATE3        = 3,
      GATE4        = 4,
      GATE_MAX     = 5,
    };

    enum gate_state_t : unsigned {
      OPEN  = 1,
      CLOSE = 0,
      STATES = 2,
    };

    void tick(gates_t new_dest);
    void set_ticks_between_gate(gates_t gate, gate_state_t state, uint16_t ticks);

  private:
    void (*gate_toggle)(uint8_t, uint8_t);

    struct gate_info_s {
      uint16_t gate_timer[STATES];
      gate_state_t last_output;
      uint16_t wait_count;
      RingFIFO exit_queue;
    };
    gate_info_s m_gates[GATE_MAX];

    void gate_sensing_handler(gates_t new_dest);
};

#endif /* _GATE_CONTROLLER_HPP_ */

