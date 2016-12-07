
#ifndef _GATE_CONTROLLER_HPP_
#define _GATE_CONTROLLER_HPP_

#include <stdint.h>

class GateController
{
  public:
    GateController(void (*gate_toggle_in)(uint8_t, uint8_t));
    virtual ~GateController() {}

    enum slot_t {
      SLOT1, SLOT2, SLOT3, SLOT4, SLOT_MAX,
    };

    void tick(slot_t new_dest);

  private:
    void (*gate_toggle)(uint8_t, uint8_t);

    void shift_buffer(slot_t new_dest);

    uint8_t gate_buffer[SLOT_MAX];
};

#endif /* _GATE_CONTROLLER_HPP_ */

