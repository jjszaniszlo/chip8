SRC_DIR=src
BUILD_DIR=build
BUILD_EXECUTABLE=main

INCLUDE_DIR:=include /opt/homebrew/include

LIB_DIR:=/opt/homebrew/lib
LIBS:=glfw

CXX=gcc
SRC_EXT=c

CXXFLAGS:=-std=c11 -g -Wall -D_THREAD_SAFE
LDFLAGS:=

INCLUDES:=$(addprefix -I,$(INCLUDE_DIR))
LIBRARIES:=$(addprefix -l,$(LIBS))
LIBRARY_DIRECTORIES:=$(addprefix -L,$(LIB_DIR))

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
mkdir?=mkdir -p
SRC:=$(call rwildcard,$(SRC_DIR),*.$(SRC_EXT))
OBJ:=$(patsubst %.$(SRC_EXT),$(BUILD_DIR)/obj/%.o,$(subst $(SRC_DIR)/,,$(SRC)))
LOCAL_DIR:=$(shell pwd)

#linker
$(BUILD_DIR)/$(BUILD_EXECUTABLE):$(OBJ)
	$(mkdir) $(dir $@)
	$(CXX) $(LDFLAGS) $(INCLUDES) $(LIBRARY_DIRECTORIES) $(LIBRARIES) $(OBJ) -o $@

#obj
$(BUILD_DIR)/obj/%.o:$(SRC_DIR)/%.$(SRC_EXT)
	$(mkdir) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean
all: $(BUILD_DIR)/$(BUILD_EXECUTABLE)

clean:
	rm -rf build

update_examples:
	rm -f $(LOCAL_DIR)/examples/main
	make clean
	make all
	cp $(LOCAL_DIR)/build/main $(LOCAL_DIR)/examples/main

bear:
	make clean
	rm -f compile_commands.json
	bear -- make all

