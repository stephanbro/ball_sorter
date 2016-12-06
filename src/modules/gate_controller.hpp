
#ifndef _GATE_CONTROLLER_HPP_
#define _GATE_CONTROLLER_HPP_

#include <stdint.h>
#include <functional>

class GateController
{
  public:
    GateController();
    virtual ~GateController() {}

    enum slot_t {
      SLOT1, SLOT2, SLOT3, SLOT4, SLOT_MAX,
    };

    void tick(slot_t new_dest);

    std::function<void(uint8_t, bool)> gate_toggle;

  private:
    void shift_buffer(slot_t new_dest);

    uint8_t gate_buffer[SLOT_MAX];
};

#endif /* _GATE_CONTROLLER_HPP_ */

