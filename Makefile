ARDUINO_DIR = /usr/share/arduino
ARDUINO_MK := /usr/share/arduino
MONITOR_PORT := /dev/ttyACM0
BOARD_TAG := uno
USER_LIB_PATH := /usr/share/arduino/libraries/
include $(ARDUINO_MK)/Arduino.mk

ARDUINO_LIBS += Wire
