CC = clang
CFLAGS = -Wall -std=c99 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL

TARGET = snowfall

all: $(TARGET)

$(TARGET): raylib_app.c
	$(CC) $(CFLAGS) -o $(TARGET) raylib_app.c $(LDFLAGS)


run: ${TARGET}
	./${TARGET}

clean:
	rm -f $(TARGET)
