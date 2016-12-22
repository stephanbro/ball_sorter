# Test building defines
CC  = gcc
CXX = g++

# Compiler flags
CFLAGS   = -std=c11 -Wall -Werror
CXXFLAGS = -std=c++11 -Wall -Werror

# Grab all sources
SOURCES += $(wildcard src/drivers/*.cpp)
SOURCES += $(wildcard src/modules/*.cpp)
SOURCES += $(wildcard src/util/*.cpp)
SOURCES += $(wildcard tests/*.cpp)

OBJ_DIR   = build-tests
OBJS      = $(SOURCES:.cpp=.o)
OBJ_FILES = $(notdir $(OBJS))
OBJ_OUT   = $(foreach o,$(OBJ_FILES),$(OBJ_DIR)/$(o))

INCLUDE_DIRS = src src/modules src/util vendor/catch/single_include
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

$(EXEC): $(OBJS)
	$(CXX) $(OBJ_OUT) -o $(EXEC)

include $(DEPS)

