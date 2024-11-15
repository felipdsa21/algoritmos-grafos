NAME := prim

TARGET := $(shell $(CXX) -dumpmachine)
WINDOWS := $(strip $(foreach W,msys w64 windows,$(findstring -$W-,-$(TARGET)-)))
BINEXT := $(if $(WINDOWS),.exe,)
OUT_FILE ?= $(NAME)$(BINEXT)

ERROR_CXXFLAGS := -Wall -Wconversion -Wextra -Wno-sign-conversion -Wpedantic -Wvla
CXXFLAGS := $(CXXFLAGS) $(ERROR_CXXFLAGS)

.PHONY: build

build: $(OUT_FILE)

$(OUT_FILE): $(NAME).cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
