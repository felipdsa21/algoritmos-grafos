NAME := kosaraju
OUT_FILE ?= $(OUT_DIR)$(NAME).bin

ERROR_CXXFLAGS := -Wall -Wconversion -Wextra -Wno-sign-conversion -Wpedantic -Wvla
CXXFLAGS := $(CXXFLAGS) $(ERROR_CXXFLAGS) -std=c++17

.PHONY: build

build: $(OUT_FILE)

$(OUT_FILE): $(NAME).cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
