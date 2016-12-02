#include "catch.hpp"
#include <stdint.h>
#include <stdio.h>
#include "gate_controller.hpp"

void gate_toggle_cb(uint8_t i, bool j)
{
  printf("%d ", j);
  //if (j == true) {
  //  printf("Closing gate %i\n", i+1);
  //}
}

TEST_CASE( "Controller callbacks work", "[GateController]" )
{
  GateController controller;
  controller.gate_toggle = gate_toggle_cb;

  printf("\n\nCalling tick 1\n");
  controller.tick(GateController::SLOT2);
  printf("\n\nCalling tick 2\n");
  controller.tick(GateController::SLOT3);
  printf("\n\nCalling tick 3\n");
  controller.tick(GateController::SLOT1);
  printf("\n\nCalling tick 4\n");
  controller.tick(GateController::SLOT2);
  printf("\n\n");
  controller.tick(GateController::SLOT_MAX);
  printf("\n\n");
  controller.tick(GateController::SLOT_MAX);
  printf("\n\n");
  controller.tick(GateController::SLOT_MAX);
  printf("\n\n");
}
