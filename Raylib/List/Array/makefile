CC = clang
# Added -I. to find headers in your project folder
CFLAGS = -Wall -std=c99 -I. -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL

TARGET = game
# Assuming your files are in the root directory or you run make from where they are
OBJS = main.o util.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# This pattern handles compiling the .c files into .o files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)