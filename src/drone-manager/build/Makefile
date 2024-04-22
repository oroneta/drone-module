CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN_DIR := ../bin
SRC_DIR := ./cc
BUILD_DIR := .
HEAD_DIR := ./h

SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(SRC_FILES:%=$(BUILD_DIR)/%.o)

INCLUDE	:= -Iinclude -Ilib \
 -I/usr/local/include/mongocxx/v_noabi \
 -I/usr/local/include/bsoncxx/v_noabi \
 -I/usr/include/mavsdk \
 -I/usr/local/include/mavsdk\
 -I/usr/include/jsoncpp \
 -I$(HEAD_DIR)
LIB		:= lib

LIBRARIES	:= -lboost_system -lpthread -lcrypto \
-lssl -lmongocxx -lbsoncxx -lmavsdk
LDLIBS := -L/usr/lib
EXECUTABLE	:= drone_manager

all: $(BIN_DIR)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) $(LDLIBS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) -c $< -o $@ $(LIBRARIES) $(LDLIBS)
	

clean:
	-rm $(BIN_DIR)/*
	-rm $(OBJ_FILES)

.PHONY: print clear client
print: $(wildcard $(SRC_DIR)/*.cpp)
	ls -la  $?

clear:
	rm -f $(SRC_DIR)/*.o

client: ../src/cc/dronePrueba.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) $(LDLIBS)
	mv client ../bin