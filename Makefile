# Compiler and linker
C = gcc
CFLAGS = -Iraylib-5.0_linux_amd64/include -g
LDFLAGS = -lraylib -lepoxy -lglfw -lassimp -lcglm -lm
# Project-specific settings
TARGET = main
SRCS = $(wildcard src/*.c)  # Add your source files here
OBJS = $(SRCS:src/%.c=build/%.o)

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(C) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files to object files
build/%.o: src/%.c
	mkdir -p build
	$(C) -c $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)
	rm -rf build

.PHONY: all clean run