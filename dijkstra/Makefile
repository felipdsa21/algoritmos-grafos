NAME := dijkstra

TARGET := $(shell $(CXX) -dumpmachine)
WINDOWS := $(strip $(foreach W,msys w64 windows,$(findstring -$W-,-$(TARGET)-)))
BINEXT := $(if $(WINDOWS),.exe,)

ERROR_CXXFLAGS := -Wall -Wconversion -Werror -Wextra -Wpedantic -Wvla
CXXFLAGS := $(CXXFLAGS) $(ERROR_CXXFLAGS)

.PHONY: build

build: $(NAME)$(BINEXT)

$(NAME)$(BINEXT): $(NAME).cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
