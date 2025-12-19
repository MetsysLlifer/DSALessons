CC = clang
# Check include paths: If Intel Mac use /usr/local/include, if M1/M2/M3 use /opt/homebrew/include
CFLAGS = -Wall -std=c99 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL

# The name of your final program
TARGET = game

# List of object files needed
OBJS = main.o physics.o graphics.o ui.o

# 1. Default Rule: Build the target
all: $(TARGET)

# 2. Link Rule: Combine all .o files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 3. Compile Rules: How to turn .c into .o
# The %.o: %.c pattern works for all files automatically
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 4. Utilities
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)