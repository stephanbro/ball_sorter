#include "catch.hpp"
#include <stdint.h>
#include <stdio.h>
#include "gate_controller.hpp"

#include <vector>

static GateController::gate_state_t gate_state[GateController::GATE_MAX] = {};

void gate_toggle_cb(uint8_t gate, uint8_t state)
{
  gate_state[gate] = (GateController::gate_state_t)(state);
}

TEST_CASE( "Sensing gate toggles", "[GateController]" )
{
  GateController controller(gate_toggle_cb);

  for (uint8_t i = 0; i < GateController::GATE_MAX; i++) {
    controller.set_ticks_between_gate((GateController::gates_t)i, 0);
  }

  std::vector<GateController::gates_t> input = {
    GateController::GATE1,
    GateController::GATE1,
    GateController::GATE1,
    GateController::GATE_MAX,
    GateController::GATE_MAX,
    GateController::GATE1,
  };

  std::vector<GateController::gate_state_t> sensing_output = {
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::OPEN,
  };

  std::vector<GateController::gate_state_t> slot1_output = {
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
  };

  for (uint8_t i = 0; i < 6; i++) {
    controller.tick(input[i]);
    REQUIRE(gate_state[GateController::GATE_SENSING] == sensing_output[i]);
    REQUIRE(gate_state[GateController::GATE1] == slot1_output[i]);
  }
}
