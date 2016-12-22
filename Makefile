# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

ifneq (,$(filter tests clean, $(MAKECMDGOALS)))
include tests.mk
endif

all:
	platformio -f -c vim run

upload:
	platformio -f -c vim run --target upload

clean:
	platformio -f -c vim run --target clean
	@$(RM) $(OBJ_DIR)

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update:
	platformio -f -c vim update

tests: $(EXEC)

