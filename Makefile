#ARDUINO_DIR = /usr/share/arduino
#ARDUINO_MK := /usr/share/arduino
#MONITOR_PORT := /dev/ttyACM0
#BOARD_TAG := uno
#USER_LIB_PATH := /usr/share/arduino/libraries/
#include $(ARDUINO_MK)/Arduino.mk
#
#ARDUINO_LIBS += Wire

# Compilers
CC  = gcc
CXX = g++

# Compiler flags
CFLAGS   = -std=gnu11
CXXFLAGS = -std=c++11 -Wall -Werror

# Grab all sources
SOURCES += $(wildcard src/drivers/*.cpp)
SOURCES += $(wildcard src/modules/*.cpp)
SOURCES += $(wildcard src/util/*.cpp)

# Grab the test source files
TEST_DIR = src/tests
SOURCES  += $(wildcard $(TEST_DIR)/*.cpp)

OBJ_DIR   = build-tests
OBJS      = $(SOURCES:.cpp=.o)
OBJ_FILES = $(notdir $(OBJS))
OBJ_OUT   = $(foreach o,$(OBJ_FILES),$(OBJ_DIR)/$(o))

INCLUDE_DIRS = src src/modules src/util
VPATH     = $(INCLUDE_DIRS)
VPATH    += $(OBJ_DIR)

INCLUDES  = $(addprefix -I, $(INCLUDE_DIRS))
CXXFLAGS += $(INCLUDES)
CFLAGS   += $(INCLUDES)

#Dependency files to include
DEPS     := $(addsuffix .d, $(OBJ_OUT))
CXXFLAGS += -MP -MD -MF $(OBJ_DIR)/$(notdir $(@)).d -MQ $(@)

# Output
EXEC      = $(OBJ_DIR)/test.out

# Tools/Commands
MKDIR = mkdir -p
RM    = rm -rf
MV    = mv

%.o: %.cpp
	@$(MKDIR) $(OBJ_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $(OBJ_DIR)/$(notdir $@)

$(OBJ_DIR)/%.d: ;
.PRECIOUS: $(OBJ_DIR)/$(notdir %.d)

all: $(EXEC)

clean:
	@$(RM) $(OBJ_DIR)

$(EXEC): $(OBJS)
	$(CXX) $(OBJ_OUT) -o $(EXEC)

include $(DEPS)

