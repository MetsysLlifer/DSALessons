# 1. Compiler and Settings
CC = clang
# CFLAGS: Commands for compiling (Include paths)
# If on Intel Mac, change /opt/homebrew to /usr/local
CFLAGS = -Wall -std=c99 -I/opt/homebrew/include

# LDFLAGS: Commands for linking (Libraries and Frameworks)
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL

# 2. Final Executable Name
TARGET = sand_sim

# 3. The Build Rules
# The first rule is the default one run when you type 'make'
all: $(TARGET)

# Link Step: Combines all .o files into the executable
$(TARGET): main.o
	$(CC) main.o -o $(TARGET) $(LDFLAGS)

# Compile Steps: Turns .c files into .o files
main.o: main.c
	$(CC) -c main.c $(CFLAGS)


# 4. Helper Commands
# Type 'make run' to build AND run immediately
run: $(TARGET)
	./$(TARGET)

# Type 'make clean' to remove built files
clean:
	rm -f *.o $(TARGET)



# THIS WILL ME IMPLEMENTED IN THE FUTURE
# $(TARGET): main.o magic.o ui.o
# 	$(CC) main.o magic.o ui.o -o $(TARGET) $(LDFLAGS)
# magic.o: magic.c
# 	$(CC) -c magic.c $(CFLAGS)
# ui.o: ui.c
# 	$(CC) -c ui.c $(CFLAGS)