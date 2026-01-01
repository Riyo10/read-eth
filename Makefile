# Makefile for Network Diagnostic Tool

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -I./include
LDFLAGS = -lcurl -lpthread -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = build

# Source files
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/network.c \
          $(SRC_DIR)/server.c \
          $(SRC_DIR)/json.c

# Object files
OBJECTS = $(BUILD_DIR)/main.o \
          $(BUILD_DIR)/network.o \
          $(BUILD_DIR)/server.o \
          $(BUILD_DIR)/json.o

# Target executable
TARGET = $(BIN_DIR)/network-diagnostic

# Default target
.PHONY: all clean install help run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "[LINKING] Building $@..."
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "[SUCCESS] Build complete: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "[COMPILING] $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "[CLEANING] Removing build artifacts..."
	rm -rf $(BUILD_DIR)
	@echo "[SUCCESS] Clean complete"

install: $(TARGET)
	@echo "[INSTALLING] Installing to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/network-diagnostic
	sudo chmod +x /usr/local/bin/network-diagnostic
	@echo "[SUCCESS] Installation complete"

uninstall:
	@echo "[UNINSTALLING] Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/network-diagnostic
	@echo "[SUCCESS] Uninstallation complete"

run: $(TARGET)
	@echo "[RUNNING] Starting Network Diagnostic Tool..."
	$(TARGET)

run-dev: $(TARGET)
	@echo "[RUNNING] Starting in development mode on port 8080..."
	$(TARGET) -p 8080

help:
	@echo "Network Diagnostic Tool - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make all       - Build the application (default)"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make install   - Install to /usr/local/bin"
	@echo "  make uninstall - Remove from /usr/local/bin"
	@echo "  make run       - Build and run the application"
	@echo "  make run-dev   - Build and run in development mode"
	@echo "  make help      - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - gcc compiler"
	@echo "  - libcurl development files"
	@echo "  - POSIX-compliant system (Linux, macOS, etc.)"
	@echo ""

.PHONY: all clean install uninstall run run-dev help
