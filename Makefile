CXX ?= clang++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -Wpedantic -O2 -Iinclude
BUILD_DIR := build
HOMEBREW_PREFIX ?= $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
RAYLIB_CXXFLAGS ?= -I$(HOMEBREW_PREFIX)/include
RAYLIB_LDFLAGS ?= -L$(HOMEBREW_PREFIX)/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

PHYSICS_SRC := src/physics/mechanics/particle.cpp src/physics/mechanics/projectile_lab.cpp src/physics/mechanics/pendulum_lab.cpp

.PHONY: all console plots raylib clean

all: console

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

console: $(BUILD_DIR)/projectile_console $(BUILD_DIR)/pendulum_console

$(BUILD_DIR)/projectile_console: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PHYSICS_SRC) labs/projectile/projectile_console.cpp -o $(BUILD_DIR)/projectile_console

$(BUILD_DIR)/pendulum_console: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PHYSICS_SRC) labs/pendulum/pendulum_console.cpp -o $(BUILD_DIR)/pendulum_console

plots: console
	./$(BUILD_DIR)/projectile_console
	python3 labs/projectile/plot_projectile.py
	./$(BUILD_DIR)/pendulum_console
	python3 labs/pendulum/plot_pendulum.py

raylib: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(RAYLIB_CXXFLAGS) $(PHYSICS_SRC) labs/projectile/projectile_raylib.cpp -o $(BUILD_DIR)/projectile_raylib $(RAYLIB_LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)
