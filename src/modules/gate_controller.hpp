
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

    enum gates_t {
      GATE_SENSING = 0,
      GATE1        = 1,
      GATE2        = 2,
      GATE3        = 3,
      GATE4        = 4,
      GATE_MAX     = 5,
    };

    enum gate_state_t {
      OPEN  = 1,
      CLOSE = 0,
      STATES = 2,
    };

    void tick(gates_t new_dest);
    void set_ticks_between_gate(gates_t gate, uint8_t ticks);

  private:
    void (*gate_toggle)(uint8_t, uint8_t);

    void gate_sensing_handler(gates_t new_dest);

    uint8_t m_ticks_between_gate[STATES][GATE_MAX];
    gate_state_t m_output[GATE_MAX];
    uint8_t m_tick_count[GATE_MAX];
    bool m_close_gate_sensing;

    RingFIFO m_gate_queue[GATE_MAX];
};

#endif /* _GATE_CONTROLLER_HPP_ */

