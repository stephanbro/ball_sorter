#include "catch.hpp"
#include <stdint.h>
#include <stdio.h>
#include "gate_controller.hpp"

#include <vector>
#include <string>

static const std::string gate_string[GateController::STATES] = {"OPEN", "CLOSE"};
static GateController::gate_state_t gate_state[GateController::GATE_MAX] = {};

void gate_toggle_cb(uint8_t gate, uint8_t state)
{
  gate_state[gate] = (GateController::gate_state_t)(state);
}

TEST_CASE( "Sensing gate toggles", "[GateController]" )
{
  GateController controller(gate_toggle_cb);

  SECTION("Ticks between gates set to 0")
  {
    for (uint8_t i = 0; i < GateController::GATE_MAX; i++) {
      controller.set_ticks_between_gate((GateController::gates_t)i, GateController::OPEN, 0);
      controller.set_ticks_between_gate((GateController::gates_t)i, GateController::CLOSE, 0);
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
      GateController::OPEN,
      GateController::CLOSE,
      GateController::OPEN,
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
      REQUIRE(gate_string[gate_state[GateController::GATE_SENSING]] == gate_string[sensing_output[i]]);
      REQUIRE(gate_string[gate_state[GateController::GATE1]]        == gate_string[slot1_output[i]]);
    }
  }

  SECTION("Ticks between gates non 0")
  {
    for (uint8_t i = 0; i < GateController::GATE_MAX; i++) {
      controller.set_ticks_between_gate((GateController::gates_t)i, GateController::OPEN, 1);
      controller.set_ticks_between_gate((GateController::gates_t)i, GateController::CLOSE, 3);
    }

    std::vector<GateController::gates_t> input = {
      GateController::GATE1,
      GateController::GATE1,
      GateController::GATE1,
      GateController::GATE_MAX,
      GateController::GATE_MAX,
      GateController::GATE1,
      GateController::GATE1,
    };

    std::vector<GateController::gate_state_t> sensing_output = {
      GateController::CLOSE,
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
      GateController::CLOSE,
    };

    for (uint8_t i = 0; i < input.size(); i++) {
      controller.tick(input[i]);
      REQUIRE(gate_string[gate_state[GateController::GATE_SENSING]] == gate_string[sensing_output[i]]);
      REQUIRE(gate_string[gate_state[GateController::GATE1]]        == gate_string[slot1_output[i]]);
    }
  }
}

TEST_CASE( "All gates toggles", "[GateController]" )
{
  GateController controller(gate_toggle_cb);

  for (uint8_t i = 0; i < GateController::GATE_MAX; i++) {
    controller.set_ticks_between_gate((GateController::gates_t)i, GateController::OPEN, 0);
    controller.set_ticks_between_gate((GateController::gates_t)i, GateController::CLOSE, 0);
  }

  std::vector<GateController::gates_t> input = {
    GateController::GATE_MAX,
    GateController::GATE2,
    GateController::GATE_MAX,
    GateController::GATE1,
    GateController::GATE_MAX,
    GateController::GATE3,
    GateController::GATE_MAX,
    GateController::GATE4,
    GateController::GATE_MAX,
    GateController::GATE_MAX,
    GateController::GATE_MAX,
    GateController::GATE_MAX,
  };

  std::vector<GateController::gate_state_t> sensing_output = {
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
  };

  std::vector<GateController::gate_state_t> slot1_output = {
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
  };

  std::vector<GateController::gate_state_t> slot2_output = {
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
    GateController::CLOSE,
  };

  std::vector<GateController::gate_state_t> slot3_output = {
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::OPEN,
    GateController::CLOSE,
  };

  std::vector<GateController::gate_state_t> slot4_output = {
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
    GateController::CLOSE,
  };

  for (uint8_t i = 0; i < 12; i++) {
    controller.tick(input[i]);
    REQUIRE(gate_state[GateController::GATE_SENSING] == sensing_output[i]);
    REQUIRE(gate_state[GateController::GATE1]        == slot1_output[i]);
    REQUIRE(gate_state[GateController::GATE2]        == slot2_output[i]);
    REQUIRE(gate_state[GateController::GATE3]        == slot3_output[i]);
    REQUIRE(gate_state[GateController::GATE4]        == slot4_output[i]);
  }
}
