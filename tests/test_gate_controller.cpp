#include "catch.hpp"
#include <stdint.h>
#include <stdio.h>
#include "gate_controller.hpp"

static GateController::gate_state_t gate_state[GateController::GATE_MAX] = {};

void gate_toggle_cb(uint8_t gate, uint8_t state)
{
  gate_state[gate] = (GateController::gate_state_t)(state);
}

TEST_CASE( "Controller callbacks work", "[GateController]" )
{
  GateController controller(gate_toggle_cb);

  for (uint8_t i = 0; i < GateController::GATE_MAX; i++) {
    controller.set_ticks_between_gate((GateController::gates_t)i, 0);
  }

  printf("\n\nCalling tick 1\n");
  controller.tick(GateController::GATE2);
  printf("\n\nCalling tick 2\n");
  controller.tick(GateController::GATE3);
  printf("\n\nCalling tick 3\n");
  controller.tick(GateController::GATE1);
  printf("\n\nCalling tick 4\n");
  controller.tick(GateController::GATE2);
  printf("\n\n");
  controller.tick(GateController::GATE_MAX);
  printf("\n\n");
  controller.tick(GateController::GATE_MAX);
  printf("\n\n");
  controller.tick(GateController::GATE_MAX);
  printf("\n\n");
}
