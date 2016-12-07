
# Arduino makefile defines
ARDUINO_DIR = /usr/share/arduino
ARDUINO_MK := /usr/share/arduino
MONITOR_PORT := /dev/ttyACM0
MONITOR_CMD = picocom
BOARD_TAG := uno
USER_LIB_PATH := ../

CFLAGS_STD = -std=c11
CXXFLAGS_STD = -std=c++11

ARDUINO_LIBS += Wire ball_sorter

ifeq ($(TESTS),)
# Does the building for arduino
    include $(ARDUINO_MK)/Arduino.mk
else
    include tests.mk
endif
