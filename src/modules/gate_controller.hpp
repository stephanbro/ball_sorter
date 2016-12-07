
#ifndef _GATE_CONTROLLER_HPP_
#define _GATE_CONTROLLER_HPP_

#include <stdint.h>

class GateController
{
  public:
    GateController(void (*gate_toggle_in)(uint8_t, uint8_t));
    virtual ~GateController() {}

    enum gates_t {
      GATE_SENSING, GATE1, GATE2, GATE3, GATE4, GATE_MAX,
    };

    enum gate_state_t {
      OPEN = 0, CLOSE = 1
    };

    void tick(gates_t new_dest);

  private:
    void (*gate_toggle)(uint8_t, uint8_t);

    void shift_buffer(gates_t new_dest);

    uint8_t gate_buffer[GATE_MAX];
};

#endif /* _GATE_CONTROLLER_HPP_ */

